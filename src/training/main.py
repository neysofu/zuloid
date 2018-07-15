#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import chess
import chess.pgn
import numpy as np
import tensorflow as tf
import keras as ks
import sqlite3
import scipy
import random as rnd
import logging
import sys
import argparse
import uuid
import os
#import numba as nb
from chess import pgn
from os.path import expanduser
from multiprocessing import Pool # For A3C (TODO)
from scipy import spatial

SEED = 146
HOT_BOARD_TENSOR_WIDTH = 455
BOARD_TENSOR_WIDTH = 512
MOVE_TENSOR_WIDTH = 5
AVERAGE_NUM_OF_MOVES_IN_CHESS_GAME = 54
MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW = 50
RATIO_OF_CHESS_960_GAMES_DURING_TRAINING = 0.4
EXPLORATION_RATE_DURING_TRAINING = 0.08
DISCOUNT_VALUE = 0.96
LOGGING_FORMAT = "%(asctime)s @ '%(name)s.%(funcName)s:%(lineno)d' [%(levelname)s]: %(message)s"
LOGGING_DATE_FORMAT = "%Y-%m-%dT%H:%M:%S%z"
BOARD_TO_MOVE_FILENAME = "Board2move.h5"
BOARD_INCR_TO_BOARD_FILENAME = "BoardIncr2Board.h5"
HOT_TO_BOARD_FILENAME = "Hot2board.h5"

__author__ = "Filippo Costa @neysofu"
__version__ = "0.2"


class Board(chess.Board):
    """
    A custom subclass of 'chess.Board' to support some non-standard operations.
    """

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def to_tensor(self):
        return np.array([[
            bit
            for bitmask in [
                piece_to_tensor(self.piece_at(square))
                for square in chess.SQUARES
            ] for bit in bitmask
        ] + [
            self.turn,
            self.has_kingside_castling_rights(chess.WHITE),
            self.has_kingside_castling_rights(chess.BLACK),
            self.has_queenside_castling_rights(chess.WHITE),
            self.has_queenside_castling_rights(chess.BLACK),
            min(self.halfmove_clock, MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW) /
            MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW, self.fullmove_number /
            (self.fullmove_number + AVERAGE_NUM_OF_MOVES_IN_CHESS_GAME)
        ]])

    def stochastic_push(self, q_values):
        """
        Explores the board accoring to some Q values over a subset of the legal
        moves. In its simplest form, it is implemented as a form of
        epsilon-greedy exploration, but can be tuned for better performance.
        Returns the chosen move.
        """
        # TODO: exploration.
        return q_values[max(q_values.keys())]

    def adjust_move_tensor(self, move_tensor):
        def move_tensor_diff(tensor_1, tensor_2):
            """
            Compute the difference between two move tensors.
            """
            source_diff = scipy.spatial.distance.euclidean(tensor_1[:2],
                                                           tensor_2[:2])
            target_diff = scipy.spatial.distance.euclidean(tensor_1[2:],
                                                           tensor_2[2:])
            return source_diff ** 2 + target_diff
        tensor_1 = [x * 8 for x in move_tensor[0][:-1]]
        legal_moves = list(self.legal_moves)
        min_diff = 139 # Maximum diff between two moves.
        for i, legal_move in enumerate(legal_moves):
            tensor_2 = [chess.square_file(legal_move.from_square),
                        chess.square_rank(legal_move.from_square),
                        chess.square_file(legal_move.to_square),
                        chess.square_rank(legal_move.to_square)]
            diff = move_tensor_diff(tensor_1, tensor_2)
            if diff < min_diff:
                min_diff = diff
                move_i = i
        return (legal_moves[move_i], min_diff / 139)

def numeric_result(board):
    result = board.result()
    if result == "1-0":
        return 1
    elif result == "1/2-1/2":
        return 0
    elif result == "0-1":
        return -1
    raise ValueError("The result of the chess game doesn't yet exist"
                     " (it is {}).".format(result))

def piece_to_tensor(piece):
    if not piece:
        return np.zeros(7)
    return [
        piece.piece_type == chess.PAWN, piece.piece_type == chess.KNIGHT,
        piece.piece_type in [chess.BISHOP, chess.QUEEN], piece.piece_type in [
            chess.ROOK, chess.QUEEN
        ], piece.piece_type == chess.KING, piece.color, not piece.color
    ]

