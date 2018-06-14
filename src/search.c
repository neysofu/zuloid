#include <stdlib.h>
#include "hiredis/hiredis.h"
#include "search.h"
#include "settings.h"

// DB setup:
// We store serch nodes (be it unexplored moves or chessboards) as keys in the
// DB, the value of which is the score and other outputs from the neural
// network. During search, we keep a prioritized task list in-memory with the
// list of nodes that we want to analyze. This list is updated very often and if
// it becomes too long it can be cut or written to disk. If lost and no copy
// is stored, we don't know how to pick up the search again! We can also embed
// pointers to other keys in the LevelDb instance, but this requires a lot of
// space. In short time controls, I can also prune the tasks list, but I don't
// know how to scale this. So, where to store the search nodes? LevelDB with
// flatbuffers seems like a good idea. Maybe too slow...

struct SearchPriorityQueue {
	redisContext *redis;
};

struct SearchPriorityQueue *
search_priority_queue_new(struct SearchPriorityQueue *spq) {
	if (!spq) {
		spq = malloc(sizeof(struct SearchPriorityQueue));
		if (!spq) {
			return NULL;
		}
	}
	system("redis-server &");
	spq->redis = redisConnect("127.0.0.1", Z64C_REDIS_PORT);
	return spq;
}

void
search_priority_queue_drop(struct SearchPriorityQueue *spq) {
	redisFree(spq->redis);
	free(spq);
}

uint64_t
search_priority_queue_pop(struct SearchPriorityQueue *spq) {
	redisCommand(spq->redis, "ZPOPMIN spq");
	return 0; // TODO
}

void
search_priority_queue_push(struct SearchPriorityQueue *spq, float priority, uint64_t value) {
	redisCommand(spq->redis, "ZADD spq key");
	redisCommand(spq->redis, "expire key 30");
}
