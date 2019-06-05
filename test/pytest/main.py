def test_id_name(uci_engine):
    assert "Z64C" in uci_engine.id["name"]
    uci_engine.quit()


def test_option_hash(uci_engine):
    assert "Hash" in uci_engine.options
    assert uci_engine.options["Hash"].type == "spin"
    assert uci_engine.options["Hash"].max >= 131072
    uci_engine.quit()


def test_option_contempt(uci_engine):
    assert "Contempt" in uci_engine.options
    assert uci_engine.options["Contempt"].type == "spin"
    assert uci_engine.options["Contempt"].min == -100
    assert uci_engine.options["Contempt"].max == +100
    uci_engine.quit()
