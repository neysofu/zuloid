#ifndef MT_9727650127843
#define MT_9727650127843

void
init_genrand64(unsigned long long seed);

void
init_by_array64(unsigned long long init_key[], unsigned long long key_length);

/* generates a random number on [0, 2^64-1]-interval */
unsigned long long
genrand64_int64(void);

#endif
