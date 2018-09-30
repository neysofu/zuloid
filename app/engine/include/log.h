#define debug_print(fmt, ...) \
        do { \
			if (Z64C_SWITCH_DEBUG) { \
				fprintf(stderr, \
						"%s:%d:%s(): ", \
						fmt, \
						__FILE__, \
                        __LINE__, \
						__func__, \
						__VA_ARGS__); \
			} while (0)
