#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sysexits.h>
#include "hiredis/hiredis.h"
#include "search.h"
#include "settings.h"

#define SEARCH_PRIORITY_QUEUE_REDIS_DB_NAME "Z64C_spq"

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
	pid_t pid = fork();
	if (pid == 0) {
		if (execl("redis-server", "--port", "5640", 0) == -1) {
			exit(EX_OSERR);
		}
	} else if (pid > 0) {
		spq->redis = redisConnect("127.0.0.1", Z64C_REDIS_PORT);
	} else if (pid == -1) {
		return NULL;
	}
	return spq;
}

void
search_priority_queue_drop(struct SearchPriorityQueue *spq) {
	redisFree(spq->redis);
	free(spq);
}

struct Move
search_priority_queue_pop_move(struct SearchPriorityQueue *spq) {
	struct redisReply *reply = redisCommand(spq->redis, "ZPOPMAX %s 1", SEARCH_PRIORITY_QUEUE_REDIS_DB_NAME);
	if (!reply || reply->type != REDIS_REPLY_STRING) {
		return MOVE_NONE;
	}
	return str_to_move(reply->str);
}

void
search_priority_queue_grow(struct SearchPriorityQueue *spq) {
	redisCommand(spq->redis, "ZREVRANGEBYSCORE spq 1 0 WITHSCORES LIMIT 0 1");
	// TODO
}

struct SearchParams *
search_params_new(struct SearchParams *sp) {
	if (!sp) {
		sp = malloc(sizeof(struct SearchParams));
		if (!sp) {
			return NULL;
		}
	}
	return sp;
}

void
search_params_drop(struct SearchParams *sp) {
	free(sp);
}
