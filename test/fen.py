import unittest
import chess

class FenTest(unittest.TestCase):

    def test_positions(self):
        with open("resources/positions.fen") as file:
        positions = file.readlines()
