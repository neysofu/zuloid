#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import chess
import chess.pgn
import numpy as np
import scipy
import tensorflow as tf
import keras as ks
import sqlite3
import random as rnd
import logging
import sys
import argparse
import uuid
import os
from os.path import expanduser
# For A3C (TODO)
from multiprocessing import Pool

SEED = 42

HOT_BOARD_TENSOR_WIDTH = 456
BOARD_TENSOR_WIDTH = 512
MOVE_TENSOR_WIDTH = 7

AVERAGE_NUM_OF_MOVES_IN_CHESS_GAME = 42
MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW = 50

RATIO_OF_CHESS_960_GAMES_DURING_TRAINING = 0.4
EXPLORATION_RATE_DURING_TRAINING = 0.08
DISCOUNT_VALUE = 0.96

LOGGING_FORMAT = "%(asctime)s @ '%(name)s.%(funcName)s:%(lineno)d' [%(levelname)s]: %(message)s"
LOGGING_DATE_FORMAT = "%Y-%m-%dT%H:%M:%S%z"

BOARD_TO_MOVE_FILENAME = "Board2move.h5"
BOARD_INCR_TO_BOARD_FILENAME = "BoardIncr2Board.h5"
HOT_TO_BOARD_FILENAME = "Hot2board.h5"

__version__ = "0.1"


class Db:

    INIT_TABLES_CMDS = [
        """
        CREATE TABLE IF NOT EXISTS games (
            SessionId VARCHAR,
            Date TIMESTAMP,
            Episode INT,
            Moves VARCHAR,
            Result CHAR(7),
            Ending VARCHAR)
        """,
        """
        CREATE TABLE IF NOT EXISTS positions (
            Fen CHAR(88),
            Event BIGINT)
        """,
        """
        CREATE TABLE IF NOT EXISTS sessions (
            SessionId VARCHAR,
            StartTime DATETIME,
            EndTIme DATETIME,
            Version VARCHAR)
        """]

    def __init__(self):
        self.path = "{}/db.sqlite3".format(ARGS.output_dir)
        self.connection = sqlite3.connect(self.path)
        for cmd in Db.INIT_TABLES_CMDS:
            self.connection.cursor().execute(cmd)
        self.connection.commit()

    def insert_game(self, game, num_episode):
        self.connection.cursor().execute("""
            INSERT INTO games (
                SessionId,
                Date,
                Episode,
                Moves,
                Result
            ) VALUES (
                '{}',
                '{}',
                {},
                '{}',
                '{}'
            )""".format(SESSION_ID,
                        "Now",
                        1337,
                        game.accept(
                            chess.pgn.StringExporter(
                                headers=False,
                                variations=False,
                                comments=False)),
                        numeric_result(game.end().board())))
        self.connection.commit()

    def insert_session(self, session_id):
        self.connection.cursor().execute("""
        INSERT INTO sessions (
            SessionId,
            Version
        ) VALUES ("{}", "{}")""".format(session_id, __version__))
        self.connection.commit()


class Board(chess.Board):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def to_tensor(self, resoluteness_factor):
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
            self.has_queenside_castling_rights(
                chess.BLACK), resoluteness_factor,
            min(self.halfmove_clock, MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW) /
            MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW, self.fullmove_number /
            (self.fullmove_number + AVERAGE_NUM_OF_MOVES_IN_CHESS_GAME)
        ]])

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

class Action:

    def __init__(self, tensor):
        self.tensor = tensor
        self.source_file = int(tensor[0][0] * 8)
        self.source_rank = int(tensor[0][1] * 8)
        self.target_file = int(tensor[0][2] * 8)
        self.target_rank = int(tensor[0][3] * 8)
        self.source = chess.square(self.source_file, self.source_rank)
        self.target = chess.square(self.target_file, self.target_rank)
        self.move = chess.Move(self.source, self.target)

    def __sub__(self, move_2):
        source_file_1 = chess.square_file(self.move.from_square)
        source_rank_1 = chess.square_file(self.move.to_square)
        target_file_1 = chess.square_rank(self.move.from_square)
        target_rank_1 = chess.square_rank(self.move.to_square)
        source_file_2 = chess.square_file(move_2.from_square)
        source_rank_2 = chess.square_file(move_2.to_square)
        target_file_2 = chess.square_rank(move_2.from_square)
        target_rank_2 = chess.square_rank(move_2.to_square)
        return scipy.spatial.distance.cosine(
            [source_file_1, source_rank_1, target_file_1, target_file_1],
            [source_file_2, source_rank_2, target_file_2, target_file_2])

    def fit_to(self, board):
        if self.move in board.legal_moves:
            return self.move
        max_moves_similarity = 0
        index_of_candidate_move = 0
        i = 0
        for legal_move in board.legal_moves:
            moves_similarity = self - legal_move
            if moves_similarity > max_moves_similarity:
                max_moves_similarity = moves_similarity
                index_of_candidate_move = i
            i += 1
        return list(board.legal_moves)[index_of_candidate_move]


