#ifndef nth_h__
#define nth_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#define In STDIN_FILENO
#define Out STDOUT_FILENO
#define InfoString \
"Welcome to the nth compiler utility.\r\n\
Copyright (C) Daniel Smith daniel.smith.again@gmail.com\r\n"
typedef unsigned long long int Int;
typedef enum {Symbol, String, Number, Expression, Sequence, Collection, Type, Quote, Unquote, Requote} Syntax;
typedef struct Program__ {
	Syntax type;
	union{ char* symbol; struct Program__ **collection; };
	Int size;
	struct Program__ *parent;
} Program;

Program *Parse();
Program *Read();
Program *ReadString();
Program *ReadSymbol();
Program *ReadQuote();
Program *ReadCollection();
Program *ReadType();
char GetChar();
Program *FancyPrint(Program *p);
void Print(Program *r);
void Discard(Program *p);

struct nth__number;
struct nth_boolean;
struct nth__function;
struct nth__type;
struct nth__collection;
struct nth__symbol;
struct nth__expression;
struct nth_range;
struct nth_word;
struct nth_byte;
struct nth_place;
typedef struct nth__number {
	char* numerator;
        char* denominator;
	Int size;
} number;
typedef struct nth_boolean {
        char value;
} boolean;
typedef struct nth__collection {
	struct nth__number size;
	void ** members;
} collection;
typedef struct nth__expression {
	struct nth__collection expr;
} expression;
typedef struct nth__function {
	enum {Raw, Cached} kind;
	union {
		struct nth__expression f;
		void *F;
	};
} function;
typedef struct nth__type {
	enum {Qualified, Enumerated} kind;
	union {
		struct nth__function predicate;
		struct nth__collection members;
	};
} type;

typedef struct nth__symbol {
	char* data;
	Int size;
} symbol;


Program  *Clean(Program *p);
void *Compile(Program *p);
Program *Eval(Program *P, collection *Env);

typedef struct {
        void *Next;
        void *Prev;
        void *Info;
        Int Size;
} CompilerData;

Int IsAddition(Program *p);
Int IsSubtraction(Program *p);
Int IsMultiplication (Program *p);
Int IsDivision (Program *p);
Int IsMathExpression (Program *p);
Int Is

#endif //nth_h__