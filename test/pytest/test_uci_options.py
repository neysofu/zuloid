def test_id_name(uci_engine):
    assert "name" in uci_engine.id
    assert "Zorro" in uci_engine.id["name"]


def test_option_hash(uci_engine):
    assert "hash" in uci_engine.options
    option = uci_engine.options["hash"]
    assert option.type == "spin"
    assert option.min == 0 and option.max >= 131072


def test_option_contempt(uci_engine):
    assert "contempt" in uci_engine.options
    option = uci_engine.options["contempt"]
    assert option.type == "spin"
    assert option.min == -100 and option.max == 100


def test_option_skill_level(uci_engine):
    assert "skill level" in uci_engine.options
    option = uci_engine.options["skill level"]
    assert option.type == "spin"
    assert option.min == 0 and option.max == 20


def test_option_ponder(uci_engine):
    assert "ponder" in uci_engine.options
    option = uci_engine.options["ponder"]
    assert option.type == "check"
    assert option.default is False
