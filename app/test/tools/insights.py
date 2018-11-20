#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Takes a .pgn database as input and, using the chess logic provided by
# python-chess, sorts the game in different files according to certain
# features.

import sys
import random
from pathlib import Path
import os.path
from os import environ
import chess.pgn
import requests
import shutil
import itertools

NUM_DATA_POINTS_PER_TEST = 500

def game_to_uci_str(game):
    uci_str = "position fen " + game.root().board().fen()
    for move in game.main_line():
        uci_str += " " + move.uci()
    return uci_str

def write_games_to_file_by_condition(games, db_path, condition):
    count = 0
    filtered_games = []
    for game in games:
        if count >= NUM_DATA_POINTS_PER_TEST:
            break
        if condition(game.end().board()):
            filtered_games.append(game)
            count += 1
    with open(db_path, "w+") as db:
        for game in filtered_games:
            db.write(game_to_uci_str(game) + "\n")

def write_positions_to_file_with_illegal_moves(games, db_path):
    with open(db_path, "w+") as db:
        all_moves = list(itertools.product(chess.SQUARES, repeat=2))
        count = 0
        for game in games:
            if count >= NUM_DATA_POINTS_PER_TEST:
                break
            i = 0
            for move in game.main_line():
                i += 1
            for _ in range(random.randint(0, i)):
                game = game.variations[0]
            board = game.board()
            db.write(" \n")
            db.write(board.fen() + "\n")
            for coords in random.sample(list(all_moves), 1024):
                move = chess.Move(coords[0], coords[1])
                if move not in board.pseudo_legal_moves:
                    db.write(move.uci() + "\n")
            count += 1
def main():
    db_path = Path(sys.argv[1])
    environ["Z64C_TEST_DB_LOCATION"] = str(db_path)
    print("Performing static analysis on the games. This will take a while.")
    with open(db_path, "r") as db:
        games = []
        while True:
            game = chess.pgn.read_game(db)
            if not game:
                break
            if game.errors:
                continue
            games.append(game)
    #write_games_to_file_by_condition(
    #    games,
    #    db_path.with_name("checkmate").with_suffix(".uci"),
    #    lambda board: board.is_checkmate())
    #write_games_to_file_by_condition(
    #    games,
    #    db_path.with_name("threefold").with_suffix(".uci"),
    #    lambda board: (board.is_fivefold_repetition() or
    #                   board.can_claim_threefold_repetition()))
    #write_games_to_file_by_condition(
    #    games,
    #    db_path.with_name("fifty").with_suffix(".uci"),
    #    lambda board: (board.can_claim_fifty_moves() or
    #                   board.is_seventyfive_moves()))
    #write_games_to_file_by_condition(
    #    games,
    #    db_path.with_name("insufficient").with_suffix(".uci"),
    #    lambda board: board.is_insufficient_material())
    #write_games_to_file_by_condition(
    #    games,
    #    db_path.with_name("stalemate").with_suffix(".uci"),
    #    lambda board: board.is_stalemate())
    write_positions_to_file_with_illegal_moves(
        games,
        db_path.with_name("illegal").with_suffix(".fen"))

if __name__ == "__main__":
    main()
