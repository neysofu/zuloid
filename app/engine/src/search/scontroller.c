#include "search/scontroller.h"
#include <unistd.h>

struct SController
{
	struct TTable *ttable;
	//thrd_t (*workers)[];
};

struct SController *
scontroller_new(struct TTable *ttable)
{
	struct SController *scontroller = xmalloc(sizeof(struct SController));
	scontroller->ttable = ttable;
	//scontroller->workers = xmalloc(sizeof(thrd_t) * sysconf(_SC_NPROCESSORS_ONLN));
	return scontroller;
}

void
scontroller_free(struct SController *scontroller)
{
	assert(scontroller);
	//free(scontroller->workers);
	free(scontroller);
}
