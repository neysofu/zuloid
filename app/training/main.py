#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import chess
import chess.pgn
import numpy as np
import tensorflow as tf
import scipy
import random as rnd
import unittest as ut
import logging
import sys
import argparse
import os
import gym
from chess import pgn
from os.path import expanduser
from multiprocessing import Pool # For A3C (TODO)
from scipy import spatial


__author__ = "Filippo Costa @neysofu"
__version__ = "0.4"

SEED = 146

LOGGING_FORMAT = "%(asctime)s @ '%(name)s.%(funcName)s:%(lineno)d' [%(levelname)s]: %(message)s"
LOGGING_DATE_FORMAT = "%Y-%m-%dT%H:%M:%S%z"


class Piece(chess.Piece):

    def value(self):
        piece_type = self.piece_type()
        if piece_type == chess.PAWN:
            return 0.10060362173
        if piece_type == chess.KNIGHT:
            return 0.30684104627
        if piece_type == chess.BISHOP:
            return 0.33199195171
        if piece_type == chess.ROOK:
            return 0.55130784708
        if piece_type == chess.QUEEN:
            return 1
        return 0


class Board(chess.Board):

    HOT_ENCODING_LEN = 454

    AVERAGE_NUM_OF_MOVES_IN_GAME = 54
    MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW = 50

    def random():
        if rnd.randint(0, 1) == 0:
            return Board.from_chess960_pos(rnd.randint(0, 959))
        else:
            return Board()

    def numeric_result(self):
        result = self.result()
        if result == "1-0":
            return 1
        elif result == "1/2-1/2":
            return 0.5
        elif result == "0-1":
            return 0
        raise ValueError("The result of the chess game doesn't yet exist"
                         " (it is {}).".format(result))

    def material_imbalance(self):
        return 0 # TODO


class Prediction(np.ndarray):

    STATE_VALUE_INDEX = 0
    STATE_VALUE_ALPHA_INDEX = 1
    STATE_VALUE_BETA_INDEX = 2

    MOVE_START_INDEX = 3

    MOVE_PAWN_START_INDEX = MOVE_START_INDEX
    MOVE_PAWN_LEN = 4
    MOVE_PAWN_NUM = 8

    MOVE_KNIGHT_START_INDEX = (MOVE_PAWN_START_INDEX +
                               MOVE_PAWN_LEN * MOVE_PAWN_NUM)
    MOVE_KNIGHT_LEN = 8
    MOVE_KNIGHT_NUM = 2

    MOVE_BISHOP_START_INDEX = (MOVE_KNIGHT_START_INDEX +
                               MOVE_KNIGHT_LEN * MOVE_KNIGHT_NUM)
    MOVE_BISHOP_LEN = 16
    MOVE_BISHOP_NUM = 3

    MOVE_ROOK_START_INDEX = (MOVE_BISHOP_START_INDEX +
                             MOVE_BISHOP_LEN * MOVE_BISHOP_NUM)
    MOVE_ROOK_LEN = 16
    MOVE_ROOK_NUM = 3

    MOVE_KING_START_INDEX = (MOVE_ROOK_START_INDEX +
                             MOVE_ROOK_LEN * MOVE_ROOK_NUM)
    MOVE_KING_LEN = 10

    MOVE_PROMOTION_START_INDEX = (MOVE_KING_START_INDEX + MOVE_KING_LEN)
    MOVE_PROMOTION_LEN = 4
    MOVE_PROMOTION_NUM = 8

    TENSOR_LEN = (MOVE_PROMOTION_START_INDEX +
                      MOVE_PROMOTION_LEN * MOVE_PROMOTION_NUM)

    def moves(self, board):
        """
        A probabilistic action selection strategy with exploration.
        """
        legal_moves = list(board.legal_moves)
        piece_map = self.piece_map()
        piece_map_by_piece_type = {}
        best_moves_by_index = np.argsort(self[0][MOVE_START_INDEX])
        for i in best_moves_by_index:
            if i >= MOVE_PAWN_START_INDEX:
                offset = i % 4
                movement = {0: (0, 1),
                       1: (0, 2),
                       2: (-1, 1),
                       3: (1, 1)}[offset]
            if i >= MOVE_KNIGHT_START_INDEX:
                piece_type = chess.KNIGHT
                offset = i % MOVE_KNIGHT_LENGTH
                movement = {0: (-2, 1),
                       1: (-2, -1),
                       2: (-1, 2),
                       3: (-1, -2),
                       4: (1, 2),
                       5: (1, -2),
                       6: (2, 1),
                       7: (2, -1)}[offset]
            if i >= MOVE_BISHOP_START_INDEX:
                piece_type = chess.BISHOP
                offset = i % MOVE_BISHOP_LENGTH
                movement = {0: (1, 1),
                            1: (2, 2),
                            2: (3, 3),
                            3: (2, 2),
                            1: (2, 2),
                            1: (2, 2),
                            1: (2, 2),
                            1: (2, 2)}
            if i >= MOVE_ROOK_START_INDEX:
                piece_type = chess.ROOK
                movement = {}[offset] # TODO
            if i >= MOVE_KING_START_INDEX:
                piece_type = chess.KING
                offset = i % MOVE_KING_LENGTH
                movement = {0: (1, 1),
                            1: (1, 0),
                            2: (1, -1),
                            3: (0, 1),
                            4: (0, -1),
                            5: (-1, 1),
                            6: (-1, 0),
                            7: (-1, -1)}[offset]
            move_in_coord = piece_map[piece_type] + movement
            if piece_type not in piece_map_by_piece_type:
                piece_map_by_piece_type[piece_type] = board.pieces(piece_type, board.turn)
            source_file = chess.square_file(piece_map_by_piece_type[piece_type])
            source_rank = chess.square_rank(piece_map_by_piece_type[piece_type])
            target_file = source_file + movement[0]
            target_rank = source_rank + movement[1]
            move = chess.Move(chess.Square(source_file, source_rank),
                              chess.Square(target_file, target_rank))
            if move in legal_moves:
                yield move

    def backpropagate_fixed_state_value(self, game):
        """
        Backpropagate the state values estimated during a game of chess according
        the CTDleaf algorithm.
        See http://www.incompleteideas.net/book/ebook/node75.html for details.
        """
        # TODO: find a custom fitting curve specific to chess. See
        # https://lichess.org/insights/LeelaChess/acpl/material
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
                self.tensors_out[i][0][0] += error * alpha * eligibility_traces[i]
        self.tensors_out[-1][0][0] = result

