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

Program  *Clean(Program *p);
void *Compile(Program *p);
Program *Eval(Program *P);

Int IsAddition(Program *p);
Int IsSubtraction(Program *p);
Int IsMultiplication (Program *p);
Int IsDivision (Program *p);
Int IsMathExpression (Program *p);

#endif //nth_h__