#include <stdio.h>

/* Logs are always prefixed by '#', regardless of the protocol in use. This is
 * in line with both UGEI and CECP guidelines. UCI doesn't have a specific
 * syntax for debug statements, but simply ignores any unknown command (as # is). */
#ifdef NDEBUG
#define LOGF(...)
#else
#define LOGF(...)                                                                          \
	do {                                                                                   \
		/* Makes the path relative to the project root.                                    \
		 *                                ~~~~~~~~~~~~~~~~~~~~ */                          \
		printf("# %s:%s:%d -- ", __FILE__ + PROJECT_DIR_LENGTH, __func__, __LINE__);       \
		printf(__VA_ARGS__);                                                               \
		putchar('\n');                                                                     \
	} while (0)
#endif
