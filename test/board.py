import os.path
import chess.pgn

def main():
    here = os.path.join(os.getcwd())
    with open(os.path.join(here, "KingBase.pgn"), encoding="utf-8") as kingbase:
        with open(os.path.join(here, "KingBase.ucimoves"), "w+") as kingbase_as_uci_moves:
            while True:
                game = chess.pgn.read_game(kingbase)
                if not game:
                    break;
                kingbase_as_uci_moves.write(chess.STARTING_FEN + "\n")
                for move in game.main_line():
                    kingbase_as_uci_moves.write(
                        "{} {} {} {} {}",
                        move.uci(),
                        move
                        + "\n")
                kingbase_as_uci_moves.write("\n")

if __name__ == "__main__":
    main()
