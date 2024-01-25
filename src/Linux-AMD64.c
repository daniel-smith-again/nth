#include "nth.h"

Program *env;
Int IsFreshRun = 1;
/*
typedef struct expression__ {
	enum{symbol, expression} T;

};*/

void Run(Program *p) {
	if (IsFreshRun) {
		env = malloc(sizeof(Program));
	}
}