class Actor:

    def __init__(self):
        self.model = Actor.model()

    def run_episode(self, board):
        transpositions = [board]
        resoluteness_factor = np.random.random()
        tensors_in = []
        predictions = []
        i = 0
        while not board.is_game_over():
            tensor_in = board.to_tensor()
            prediction = self.predict(tensor_in)
            move = prediction.move_generator(board)[0]
            tensors_in.append(tensor_in)
            predictions.append(prediction)
            board.push(move)
            transpositions.append(board)
            i += 1
        LOGGER.debug(f"{i} moves were played. The result is "
                     f"{board.result()}.")
        return Episode(chess.pgn.Game.from_board(board),
                       tensors_in,
                       predictions)

    def predict(self, board):
        pass

    def init_model():
        layer_0 = tf.placeholder(tf.float32, [None, Board.HOT_ENCODING_LEN])
        layer_0_meta = tf.placeholder(tf.float32, [None, 8])
        # TODO: add meta information to layer_0
        layer_1 = tf.layers.dense(inputs=layer_0,
                                  units=1024,
                                  activation=soft_sign_square)
        layer_2 = tf.layers.dense(inputs=layer_1,
                                  units=1024,
                                  activation=soft_sign_square)
        layer_3 = tf.layers.dropout(inputs=layer_2,
                                    rate=0.5)
        layer_4 = tf.layers.dense(inputs=layer_3,
                                  units=1024,
                                  activation=soft_sign_square)
        layer_5 = tf.layers.dropout(inputs=layer_4,
                                    rate=0.5)
        layer_6 = tf.layers.dense(inputs=layer_5,
                                  units=1024,
                                  activation=soft_sign_square)
        layer_7 = tf.layers.dropout(inputs=layer_6,
                                    rate=0.5)
        layer_8 = tf.layers.dense(inputs=layer_7,
                                  units=Prediction.TENSOR_LEN)
        layer_9 = tf.nn.softmax(layer_8)
        loss = tf.losses.sparse_softmax_cross_entropy(labels=layer_9,
                                                      logits=layer_9)
        optimizer = tf.train.AdamOptimizer()
        train_op = optimizer.minimize(loss=loss,
                           global_step=tf.train.get_global_step())
        return tf.estimator.EstimatorSpec(mode=tf.estimator.ModeKeys.TRAIN,
                                          loss=loss,
                                          train_op=train_op)

    def read_from_disk_if_exists(self):
        try:
            tf.train.import_meta_graph("Z64C.meta").restore()
            self.model.load_weights(self.args.output_dir + "/model.h5")
            LOGGER.info("Successfully loaded weights from the last session.")
        except:
            LOGGER.error("The weights of the model couldn't be found. New "
                         "weights will be initialized.")

    def write_to_disk(self):
        tf.train.Saver(saver.save(sess, "Z64C"))

    def activation():
        # TODO: use bit shifts.
        return tf.divide(tf.multiply(x, 2),
                         tf.add(tf.square(x), 1))

