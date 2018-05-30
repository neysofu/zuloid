import unittest
import chess
import chess.uci

class UciTest(unittest.TestCase):

    def test_on_and_off(self):
        engine = chess.uci.popen_engine("/usr/bin/stockfish")
        engine.uci()
        self.assertEqual(engine.quit(), 0)

if __name__ == "__main__":
    unittest.main()
