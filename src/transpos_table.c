#include <stdlib.h>
#include "hiredis/hiredis.h"
#include "transpos_table.h"

struct TransposTable *
transpos_table_new(struct TransposTable *transpos_table) {
	if (!transpos_table) {
		transpos_table = malloc(sizeof(struct TransposTable));
		if (!transpos_table) {
			return NULL;
		}
	}
	//transpos_table->redis = redisAsyncConnect("127.0.0.1", 6379);
	return transpos_table;
}

/*
struct TranspositionTable {
	redisContext *redis;
	redisAsyncContext *c redis;
}

struct TranspositionTable *
transposition_table_new(void) {
	redisAsyncConnect("127.0.0.1", 6339);
}


void
transpos_table_drop(struct TransposTable *ttable) {
	redisAsyncDisconnect(ttable->redis);
}*/