class Agent:

    # Hyperparameters. Boy, isn't ML fun...
    LEARNING_RATE = 0.0004
    EXPLORATION_RATE = 0.85
    DISCOUNT_VALUE = 1.0
    BATCH_SIZE = 512
    # All hyperparameter values decay over time to guarantee convergence.
    DECAY_RATE = 0.99998

    def __init__(self, board):
        self.model = Agent.playing_model()
        #self.read_from_disk()
        self.board = Board()
        self.batch = []
        self.learning_rate = Agent.LEARNING_RATE
        self.exploration_rate = Agent.EXPLORATION_RATE
        self.discount_value = Agent.DISCOUNT_VALUE
        # TODO: hyperparameter values decay.
        num_episodes = int(ARGS.num_episodes)
        self.i = 0
        while self.i < num_episodes:
            self.play()
            self.i += 1
            if self.i % 500 == 0:
                LOGGER.info("Num. of episodes is {}".format(self.i))
        self.write_to_disk()

    def play(self):
        resoluteness_factor = np.random.random()
        LOGGER.info(
            "New game (game ID is '{}'). Starting position: \"{}\"".format(
                SESSION_ID, self.board.fen()))
        i = 0
        while not self.board.is_game_over():
            board_tensor = self.board.to_tensor(resoluteness_factor)
            move = self.move(board_tensor)
            self.board.push(move)
            LOGGER.info(f"{i}. {move.uci()} ({self.model.predict(board_tensor)[0][4]})")
            i += 1
        DB.insert_game(chess.pgn.Game.from_board(self.board), self.i)
        LOGGER.info("Game ended. Result: {}. Num. of moves: {}".format(
            self.board.result(), self.board.fullmove_number))
        self.backpropagate_state_value()
        self.board = Board()

    def move(self, board_tensor):
        if np.random.random() < Agent.EXPLORATION_RATE:
            return rnd.choice(list(self.board.legal_moves))
        else:
            return Action(self.model.predict(board_tensor)).fit_to(self.board)

    def playing_model():
        model = ks.models.Sequential()
        model.add(
            ks.layers.Dense(
                HOT_BOARD_TENSOR_WIDTH,
                activation='relu',
                input_dim=HOT_BOARD_TENSOR_WIDTH))
        model.add(ks.layers.Dropout(0.5))
        model.add(ks.layers.Dense(64, activation='relu'))
        model.add(ks.layers.Dropout(0.5))
        model.add(ks.layers.Dense(MOVE_TENSOR_WIDTH, activation='sigmoid'))
        model.compile(
            loss='categorical_crossentropy',
            optimizer="sgd",
            metrics=['accuracy'])
        return model

    def backpropagate_state_value(self):
        game = chess.pgn.Game.from_board(self.board)
        state_value = numeric_result(self.board)
        stack_of_boards = [game.board()]
        for move in game.main_line():
            stack_of_boards.append(stack_of_boards[-1])
            stack_of_boards[-1].push(move)
        stack_of_boards = [Board(fen=board.fen()) for board in stack_of_boards][::-1]
        for board in stack_of_boards:
            board_tensor = self.board.to_tensor(0.1337)
            target_tensor = self.model.predict(board_tensor)
            target_tensor[0][4] += 0.01 * (state_value - target_tensor[0][4])
            state_value = target_tensor[0][4]
            self.model.fit(board_tensor, target_tensor)

    def write_to_disk(self):
        self.model.save(ARGS.output_dir + "/model.h5")

    def read_from_disk(self):
        self.model.load_weights(ARGS.output_dir + "/model.h5")

    def remember(self):
        # FIXME: experience replay is still a long way to go before being
        # usable.
        self.batch.append((board, move))

    def setup_board(self):
        chess960 = rnd.uniform(0, 1) < RATIO_OF_CHESS_960_GAMES_DURING_TRAINING
        self.board = Board(chess960=chess960)

def cli():
    parser = argparse.ArgumentParser()
    default_output_dir = expanduser("~") + "/Z64C-training-target/"
    parser.add_argument("-o", "--output-dir", default=default_output_dir)
    parser.add_argument("-n", "--num-episodes", default="1024")
    return parser


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


def main():
    global ARGS
    global DB
    global LOGGER
    global SESSION_ID
    tf.logging.set_verbosity(tf.logging.ERROR)
    ARGS = cli().parse_args(sys.argv[1:])
    os.makedirs(ARGS.output_dir, exist_ok=True)
    DB = Db()
    LOGGER = init_logger("{}/session.log".format(ARGS.output_dir))
    SESSION_ID = uuid.uuid1()
    print("")
    LOGGER.info("Starting a new Z64C training session with ID '{}'.".format(
        SESSION_ID))
    DB.insert_session(SESSION_ID)
    tf.set_random_seed(SEED)
    np.random.seed(SEED)
    rnd.seed(SEED)
    tf.reset_default_graph()
    Agent(Board())
    LOGGER.info(
        "Exiting the Z64C training session with ID '{}'.".format(SESSION_ID))


def exit_gracefully():
    # TODO
    pass


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        LOGGER.info("Keyboard interrupt received. Exiting gracefully. Press"
                    "CTRL+C a second time to exit immediately.")
        exit_gracefully()
