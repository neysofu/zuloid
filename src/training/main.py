#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import chess
import numpy as np
import scipy
#import tensorflow as tf
import keras as ks
import sqlite3
import random as rnd
import logging
import sys
import argparse
import uuid
import os
from os.path import expanduser

SEED = 42

HOT_BOARD_TENSOR_WIDTH = 456
BOARD_TENSOR_WIDTH = 512
MOVE_TENSOR_WIDTH = 4

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

    INIT_TABLES_CMDS = """
        CREATE TABLE IF NOT EXISTS games (
            Event VARCHAR,
            Site VARCHAR,
            Date TIMESTAMP,
            Iteration INT,
            NumMoves INT,
            WhiteId VARCHAR,
            BlackId VARCHAR,
            Result TINYINT
        );
        CREATE TABLE IF NOT EXISTS positions (
            Fen CHAR(88),
            Event BIGINT
        );
        CREATE TABLE IF NOT EXISTS sessions (
            SessionId VARCHAR,
            StartTime DATETIME,
            EndTIme DATETIME,
            Version VARCHAR
        )
    """

    def __init__(self):
        self.path = "{}/db.sqlite3".format(ARGS.output_dir)
        self.connection = sqlite3.connect(self.path)

    def insert_game(db, game):
        self.connection.cursor().execute("""
            INSERT INTO games (
                Event,
                Site,
                Date,
                Round,
                White,
                Black,
                Result
            ) VALUES (
                {},
                {},
                {},
                {},
                {}
            )""".format(
            game.headers["Event"], game.headers["Site"],
            game.headers["Date"], game.headers["Round"],
            game.headers["White"], game.headers["Black"],
            game.headers["Result"], game.headers["Ending"]))

    def insert_session(self, session_id):
        self.connection.cursor().execute("""
        INSERT INTO sessions (
            SessionId,
            Version
        ) VALUES ("{}", "{}")""".format(
            session_id,
            __version__))

class Board(chess.Board):

    def __init__(self):
        is_960 = rnd.uniform(0, 1) < RATIO_OF_CHESS_960_GAMES_DURING_TRAINING
        super().__init__(chess960=is_960)

    def to_tensor(self, resoluteness_factor):
        return np.array([
            [bit for bitmask in [piece_to_tensor(self.piece_at(square))
             for square in chess.SQUARES] for bit in bitmask] +
            [self.turn,
             self.has_kingside_castling_rights(chess.WHITE),
             self.has_kingside_castling_rights(chess.BLACK),
             self.has_queenside_castling_rights(chess.WHITE),
             self.has_queenside_castling_rights(chess.BLACK),
             resoluteness_factor,
             min(self.halfmove_clock, MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW) / MAX_NUM_OF_REVERSIBLE_MOVES_BEFORE_DRAW,
             self.fullmove_number / (self.fullmove_number + AVERAGE_NUM_OF_MOVES_IN_CHESS_GAME)]])

def piece_to_tensor(piece):
    if not piece:
        return np.zeros(7)
    return [
        piece.piece_type == chess.PAWN,
        piece.piece_type == chess.KNIGHT,
        piece.piece_type in [chess.BISHOP, chess.QUEEN],
        piece.piece_type in [chess.ROOK, chess.QUEEN],
        piece.piece_type == chess.KING,
        piece.color,
        not piece.color]

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

class Engine:

    EXPLORATION_RATE = 0.4

    def __init__(self, board):
        self.model = Engine.playing_model()
        self.board = Board()
        self.experience_batch_length = 64
        num_iterations = int(ARGS.num_iterations)
        while num_iterations > 0:
            self.train()
            num_iterations -= 1
        self.write_to_disk()

    def train(self):
        # TODO: study a better distribution.
        resoluteness_factor = np.random.random()
        LOGGER.info("New game (game ID is '{}'). Starting position: \"{}\"".format(SESSION_ID, self.board.fen()))
        while not self.board.is_game_over():
            board_tensor = self.board.to_tensor(resoluteness_factor)
            move = self.move(board_tensor)
            self.board.push(move)
        LOGGER.info("Game ended. Result: {}. Num. of moves: {}".format(
            self.board.result(),
            self.board.fullmove_number))
        #self.model.train()
        self.board = Board()

    def move(self, board_tensor):
        if np.random.random() < Engine.EXPLORATION_RATE:
            return rnd.choice(list(self.board.legal_moves))
        else:
            return Action(self.model.predict(board_tensor)).fit_to(self.board)

    def playing_model():
        model = ks.models.Sequential()
        model.add(ks.layers.Dense(HOT_BOARD_TENSOR_WIDTH,
                                  input_dim=HOT_BOARD_TENSOR_WIDTH,
                                  activation="relu"))
        model.add(ks.layers.Dense(HOT_BOARD_TENSOR_WIDTH,
                                  activation="relu"))
        model.add(ks.layers.Dense(HOT_BOARD_TENSOR_WIDTH,
                                  activation="relu"))
        model.add(ks.layers.Dense(5,
                                  activation="softmax"))
        model.compile(loss="categorical_crossentropy",
                      optimizer=ks.optimizers.Adam(),
                      )
        return model

    def read_from_disk(self):
        self.model.load_weights(ARGS.output_dir + "/model.h5")

    def fit(self, board_tensor, move_tensor):
        self.experience_batch[np.random.rndint(0, self.experience_batch_length)] = (board_tensor, move_tensor)
        self.model.fit(self.experience_batch)

    def write_to_disk(self):
        self.model.save(ARGS.output_dir + "/model.h5")

def cli():
    parser = argparse.ArgumentParser()
    default_output_dir = expanduser("~") + "/Z64C-training-target/"
    parser.add_argument("-o",
                        "--output-dir",
                        default=default_output_dir)
    parser.add_argument("-n",
                        "--num-iterations",
                        default="1024")
    # TODO (?)
    parser.add_argument("-s",
                        "--source-data",
                        default="rl")
    return parser

def init_logger(log_file_path):
    logger = logging.getLogger()
    logging.basicConfig(
        level=logging.DEBUG,
        format=LOGGING_FORMAT,
        datefmt=LOGGING_DATE_FORMAT,
        stream=sys.stdout,
        filemode="w")
    formatter = logging.Formatter(fmt=LOGGING_FORMAT,
                                  datefmt=LOGGING_DATE_FORMAT)
    file_handler = logging.FileHandler(log_file_path)
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)
    return logger

def set_numpy_seed():
    np.random.seed(SEED)
    LOGGER.info("Using {} as the NumPy seed.".format(SEED))

def main():
    global ARGS
    global DB
    global LOGGER
    global SESSION_ID
    #tf.logging.set_verbosity(tf.logging.ERROR)
    ARGS = cli().parse_args(sys.argv[1:])
    os.makedirs(ARGS.output_dir, exist_ok=True)
    DB = Db()
    LOGGER = init_logger("{}/session.log".format(ARGS.output_dir))
    SESSION_ID = uuid.uuid1()
    LOGGER.info("Starting a new Z64C training session with ID '{}'.".format(SESSION_ID))
    DB.insert_session(SESSION_ID)
    set_numpy_seed()
    print("")
    Engine(Board())
    LOGGER.info("Exiting the Z64C training session with ID '{}'.".format(SESSION_ID))

if __name__ == "__main__":
    main()
