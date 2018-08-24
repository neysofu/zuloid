struct TTable {
	size_t num_buckets;
	struct TTableBucket *buckets[];
};

struct TTableBucket {
	struct TTableItem items[];
};

struct TTableItem {
	struct Board board;
	uint16_t count;
	Move moves[BOARD_AVERAGE_NUM_CANDIDATE_MOVES];
};

struct TTable *
ttable_new(size_t i) {
	struct TTable ttable = malloc(sizeof(struct TTable) +
			                      sizeof(* struct TTableItem) * i);
}

void
ttable_drop(struct TTable *ttable) {
	while (ttable->num_buckets-- > 0) {
		free(ttable->buckets[ttable->num_buckets])
	}
	free(ttable);
}

ttable_save_to_file(sruct TTable *ttable, FILE *file) {
}

void
ttable_insert(struct TTable *ttable, struct Board *board) {
	board_hash(board);
}

void
ttable_resize(struct TTable *ttable) {
	struct TTable *new_ttable = realloc
	size_t new_num_buckets = ttable->num_buckets << 1;
	do {
		ttable->buckets[ttable->num_buckets] = ttable_bucket_new();
	} while (ttable->num_buckets++ < new_num_buckets);
}

struct TTableBucket *
ttable_bucket_new(void) {
	return memset(malloc(TTABLE_ITEM_BUCKET_SIZE), 0, TTABLE_ITEM_BUCKET_SIZE);
}

void
ttable_bucket_drop(struct TTableBucket *bucket) {
	free(bucket);
}

void
ttable_delete(struct TTable *ttable, uint64_t hash) {
	while (ttable->num_buckets-- < new_num_buckets) {
		ttable_bucket_drop(ttable->buckets[ttable->num_buckets]);
	}
}
