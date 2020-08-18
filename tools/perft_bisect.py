#!/usr/bin/env python3

from subprocess import Popen, PIPE, STDOUT
import sys


def movelist2dict(moves):
    result = {}
    for move in moves:
        result[move.split(':')[0]] = int(move.split(':')[1])
    return result


def diff(list1, list2):
    moves1 = movelist2dict(list1)
    moves2 = movelist2dict(list2)
    for (move, result) in moves1.items():
        if move not in moves2:
            print('+ {}: {}'.format(move, result))
        elif moves2[move] != result:
            print('~ {}: {} vs {}'.format(move, result, moves2[move]))
        del moves2[move]
    for (move, result) in moves2.items():
        if move not in moves2:
            print('- {}: {}'.format(move, result))


def stockfish_moves(fen, depth):
    stockfish = Popen(['stockfish'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = stockfish.communicate(
        input='uci\nposition fen {}\ngo perft {}\nquit'.format(fen, depth).encode('utf-8'))[0]
    output = stdout_data.decode('utf-8').split('\n')
    output = output[output.index('uciok') + 1:]
    output = output[:-4]
    return output


def zuloid_moves(executable, fen, depth):
    zuloid = Popen([executable], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    stdout_data = zuloid.communicate(
        input='uci\nposition fen {}\ngo perft {}\nquit'.format(fen, depth).encode('utf-8'))[0]
    output = stdout_data.decode('utf-8').split('\n')
    output = output[output.index('uciok') + 1:]
    output = output[:-4]
    return output


def main(engine1, engine2, fen, depth):
    expected = stockfish_moves(fen, depth)
    actual = zuloid_moves(engine1, fen, depth)
    diff(expected, actual)


if __name__ == '__main__':
    engine1 = sys.argv[1]
    engine2 = sys.argv[2]
    depth = sys.argv[3]
    fen = ' '.join(sys.argv[4:])
    main(engine1, engine2, fen, depth)
