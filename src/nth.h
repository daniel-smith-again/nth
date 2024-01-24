#ifndef nth_h__
#define nth_h__

#include <stdio.h>
#include <stdlib.h>
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
typedef struct Program__ {
	enum {Symbol, Expression, Sequence, Collection, Type, Quote, Insert, Spread} type;
	union{ char* symbol; struct Program__ **collection; };
	Int size;
	struct Program__ *parent;
} Program;

Program *Parse();
Program *Eval(Program *p);
void Run(Program *p);
void Print(Program *r);
void Discard(Program *p);

#endif //nth_h__