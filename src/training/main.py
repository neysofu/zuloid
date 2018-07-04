#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import chess
import numpy as np
import tensorflow as tf
import keras as ks
import random as rnd
import logging
import sys
import argparse

BOARD_AS_ONE_HOT_ENCODING_WIDTH = 424
BOARD_AS_VECTOR_WIDTH = 1024
MOVE_AS_VECTOR_WIDTH = 4

def create_logger(log_file_path):
    logger = logging.getLogger()
    formatter = logging.Formatter("standard")
    logging.basicConfig(
        level=logging.DEBUG,
        format="%(asctime)s @ '%(name)s.%(funcName)s:%(lineno)d' [%(levelname)s]: %(message)s",
        datefmt="%Y-%m-%dT%H:%M:%S%z",
        stream=sys.stdout,
        filemode="w")
    # Write to file
    file_handler = logging.FileHandler(log_file_path)
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)
    return logger

# Board2vec
# =========

def piece_to_one_hot_encoding(piece):
    if not piece:
        return [0, 0, 0, 0, 0, 0]
    return [
        piece.piece_type == chess.PAWN,
        piece.piece_type == chess.KNIGHT,
        piece.piece_type in [chess.BISHOP, chess.QUEEN],
        piece.piece_type in [chess.ROOK, chess.QUEEN],
        piece.piece_type == chess.KING,
        piece.color,
        not piece.color]

def board_to_one_hot_encoding(board, resoluteness_factor):
    return np.array([
        [bit for bitmask in [piece_to_one_hot_encoding(board.piece_at(square))
         for square in chess.SQUARES] for bit in bitmask] +
        [board.turn,
         board.has_kingside_castling_rights(chess.WHITE),
         board.has_kingside_castling_rights(chess.BLACK),
         board.has_queenside_castling_rights(chess.WHITE),
         board.has_queenside_castling_rights(chess.BLACK),
         resoluteness_factor,
         min(board.halfmove_clock, 50) / 50,
         board.fullmove_number / (board.fullmove_number + 40)]])

def board_to_vec_model():
    model = ks.models.Sequential()
    model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                              input_dim=BOARD_AS_ONE_HOT_ENCODING_WIDTH,
                              activation="linear"))
    model.add(ks.layers.LeakyReLU(alpha=0.2))
    model.add(ks.layers.normalization.BatchNormalization())
    model.add(ks.layers.Dropout(0.1))
    for i in range(3):
        model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                                  activation="linear"))
        model.add(ks.layers.LeakyReLU(alpha=0.2))
        model.add(ks.layers.normalization.BatchNormalization())
        model.add(ks.layers.Dropout(0.1))
    model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                              activation="linear"))
    model.compile(loss="categorical_crossentropy",
                  optimizer=ks.optimizers.Adamax(),
                  metrics=["accuracy"])
    return model

# BoardIncr2vec
# =============

def board_incr_to_vec_model():
    model = ks.models.Sequential()
    model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                              input_dim=BOARD_AS_VECTOR_WIDTH*2+MOVE_AS_VECTOR_WIDTH,
                              activation="linear"))
    model.add(ks.layers.LeakyReLU(alpha=0.2))
    model.add(ks.layers.normalization.BatchNormalization())
    model.add(ks.layers.Dropout(0.1))
    for i in range(2):
        model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                                  activation="linear"))
        model.add(ks.layers.LeakyReLU(alpha=0.2))
        model.add(ks.layers.normalization.BatchNormalization())
        model.add(ks.layers.Dropout(0.1))
    model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                              activation="linear"))
    model.compile(loss="categorical_crossentropy",
                  optimizer=ks.optimizers.Adamax(),
                  metrics=["accuracy"])
    return model

# Game2moves
# ==========

def read_move_from_tensor(tensor):
    source_file = int(tensor[0] * 8)
    source_rank = int(tensor[1] * 8)
    target_file = int(tensor[2] * 8)
    target_rank = int(tensor[3] * 8)
    source = chess.square(source_file, source_rank)
    target = chess.square(target_file, target_rank)
    return chess.Move(source, target)