class Agent:

    # Hyperparameters. Boy, isn't ML fun...
    LEARNING_RATE = 0.0004
    EXPLORATION_RATE = 0.85
    DISCOUNT_VALUE = 1.0
    BATCH_SIZE = 512
    # All hyperparameter values decay over time to guarantee convergence.
    DECAY_RATE = 2E-5

    def __init__(self, database):
        self.db = database
        self.db_i = 0
        self.model = Agent.playing_model()
        self.read_from_disk_if_exists()
        self.batch = []
        self.learning_rate = Agent.LEARNING_RATE
        self.exploration_rate = Agent.EXPLORATION_RATE
        self.discount_value = Agent.DISCOUNT_VALUE
        # TODO: hyperparameter values decay.
        num_episodes = int(ARGS.num_episodes)
        self.episode_i = 0
        while self.episode_i < num_episodes:
            self.play(setup_board())
            self.episode_i += 1
        self.write_to_disk()

    def play(self, board):
        """
        Test the agent at a game of chess against itselt and register what
        happens. These information will then be used to train the model.
        """
        # FIXME: currently ignored.
        resoluteness_factor = np.random.random()
        LOGGER.debug(f"New game (episode n.{self.episode_i + 1}) with "
                    f"initial position '{board.fen()}', "
                    f"resoluteness factor ~{round(resoluteness_factor, 3)}.")
        i = 0
        predictions = []
        while not board.is_game_over():
            input_tensor = board.to_tensor()
            output_tensor = self.model.predict(input_tensor)
            predictions.append((input_tensor, output_tensor))
            # FIXME
            move = board.adjust_move_tensor(output_tensor)[0]
            board.push(move)
            i += 1
        LOGGER.debug(f"{i} moves were played. The result is "
                     f"{board.result()}.")
        self.queue_for_training(chess.pgn.Game.from_board(board), predictions)

    def playing_model():
        model = ks.models.Sequential()
        model.add(
            ks.layers.Dense(
                HOT_BOARD_TENSOR_WIDTH,
                activation="linear",
                input_dim=HOT_BOARD_TENSOR_WIDTH))
        model.add(ks.layers.Dropout(0.2))
        model.add(ks.layers.LeakyReLU())
        model.add(ks.layers.Dense(64, activation="linear"))
        model.add(ks.layers.LeakyReLU())
        model.add(ks.layers.Dense(MOVE_TENSOR_WIDTH, activation="sigmoid"))
        model.compile(loss="categorical_crossentropy",
                      optimizer=ks.optimizers.Adamax(lr=Agent.LEARNING_RATE,
                                                        decay=Agent.DECAY_RATE),
                      metrics=["accuracy"])
        return model

    def queue_for_training(self, game, predictions):
        """
        Train the model based on a game played by the AI. Its inputs along with
        the relative outputs are included to avoid duplicate computation.
        """
        state_values = backpropagate_state_value([p[1][0][4] for p in predictions],
                                                 game)
        for i, prediction in enumerate(predictions):
            prediction[1][0][4] = state_values[i]
        target_predictions = predictions
        self.batch.append(target_predictions)
        if len(self.batch) >= Agent.BATCH_SIZE:
            self.model.fit([x[0] for x in self.batch],
                           [x[1] for x in self.batch])

    def read_from_disk_if_exists(self):
        try:
            self.model.load_weights(ARGS.output_dir + "/model.h5")
            LOGGER.info("Successfully loaded weights from the last session.")
        except:
            LOGGER.error("The weights of the model couldn't be found. New "
                         "weights will be initialized.")

    def write_to_disk(self):
        self.model.save(ARGS.output_dir + "/model.h5")

def setup_board():
    chess960 = rnd.uniform(0, 1) < RATIO_OF_CHESS_960_GAMES_DURING_TRAINING
    return Board(chess960=chess960)

def proximal_policy_optimization_loss(actual_value, predicted_value, old_prediction):
    advantage = actual_value - predicted_value
    def loss(y_true, y_pred):
        prob = K.sum(y_true * y_pred)
        old_prob = K.sum(y_true * old_prediction)
        r = prob/(old_prob + 1e-10)
        return -K.log(prob + 1e-10) * K.mean(K.minimum(r * advantage, K.clip(r, min_value=0.8, max_value=1.2) * advantage))
    return loss

def backpropagate_state_value(state_values, game):
    """
    Backpropagates the state values estimated during a game of chess according
    the TD(lambda) algorithm.
    See http://www.incompleteideas.net/book/ebook/node75.html for details.
    """
    # TODO: find a custom fitting curve specific to chess. See
    # https://lichess.org/insights/LeelaChess/acpl/material
    result = game.end().board().result()
    eligibility_traces = np.zeros(len(state_values))
    trace_decay_parameter = 0.01
    discount_rate = 0.96
    weight = 0.82
    for i in range(len(state_values)):
        eligibility_traces *= trace_decay_parameter * discount_rate
        eligibility_traces[i] += 1
        #error = result + state_values[i+1] - state_values[i]
        #state_values += alpha * error * eligibility_traces
    return state_values

def init_logger(log_file_path):
    logger = logging.getLogger()
    logging.basicConfig(
        level=logging.DEBUG,
        format=LOGGING_FORMAT,
        datefmt=LOGGING_DATE_FORMAT,
        stream=sys.stdout,
        filemode="w")
    formatter = logging.Formatter(
        fmt=LOGGING_FORMAT, datefmt=LOGGING_DATE_FORMAT)
    file_handler = logging.FileHandler(log_file_path)
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)
    return logger

def cli():
    parser = argparse.ArgumentParser()
    default_output_dir = expanduser("~") + "/Z64C-training-output/"
    parser.add_argument("-o",
                        "--output-dir",
                        default=default_output_dir)
    parser.add_argument("-n",
                        "--num-episodes",
                        type=int,
                        default="1")
    return parser

def main():
    global ARGS
    global LOGGER
    global SESSION_ID
    tf.logging.set_verbosity(tf.logging.ERROR)
    ARGS = cli().parse_args(sys.argv[1:])
    os.makedirs(ARGS.output_dir, exist_ok=True)
    LOGGER = init_logger("{}/session.log".format(ARGS.output_dir))
    SESSION_ID = uuid.uuid1()
    print("")
    LOGGER.info(f"New training session with ID {SESSION_ID}.")
    tf.set_random_seed(SEED)
    np.random.seed(SEED)
    rnd.seed(SEED)
    tf.reset_default_graph()
    Agent(Board())
    LOGGER.info("Goodbye.")

def exit_gracefully():
    # TODO
    pass


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        LOGGER.info("Keyboard interrupt received. Exiting gracefully. Press "
                    "CTRL+C a second time to exit immediately and disregard any"
                    " changes.")
        exit_gracefully()
