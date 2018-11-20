#include "tablebase.h"

enum TablebaseFormat {
	TABLEBASE_SYZGYZ,
	TABLEBASE_NALIMOV,
};

struct Tablebase
{
	char *path;
	enum TablebaseFormat format;
};
