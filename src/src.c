#define InfoString \
"Welcome to the nth language interpreter.\r\n\
Copyright (C) Daniel Smith daniel.smith.again@gmail.com"

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
#define OUT STDOUT_FILENO


typedef unsigned long long int Int;

typedef struct nth_unit {
        char *Type;
        Int Size;
        union {
                struct {
                        char *Numerator;
                        char *Denominator;
                } Number;
                struct {
                        char *Data;
                } Symbol;
                struct {
                        struct nth_unit **Members;                     struct nth_unit **Members;
                } Collection;
                struct {
                        char Value;
                } Boolean;
                struct {
                        char *Type;
                        struct nth_unit **Elements;
                } Vector;
                struct {
                        Int Value;
                } Word;
                struct {
                        unsigned char Value;
                } Byte;
                struct {
                        struct nth_unit *Max;
                        struct nth_unit *Min;
                } Range;
                struct {
                        struct nth_unit Definition;
                        struct nth_unit Parameters;
                        struct nth_unit Result;
                        struct nth_unit Handlers;
                        struct nth_unit Closure;
                } Function;
        };
} Unit;

typedef enum {_Symbol, _String, _Number, _Expression, _Sequence, _Collection, _Type, _Quote, _Unquote, _Requote} Form;
typedef struct nth_program {
        Form Type;
        Int Size;
        union {char *Symbol; struct nth_program **Collection;};
        struct nth_program *Parent;
} Program;

char *Buffer;
Int BufferSize;
Int BufferLength;
Int Pos;
Int Offset;
char GetChar ();

char *Nest;
Int Depth;
void ShowHint();

Program *Read();
