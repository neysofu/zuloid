def test_id_name(uci_engine):
    assert uci_engine.id["name"] == "Z64C"
    uci_engine.quit()


def test_option_hash(uci_engine):
    assert "Hash" in uci_engine.options
    assert uci_engine.options["Hash"].type == "spin"
    assert uci_engine.options["Hash"].max >= 131072
    uci_engine.quit()
