#! /usr/bin/python3

import chess
import random


class Agent:

    def __init__(self):
        self.weights = [[]]

    def eval(self, board: chess.Board):
        inputs = [
            board.castling_rights,
            board.ep_square,
            board.ep_square,
            board.ep_square,
        ]


class Engine:

    def __init__(self, board=chess.Board()):
        self.board = board
        self.best_move = None
        self.ponder_move = None
        pass

    def search(self):
        legal_moves = list(self.board.legal_moves)
        self.best_move = random.choice(legal_moves)
        return self.best_move


if __name__ == "__main__":
    pass
