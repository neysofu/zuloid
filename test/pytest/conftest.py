import pytest
import chess
import chess.engine


def pytest_addoption(parser):
    parser.addoption("--path-to-engine", action="store",
                     help="Path to Z64C engine binary")


def pytest_generate_tests(metafunc):
    path_to_engine = metafunc.config.option.path_to_engine
    if 'uci_engine' in metafunc.fixturenames:
        engine = chess.engine.SimpleEngine.popen_uci(path_to_engine)
        engine.
        metafunc.parametrize("uci_engine", [engine])
