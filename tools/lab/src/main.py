#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import chess
import chess.pgn
import numpy as np
import tensorflow as tf
import keras as ks
import scipy
import random as rnd
import logging
import sys
import argparse
import os
from chess import pgn
from os.path import expanduser
from multiprocessing import Pool  # For A3C (TODO)
from scipy import spatial
from sklearn.metrics.pairwise import cosine_similarity


SEED = 146
HOT_BOARD_TENSOR_WIDTH = 455
BOARD_TENSOR_WIDTH = 512
MOVE_TENSOR_WIDTH = 5
AVERAGE_NUM_OF_MOVES_IN_CHESS_GAME = 54
MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW = 50
RATIO_OF_CHESS_960_GAMES_DURING_TRAINING = 0.175
EXPLORATION_RATE_DURING_TRAINING = 0.08
DISCOUNT_VALUE = 0.96
LOSS_CLIPPING = 0.2
LOGGING_FORMAT = "%(asctime)s @ '%(name)s.%(funcName)s:%(lineno)d' [%(levelname)s]: %(message)s"
LOGGING_DATE_FORMAT = "%Y-%m-%dT%H:%M:%S%z"

__author__ = "Filippo Costa"
__version__ = "0.3"


class Board2Vec:

    def __init__(self):
        self.model = Board2Vec.model()

    def fit(self, board, depth):
        """
        Fits the given chessboard prediction according to the game tree up to
        a certain depth.
        """
        original_board_hot = board.clone()
        target_similarities = {}
        for i in range(depth):
            for move in board.legal_moves:
                board.push(move)
                boards.append(board)
                board_hot = board_to_one_hot_encoding(board)
                target_similarities[board_to_one_hot_encoding(
                    board)] = cosine_similarity(board_hot, original_board_hot)
                board.pop(move)
        board.push(random.choice(board.legal_moves))
        for b1 in boards:
            for b2 in boards:
                b1_moves = set(b1.legal_moves)
        b1_vec = self.model.predict(b1_hot_encoding)
        b2_vec = self.model.predict(b2_hot_encoding)
        # self.model.fit(b1_hot_encoding)

    def model():
        model = ks.layers.Sequential()
        model.add(ks.Conv2D(32, kernel_size=(3, 3),
                            activation='relu',
                            input_shape=input_shape))
        model.add(ks.layers.Conv2D(64, (3, 3), activation='relu'))
        model.add(ks.layers.MaxPooling2D(pool_size=(2, 2)))
        model.add(ks.layers.Flatten())
        model.add(ks.layers.Dense(128, activation='relu'))
        model.add(ks.layers.Dense(num_classes, activation='softmax'))
        model.compile(loss=keras.losses.categorical_crossentropy,
                      optimizer=keras.optimizers.Adadelta(),
                      metrics=['accuracy'])


