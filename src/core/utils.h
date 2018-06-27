#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

bool
atob(char *str);

bool
is_stdin_empty(void);

uint32_t
hash_djb2(char *str);
