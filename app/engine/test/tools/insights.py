#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Takes a .pgn database as input and, using the chess logic provided by
# python-chess, sorts the game in different files according to certain
# features.

import sys
from pathlib import Path
import os.path
from os import environ
import chess.pgn
import requests
import shutil
import itertools

NUM_DATA_POINTS_PER_TEST = 30

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
        count = 0
        for game in games:
            if count >= NUM_DATA_POINTS_PER_TEST:
                break
            board = game.root().board()
            db.write(board.fen())
            for move in game.main_line():
                for legal_move in board.legal_moves:
                    db.write(" " + move.uci())
                board.push(move)

def all_possible_moves():
    return itertools.product(chess.SQUARE_NAMES, chess.SQUARE_NAMES)

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
    write_games_to_file_by_condition(
        games,
        db_path.with_suffix(".uci.checkmate"),
        lambda board: board.is_checkmate())
    write_games_to_file_by_condition(
        games,
        db_path.with_suffix(".uci.fivefold"),
        lambda board: (board.is_fivefold_repetition() or
                       board.can_claim_threefold_repetition()))
    write_games_to_file_by_condition(
        games,
        db_path.with_suffix(".uci.fifty"),
        lambda board: (board.can_claim_fifty_moves() or
                       board.is_seventyfive_moves()))
    write_games_to_file_by_condition(
        games,
        db_path.with_suffix(".uci.insufficient"),
        lambda board: board.is_insufficient_material())
    write_games_to_file_by_condition(
        games,
        db_path.with_suffix(".uci.stalemate"),
        lambda board: board.is_stalemate())
    write_positions_to_file_with_illegal_moves(
        games,
        db_path.with_suffix(".fen.illegal"))

if __name__ == "__main__":
    main()