def board_to_one_hot_encoding(self):
    return np.array([[
        bit for bitmask in [
            piece_to_one_hot_encoding(self.piece_at(square))
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
        min_diff = 139  # Maximum diff between two moves.
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

    def give_reward(self, move):
        pass
        # TODO
        # if self.is_capture(move):
        #    return 0.2
        # if self.is_check(move):
        #    return 0.05


def numeric_result(board):
    result = board.result()
    if result == "1-0":
        return 1
    elif result == "1/2-1/2":
        return 0.5
    elif result == "0-1":
        return 0
    raise ValueError("The result of the chess game doesn't yet exist"
                     " (it is {}).".format(result))


class Piece(chess.Piece):

    def to_tensor(self):
        if not self:
            return np.zeros(7)
        return [self.piece_type == chess.PAWN,
                self.piece_type == chess.KNIGHT,
                self.piece_type in [chess.BISHOP, chess.QUEEN],
                self.piece_type in [chess.ROOK, chess.QUEEN],
                self.piece_type == chess.KING,
                self.color,
                not self.color]


class Agent:

    # Hyperparameters. Boy, isn't ML fun...
    LEARNING_RATE = 0.004
    EXPLORATION_RATE = 0.85
    DISCOUNT_VALUE = 1.0
    BATCH_SIZE = 2
    # All hyperparameter values decay over time to guarantee convergence.
    DECAY_RATE = 2E-5

    def __init__(self, args):
        self.batch = []
        self.model = Agent.playing_model()
        self.read_from_disk_if_exists()
        self.num_episodes = int(args.num_episodes)
        self.episode_i = 0
        self.args = args
        if self.args.database:
            self.train_on_database(self.args.database)
        else:
            while self.episode_i < self.num_episodes:
                self.queue_for_training(self.run_episode(setup_board()))
                self.episode_i += 1
                if self.episode_i % 100 == 0:
                    self.write_to_disk()
        self.write_to_disk()

    def run_episode(self, board):
        """
        Test the agent at a game of chess against itselt and register what
        happens. These information will then be used to train the model.
        """
        # FIXME: currently ignored.
        resoluteness_factor = np.random.random()
        LOGGER.debug(f"New game (episode n.{self.episode_i + 1}) with "
                     f"initial position '{board.fen()}', "
                     f"resoluteness factor ~{round(resoluteness_factor, 3)}.")
        tensors_in = []
        tensors_out = []
        rewards = []
        i = 0
        while not board.is_game_over():
            tensor_in = board.to_tensor()
            tensor_out = self.model.predict(tensor_in)
            if np.random.random() < 0.3:
                move = rnd.choice(list(board.legal_moves))
                move_error = 0
            else:
                # FIXME: add move error
                move, move_error = board.adjust_move_tensor(tensor_out)
            reward = -move_error
            rewards.append(reward)
            tensors_in.append(tensor_in)
            tensors_out.append(tensor_out)
            board.push(move)
            active_color = ("White" if board.turn == chess.WHITE else "Black")
            LOGGER.debug(f"{active_color} moved {move}.")
            i += 1
        LOGGER.debug(f"{i} moves were played. The result is "
                     f"{board.result()}.")
        return Episode(chess.pgn.Game.from_board(board),
                       tensors_in,
                       tensors_out,
                       rewards)

    def playing_model():
        model = ks.models.Sequential()
        model.add(
            ks.layers.Dense(
                HOT_BOARD_TENSOR_WIDTH,
                activation="linear",
                input_dim=HOT_BOARD_TENSOR_WIDTH))
        model.add(ks.layers.LeakyReLU())
        model.add(ks.layers.Dense(64, activation="linear"))
        model.add(ks.layers.LeakyReLU())
        model.add(ks.layers.Dense(MOVE_TENSOR_WIDTH, activation="sigmoid"))
        model.compile(loss="categorical_crossentropy",
                      optimizer=ks.optimizers.Adamax(lr=Agent.LEARNING_RATE),
                      metrics=["accuracy"])
        return model

    def queue_for_training(self, episode, policy=True):
        """
        Train the model based on a game played by the AI. Its inputs along with
        the relative outputs are included to avoid duplicate computation.
        """
        episode.backpropagate_result()
        if policy:
            episode.search_for_better_moves(self.model)
        self.batch.append(episode)
        if len(self.batch) >= Agent.BATCH_SIZE:
            self.replay_experience()

    def replay_experience(self):
        rnd.shuffle(self.batch)
        for episode in self.batch:
            for i, t_in in enumerate(episode.tensors_in):
                self.model.fit(t_in, episode.tensors_out[i])

    def train_on_database(self, db):
        with read(db) as db:
            game = chess.pgn.read_game(db)
            if not game:
                return
            board = game.root().board()
            for move in game.main_line():
                t_in = Board.to_tensor(board)
                t_out = self.model.predict(t_in)
                move_tensor = move_to_tensor(move)
                t_out[0][1] = move_tensor[0]
                t_out[0][2] = move_tensor[1]
                t_out[0][3] = move_tensor[2]
                t_out[0][4] = move_tensor[3]
                tensors_in.append(tensor_in)
                tensors_out.append(tensor_out)
            episode = Episode(game,
                              tensors_in,
                              tensors_out,
                              [])
            self.queue_for_training(episode, policy=False)

    def read_from_disk_if_exists(self):
        try:
            self.model.load_weights(self.args.output_dir + "/model.h5")
            LOGGER.info("Successfully loaded weights from the last session.")
        except:
            LOGGER.error("The weights of the model couldn't be found. New "
                         "weights will be initialized.")

    def write_to_disk(self):
        self.model.save(self.args.output_dir + "/model.h5")


class Episode:

    def __init__(self,
                 game,
                 tensors_in,
                 tensors_out,
                 move_errors):
        self.game = game
        self.tensors_in = tensors_in
        self.tensors_out = tensors_out
        self.move_errors = move_errors

    def adjust_tensors_out(self, model):
        material_imbalance_weights = {-6.5: 5.3,
                                      -5: 21.7,
                                      -2: 24.4,
                                      -0.5: 24.2,
                                      0: 16.6,
                                      0.5: 19.8,
                                      2: 30.7,
                                      5: 18.6,
                                      6.5: 3.3}
        reward = numeric_result(game.end().board())
        eligibility_traces = np.zeros(len(state_values))
        trace_decay_parameter = 0.725
        discount_rate = 1.0
        for i in range(len(state_values) - 1):
            error = reward + discount_rate * \
                state_values[i+1] - state_values[i]
            eligibility_traces[i] += 1
            eligibility_traces *= discount_rate ** trace_decay_parameter
            state_values[i] += discount_rate * error * eligibility_traces[i]
        state_values[-1] = reward
        return state_values

    def backpropagate_result(self):
        """
        Backpropagates the state values estimated during a game of chess according
        the TD(64) algorithm.
        See http://www.incompleteideas.net/book/ebook/node75.html for details.
        """
        # TODO: find a custom fitting curve specific to chess. See
        # https://lichess.org/insights/LeelaChess/acpl/material
        material_imbalance_weights = {-6.5: 0.41549781766,
                                      -5: 0.8407380037,
                                      -2: 0.89150898684,
                                      -0.5: 0.88784774349,
                                      0: 0.73533435414,
                                      0.5: 0.80308850076,
                                      2: 1,
                                      5: 0.77837214247,
                                      6.5: 0.32785950753}
        result = numeric_result(self.game.end().board())
        eligibility_traces = np.zeros(len(self.tensors_out))
        trace_decay_parameter = 0.675
        alpha = 0.1
        for i in range(len(self.tensors_out) - 1):
            error = (result +
                     self.tensors_out[i+1][0][0] -
                     self.tensors_out[i][0][0])
            eligibility_traces[i] += 1
            eligibility_traces *= trace_decay_parameter
            for i in range(len(self.tensors_out) - 1):
                self.tensors_out[i][0][0] += error * \
                    alpha * eligibility_traces[i]
        self.tensors_out[-1][0][0] = result

    def search_for_better_moves(self, model):
        board = self.game.root().board()
        for i, move in enumerate(self.game.main_line()):
            if board.turn == chess.WHITE:
                best_state_value = -42
                comparison_function = np.float32.__ge__
            else:
                best_state_value = 42
                comparison_function = np.float32.__le__
            for legal_move in board.legal_moves:
                board.push(legal_move)
                state_value = model.predict(Board.to_tensor(board))[0][0]
                if comparison_function(state_value, best_state_value):
                    best_state_value = state_value
                    best_move = legal_move
                board.pop()
            board.push(move)
            move_tensor = move_to_tensor(best_move)
            self.tensors_out[i][0][1] = move_tensor[0]
            self.tensors_out[i][0][2] = move_tensor[1]
            self.tensors_out[i][0][3] = move_tensor[2]
            self.tensors_out[i][0][4] = move_tensor[3]


def setup_board():
    if np.random.random() < 0.5:
        return Board.from_chess960_pos(rnd.randint(0, 959))
    else:
        return Board()


def move_to_tensor(move):
    return [chess.square_file(move.from_square) / 8,
            chess.square_rank(move.from_square) / 8,
            chess.square_file(move.to_square) / 8,
            chess.square_rank(move.to_square) / 8]


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
    default_output_dir = expanduser("~") + "/Zuloid-training-output/"
    parser.add_argument("-o",
                        "--output-dir",
                        default=default_output_dir)
    parser.add_argument("-n",
                        "--num-episodes",
                        type=int,
                        default="1")
    parser.add_argument("-b",
                        "--batch-size",
                        type=int,
                        default="128")
    parser.add_argument("-d",
                        "--database",
                        type=str)
    return parser


def main():
    global LOGGER
    tf.logging.set_verbosity(tf.logging.ERROR)
    args = cli().parse_args(sys.argv[1:])
    os.makedirs(args.output_dir, exist_ok=True)
    LOGGER = init_logger("{}/session.log".format(args.output_dir))
    print("")
    LOGGER.info(f"New training session of {args.num_episodes} episode(s).")
    tf.set_random_seed(SEED)
    np.random.seed(SEED)
    rnd.seed(SEED)
    LOGGER.debug(f"The random seed is set to {SEED}.")
    tf.reset_default_graph()
    Agent(args)
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
