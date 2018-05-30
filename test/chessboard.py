import unittest
import chess
import chess.pgn

class LegalMovesTest(unittest.TestCase):

    def test_from_file(self):
        pgn = open("data/pgn/kasparov-deep-blue-1997.pgn")
        while True:
            game = chess.pgn.read_game(pgn)
            board = CB_INIT
            for move in game.main_line():
                board.push(move)

    def test_game(self, game):
        board = CB_INIT
        for move in game.main_line():
            board.push(move)
