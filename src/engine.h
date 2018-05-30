struct Engine;

struct Engine *
engine_init();

void
engine_establish_connection(struct Engine *engine);

int
engine_wait(struct Engine *engine);

void
uci_identify();

void
uci_ok();

void
uci_ready_ok();

void
uci_new_game();

void
uci_go();

void
uci_stop();

void
uci_best_move();