class Game2moves:

    def __init__(self, dir=None):
        if dir:
            self.dir = dir
        self.population_size = 32
        self.match_length = 16
        self.population = [self.model() for x in range(self.population_size)]
        self.population_values = [rnd.uniform(0, 1) for x in
                                   range(self.population_size)]
        self.population_values = np.array(self.population_values) / sum(self.population_values)
        self.model = self.model()
        self.learning_rate = 0.005
        self.rho = 0.9
        self.train()

    def model(self):
        model = ks.models.Sequential()
        model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                                  input_dim=BOARD_AS_VECTOR_WIDTH,
                                  activation="linear"))
        for i in range(3):
            model.add(ks.layers.LeakyReLU(alpha=0.2))
            model.add(ks.layers.normalization.BatchNormalization())
            model.add(ks.layers.Dropout(0.1))
            model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                                      activation="linear"))
        model.add(ks.layers.Dense(BOARD_AS_VECTOR_WIDTH,
                                  activation="softmax"))

        model.compile(loss="categorical_crossentropy",
                      optimizer=ks.optimizers.Adamax(),
                      metrics=["accuracy"])

        return model

    def boards_difference(self, board_1, board_2):
        pieces_1 = board_1.piece_map()
        pieces_2 = board_2.piece_map()

    def reward_individual(self, individual, reward):
        self.population_values[individual] += reward

    def train(self):
        # TODO: choose a S-shaped distribution skewed to the right.
        resoluteness_factor = rnd.uniform(0, 1)
        board = rnd_starting_board()
        players = [None, None]
        while players[0] == players[1]:
            players = np.random.choice(self.population,
                                       2,
                                       p=self.population_values)
        board_as_one_hot_encoding = board_to_one_hot_encoding(board,
                                                              resoluteness_factor)
        board_as_vector = board_to_vec_model().predict(board_as_one_hot_encoding)
        while not board.is_game_over():
            players = players[::-1]
            move = read_move_from_tensor(board_as_vector[0])
            board.push(move)
            LOGGER.debug("Z64C played {}".format(move))
        LOGGER.info("Game ended. Result: ".format(board.result))
        self.finalize_game(players[0], players[1], board.result)

    def finalize_game(self, player_1, player_2, result):
        pass # TODO
        #if result == "1-0":
        #    self.reward_individual(player_white, reward_for_win)
        #    self.train_individual(player_black, positions)
        #elif result == "0-1":
        #    self.reward_individual(player_black, reward_for_win)
        #    self.train_individual(player_black, positions)
        #else:
        #    self.reward_individual(player_white, reward_for_draw)
        #    self.reward_individual(player_black, reward_for_draw)
        #    self.train_individual(player_black, positions)
        #    self.train_individual(player_black, positions)

    def train_individual(self, individual):
        pass

def rnd_starting_board():
    is_960 = rnd.uniform(0, 1) > 0.05
    return chess.Board(chess960=is_960)

def cli():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--log-file-path")
    subcommands = parser.add_subparsers(dest="subcommand")
    board_to_vec = subcommands.add_parser("board_to_vec")
    move_to_vec = subcommands.add_parser("move_to_vec")
    game_to_moves = subcommands.add_parser("game_to_moves")
    return parser

def main():
    print("")
    print("======= Z64C =======")
    global LOGGER
    seed = 42
    np.random.seed(seed)
    LOGGER = create_logger("abc.log")
    LOGGER.debug("Using {} as the NumPy seed.".format(seed))
    tf.logging.set_verbosity(tf.logging.ERROR)
    parser = cli()
    args = parser.parse_args(sys.argv[1:])
    if args.subcommand == "board_to_vec":
        model = board_to_vec_model()
    elif args.subcommand == "move_to_vec":
        model = board_incr_to_vec_model()
    elif args.subcommand == "game_to_moves":
        Game2moves()

if __name__ == "__main__":
    main()
