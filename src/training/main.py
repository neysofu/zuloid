#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import chess
import numpy as np
import tensorflow as tf
import keras as ks
from keras.optimizers import SGD
import random as rnd
import sys

def board_to_one_hot_encoding(board):
    def square_to_one_hot_encoding(square):
        return [
            board.piece_at(square).piece_type == chess.PAWN,
            board.piece_at(square).piece_type == chess.KNIGHT,
            board.piece_at(square).piece_type in [chess.BISHOP, chess.QUEEN],
            board.piece_at(square).piece_type in [chess.ROOK, chess.QUEEN],
            board.piece_at(square).piece_type == chess.KING,
            board.piece_at(square).color,
        ]
    return tf.constant([bit for bit in square_to_one_hot_encoding(square)
                            for square in board.SQUARES])

class Board2vec:

    def __init__(self):
        self.model = self.model()

    def model(self):
        model = ks.models.Sequential()
        model.add(ks.layers.Dense(units=512, activation="relu"))
        model.add(ks.layers.Dense(units=512, activation="relu"))
        model.compile(optimizer=ks.optimizers.SGD(lr=0.04),
                      loss="mean_squred_error")
        return model

    def fit_on_games(self, games):
        pass

    def fit_on_positions(self, positions):
        pass

    def best_individual(self):
        return self.population

class Vec2board:

    def __init__(self):
        self.model = self.model()

    def model(self):
        model = ks.models.Sequential()
        model.add(ks.layers.Dense(units=448))
        model.add(ks.layers.Dense(units=384, activation="hard_sigmoid"))
        model.compile(optimizer=ks.optimizers.SGD(lr=0.4),
                      loss="mean_squred_error")
        return model

    def train(self):
        pass

    def best_individual(self):
        pass

class Game2moves:

    def __init__(self, dir=None):
        if dir:
            self.dir = dir
        self.population_size = 32
        self.match_length = 16
        self.population = [self.model() for x in range(self.population_size)]
        self.population_values = [rnd.uniform(0, 1) for x in
                                   range(self.population_size)]
        self.model = self.model()
        self.learning_rate = 0.005
        self.rho = 0.9
        self.optimizer = self.optimizer()
        self.board_to_vec = Board2vec()
        self.vec_to_board = Vec2board()

    def model(self):
        model = ks.models.Sequential()
        #model.add(ks.layers.GRU(1024,
        #                        input_shape=(None, 1024),
        #                        recurrent_activation="tanh",
        #))
        #model.add(ks.layers.Dense(units=896,
        #                          activation="relu"))
        #model.add(ks.layers.Dense(units=832,
        #                          activation="relu"))
        #model.add(ks.layers.Dense(units=768,
        #                          activation="relu"))
        #model.add(ks.layers.Dense(units=896,
        #                          activation="relu"))
        #model.add(ks.layers.Dense(units=896,
        #                          activation="relu"))
        #model.add(ks.layers.Dense(units=896,
        #                          activation="relu"))
        #model.add(ks.layers.Dense(units=896,
        #                          activation="relu"))
        #model.add(ks.layers.Dense(units=896,
        #                          activation="relu"))
        model.add(ks.layers.Dense(units=896,
                                  activation="relu"))
        model.compile(loss="categorical_crossentropy",
                      optimizer="sgd",
                      metrics=["accuracy"])
        return model

    def model_board_to_vec(self):
        model = ks.models.Sequential()
        model.add(ks.layers.Dense(units=896,
                                  activation="relu"))
        model.compile(loss="categorical_crossentropy",
                      optimizer=self.optimizer,
                      metrics=["accuracy"])
        return model

    def model_vec_to_board(self):
        model = ks.models.Sequential()
        model.add(ks.layers.Dense(units=896,
                                  activation="relu"))
        model.compile(loss="categorical_crossentropy",
                      optimizer=self.optimizer,
                      metrics=["accuracy"])
        return model

    def boards_difference(self, board_1, board_2):
        pieces_1 = board_1.piece_map()
        pieces_2 = board_2.piece_map()


    def optimizer(self):
        return ks.optimizers.Adadelta(rho=0.9)
                        #             rho=self.rho,
                        #             epsilon=None,
                        #             decay=0.0)

    def reward_individual(self, individual, reward):
        self.population[individual] += reward

    def fit(self):
        reward_for_win = 1
        reward_for_draw = rnd.uniform(0, reward_for_win)
        board = chess.Board()
        player_1 = player_white = None
        player_2 = player_black = None
        while player_1 == player_2:
            player_1, player_2 = np.random.choice(self.population,
                                                  2,
                                                  p=self.population_weights)
        while not board.is_game_over():
            board_as_one_hot_encoding = board_to_one_hot_encoding(board)
            board_as_vector = player_1.predict(board_as_vector)
            board = vector_to_board(board_as_vector)
            player_1.fit()
            self.board_to_vec.fit(board_as_one_hot_encoding, board_as_vector)
            self.vec_to_board.fit(board_as_vector, board_as_one_hot_encoding)
            player_1, player_2 = player_2, player_1
        if result == "1-0":
            self.reward_individual(player_white, reward_for_win)
            self.train_individual(player_black, positions)
        elif result == "0-1":
            self.reward_individual(player_black, reward_for_win)
            self.train_individual(player_black, positions)
        else:
            self.reward_individual(player_white, reward_for_draw)
            self.reward_individual(player_black, reward_for_draw)
            self.train_individual(player_black, positions)
            self.train_individual(player_black, positions)

    def train_individual(self, individual):
        pass

def main():
    np.random.seed(24)
    tf.logging.set_verbosity(tf.logging.INFO)
    Game2moves(dir=sys.argv[1])

if __name__ == "__main__":
    main()
