import pytest
import chess
import chess.engine


def pytest_addoption(parser):
    parser.addoption("--path-to-Z64C", action="store",
                     help="Path to Z64C engine binary")
    parser.addoption("--path-to-SF", action="store",
                     help="Path to Stockfish binary")


@pytest.fixture
def uci_engine(request):
    path_to_engine = request.config.getoption("--path-to-Z64C")
    engine = chess.engine.SimpleEngine.popen_uci(path_to_engine)

    def quit_engine():
        try:
            engine.quit()
        except chess.engine.EngineTerminatedError:
            pass

    request.addfinalizer(quit_engine)
    return engine
