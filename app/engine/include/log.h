#include "switches.h"

#if SWITCH_DEBUG
#define debug_printf(fmt, args...)                                             \
	fprintf(                                                                     \
	  stderr, "[DEBUG] %s:%s:%d:()" fmt, __FILE__, __func__, __LINE__, ##args);
#else
#define debug_printf(fmt, args...)
#endif
