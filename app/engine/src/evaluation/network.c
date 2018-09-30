#include "switches.h"

#if SWITCH_CLUSTER == SWITCH_BACKEND_CPU
#include "network_cpu.c"
#elif SWITCH_BACKEND == SWITCH_BACKEND_AF
#include "network_af.c"
#endif