class Agent:

    def __init__(self, num_episodes, output_dir, num_actors=4, ):
        LOGGER.info(f"New training session of {args.num_episodes} episode(s).")
        self.num_actors = num_actors
        self.actors = []
        for i in range(self.num_actors):
            self.actors[i] = Actor()
        self.batch = []
        self.num_episodes = num_episodes

    def train(self):
        i = 0
        while i < self.num_episodes:
            episode = self.actors[0].run_episode()
            self.queue_for_training(episode)
            i += 1
            if i % 100 == 0:
                self.write_to_disk()

    def queue_for_training(self, episode):
        """
        Train the model based on a game played by the AI. Its inputs along with
        the relative outputs are included to avoid duplicate computation.
        """
        episode.backpropagate_result()
        self.batch.append(episode)
        if len(self.batch) >= Agent.BATCH_SIZE:
            self.replay_experience()

    def replay_experience(self):
        rnd.shuffle(self.batch)
        for episode in self.batch:
            for i, t_in in enumerate(episode.tensors_in):
                self.model.fit(t_in, episode.tensors_out[i])
        self.batch = []


class Episode:
    """
    An episode is a single learning iteration (a chess game). Operations running
    on the data collected during an episode can harvest interesting data that is
    then fed into the main training routine.
    """

    def __init__(self, game, tensors_in, tensors_out, state_values):
        self.game = game
        self.tensors_in = tensors_in
        self.tensors_out = tensors_out
        self.state_values = state_values

    def backpropagate_result(self):


def logger(log_file_path):
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
    parser.add_argument("-d",
                        "--database",
                        type=str)
    subparsers = parser.add_subparsers(dest="subcmd")
    subparser_test = subparsers.add_parser("train")
    parser.add_argument("-n",
                        "--num-episodes",
                        type=int,
                        default="128")
    parser.add_argument("-b",
                        "--batch-size",
                        type=int,
                        default="128")
    parser.add_argument("-t", "--time-limit", type=str)
    subparser_test = subparsers.add_parser("test")
    subparser_test.add_argument("test_args", nargs="*")
    subparser_play = subparsers.add_parser("play")
    subparser_play.add_argument("-f",
                                "--fen",
                                type=str,
                                default="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    subparser_play.add_argument("-c",
                                "--color",
                                type=str,
                                default="w")
    return parser

def set_random_seed():
    tf.set_random_seed(SEED)
    np.random.seed(SEED)
    rnd.seed(SEED)
    LOGGER.debug(f"The random seed is set to {SEED}.")

def main():
    global ARGS
    global LOGGER
    ARGS = cli().parse_args(sys.argv[1:])
    LOGGER = logger("{}/session.log".format(ARGS.output_dir))
    os.makedirs(ARGS.output_dir, exist_ok=True)
    tf.logging.set_verbosity(tf.logging.ERROR)
    LOGGER.debug(f"Using Tensorflow {tf.__version__}.")
    set_random_seed()
    if ARGS.subcmd == "train":
        Agent.train()
    tf.reset_default_graph()
    LOGGER.info("Goodbye.")

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        LOGGER.info("Keyboard interrupt received. Exiting gracefully. Press "
                    "CTRL+C a second time to exit immediately and disregard any"
                    " changes.")
