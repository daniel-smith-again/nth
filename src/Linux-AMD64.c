#include "nth.h"

void Init(collection *I) {

}

Program *Clean(Program *p) { //turn sugared syntax into pure s-expr
	switch(p->type) {
		case Symbol:
			//check if number then switch to number
			p->type = Number;
			for (Int n = 0, divradix = 0, dotradix = 0; n < p->size; n++) {
				if (p->symbol[n] < '0' || p->symbol[n] > '9') {
					if (p->symbol[n] == '.' && !dotradix && n > 0) 
						dotradix = 1;
					else
						goto NaN;
					if (p->symbol[n] == '/' && !divradix && n > 0)
						divradix = 1;
					else
						goto NaN; 
					if (n == 0) {
						if (p->symbol[n] != '+' || p->symbol[n] != '-')
							goto NaN;
					} 
				}
			}
			break;
			NaN:
				p->type = Symbol;
				break;
		case Sequence:
			for (Int n = 0; n < p->size; n++) {
				Clean(p->collection[n]);
			}
			p->type = Expression;
			p->size++;
			p->collection = realloc(p->collection, sizeof(Program*) * p->size);
			for (Int n = p->size; n > 1; n--) {
				p->collection[n-1] = p->collection[n-2];
			}
			p->collection[0] = malloc(sizeof(Program));
			p->collection[0]->type = Symbol;
			p->collection[0]->size = 2;
			p->collection[0]->collection = 0;
			p->collection[0]->symbol = malloc(sizeof(char) * 2);
			strncpy(p->collection[0]->symbol, "do", 2);
			p->collection[0]->parent = p;
			break;
		case Collection:
			//change to (fields ...)
			for (Int n = 0; n < p->size; n++) {
				Clean(p->collection[n]);
			}
			p->type = Expression;
			p->size ++;
			p->collection = realloc(p->collection, sizeof(Program*) * p->size);
			for (Int n = p->size; n > 1; n--) {
				p->collection[n - 1] = p->collection[n - 2];
			}
			p->collection[0] = malloc(sizeof(Program));
			p->collection[0]->type = Symbol;
			p->collection[0]->size = 6;
			p->collection[0]->collection = 0;
			p->collection[0]->symbol = malloc(sizeof(char) * 6);
			strncpy(p->collection[0]->symbol, "fields", 6);
			p->collection[0]->parent = p;
			break;
		case Type:
			//change to (members ...)
			for (Int n = 0; n < p->size; n++) {
				Clean(p->collection[n]);
			}
			p->type = Expression;
			p->size ++;
			p->collection = realloc(p->collection, sizeof(Program*) * p->size);
			for (Int n = p->size; n > 1; n--) {
				p->collection[n - 1] = p->collection[n - 2];
			}
			p->collection[0] = malloc(sizeof(Program));
			p->collection[0]->type = Symbol;
			p->collection[0]->size = 7;
			p->collection[0]->collection = 0;
			p->collection[0]->symbol = malloc(sizeof(char) * 7);
			strncpy(p->collection[0]->symbol, "members", 7);
			p->collection[0]->parent = p;
			break;
		case Quote:
			for (Int n = 0; n < p->size; n++) {
				Clean(p->collection[n]);
			}
			p->type = Expression;
			p->size ++;
			p->collection = realloc(p->collection, sizeof(Program*) * p->size);
			for (Int n = p->size; n > 1; n--) {
				p->collection[n - 1] = p->collection[n - 2];
			}
			p->collection[0] = malloc(sizeof(Program));
			p->collection[0]->type = Symbol;
			p->collection[0]->size = 5;
			p->collection[0]->collection = 0;
			p->collection[0]->symbol = malloc(sizeof(char) * 5);
			strncpy(p->collection[0]->symbol, "quote", 5);
			p->collection[0]->parent = p;
			break;
		case Insert:
			for (Int n = 0; n < p->size; n++) {
				Clean(p->collection[n]);
			}
			p->type = Expression;
			p->size ++;
			p->collection = realloc(p->collection, sizeof(Program*) * p->size);
			for (Int n = p->size; n > 1; n--) {
				p->collection[n - 1] = p->collection[n - 2];
			}
			p->collection[0] = malloc(sizeof(Program));
			p->collection[0]->type = Symbol;
			p->collection[0]->size = 6;
			p->collection[0]->collection = 0;
			p->collection[0]->symbol = malloc(sizeof(char) * 6);
			strncpy(p->collection[0]->symbol, "insert", 6);
			p->collection[0]->parent = p;
			break;
		case Spread:
			for (Int n = 0; n < p->size; n++) {
				Clean(p->collection[n]);
			}
			p->type = Expression;
			p->size ++;
			p->collection = realloc(p->collection, sizeof(Program*) * p->size);
			for (Int n = p->size; n > 1; n--) {
				p->collection[n - 1] = p->collection[n - 2];
			}
			p->collection[0] = malloc(sizeof(Program));
			p->collection[0]->type = Symbol;
			p->collection[0]->size = 6;
			p->collection[0]->collection = 0;
			p->collection[0]->symbol = malloc(sizeof(char) * 6);
			strncpy(p->collection[0]->symbol, "spread", 6);
			p->collection[0]->parent = p;
			break;
		case Expression:
			for (Int n = 0; n < p->size; n++) {
				Clean(p->collection[n]);
			}
			break;
	}
	return p;
}

void Eval(Program *p, collection *Env) {
	//write(Out, "EVALUATING", 10);
	Program *n = Clean(p);
	FancyPrint(n);
	
}