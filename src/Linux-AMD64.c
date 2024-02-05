#include "nth.h"

void Init(collection *I) {
        I = malloc(sizeof(collection));
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
		case Unquote:
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
			strncpy(p->collection[0]->symbol, "unquote", 7);
			p->collection[0]->parent = p;
			break;
		case Requote:
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
			strncpy(p->collection[0]->symbol, "requote", 7);
			p->collection[0]->parent = p;
			break;
		case Expression:
			for (Int n = 0; n < p->size; n++) {
				Clean(p->collection[n]);
			}
			break;
		case String:
			Program *tmp;
			if (p->size == 1 && p->collection[0]->type == Symbol) {
				tmp = p->collection[0];
				free(p->collection);
				p->size = tmp->size;
				p->symbol = tmp->symbol;
				p->type = String;
			}
			else {
				tmp = malloc(sizeof(Program));
				tmp->size = p->size + (p->size - 1);
				tmp->type = Expression;
				tmp->parent = p->parent;
				tmp->collection = malloc(sizeof(Program*) * tmp->size);
				for (Int n = 0, m = 0; n < p->size; n++, m++) {
					tmp->collection[m] = p->collection[n];
					if (tmp->collection[m]->type == Symbol)
						tmp->collection[m]->type = String;
					if (n < p->size - 1) {
						m++;
						tmp->collection[m] = malloc(sizeof(Program));
						tmp->collection[m]->type = Symbol;
						tmp->collection[m]->size = 1;
						tmp->collection[m]->symbol = malloc(sizeof(char));
						tmp->collection[m]->symbol[0] = '+';
						tmp->collection[m]->parent = tmp;
					}
				}
				free(p->collection);
				p->collection = tmp->collection;
				p->size = tmp->size;
				p->type = tmp->type;
			}
			free(tmp);
			break;
	}
	return p;
}

void *Compile(Program *p) {

}

Program *Eval(Program *p, collection *Env) {
	//FancyPrint(p);
        Compile(p);
        
}

char GetChar() {
	char c;
	Int r = read(In, &c, 1);
	if (r == 1) 
		return c;
	else
		return (char)0;
}