import pytest


def test_id_name(uci_engine):
    assert uci_engine.id["name"] == "Z64C"
