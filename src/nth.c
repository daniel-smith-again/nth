#define InfoString \
"Welcome to the nth language programming utility\r\n\
Copyright (C) Daniel Smith daniel.smith.again@gmail.com"
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <elf.h>
typedef unsigned long long int Int;
typedef enum {image, function, type, collection, sequence, string, range, vector, program, symbol, byte, word} Format;
struct __image__;
struct __function__;
struct __type__;
struct __collection__;
struct __range__;
struct __vector__; 
struct __number__;
struct __character__;
struct __word__;
struct __byte__;
struct __place__;
struct __string__;
struct __program__;
struct __sequence__;
struct __unit__;

typedef struct __character__
{
  unsigned long int c;
} Character;

typedef struct __word__
{
  Int w;
} Word;

typedef struct __byte__
{
  unsigned char b;
} Byte;

typedef struct __image__
{
  struct __unit__ *data;
  struct __symbol__ *names;
  Int length;
} Image;
typedef struct __function__
{
  struct __byte__ IsCompiled;
  struct __byte__ *text;
  struct __program__ *definition;
  struct __symbol__ *parameters;
  struct __unit__ *types;
  Int parametersl;
  struct __unit__ *result;
  struct __function__ *handlers;
  Int handlersl;
  struct __image__ *closure;
} Function;

typedef struct __type__
{
  enum{qualified, enumerated} kind;
  union{
    struct __function__ *pred;
    struct __unit__ *members;
  };
  Int size;
} Type;

typedef struct __collection__
{
  struct __unit__ *fields;
  struct __symbol__ *names;
  Int size;
} Collection;

typedef struct __vector__
{
  struct __unit__ *kind;
  struct __unit__ *indicies;
  Int length;
} Vector;

typedef struct __number__
{
  struct __byte__ sign;
  struct __byte__ *numerator;
  Int nl;
  struct __byte__ *denominator;
  Int dl;
} Number;

typedef struct __range__
{
  struct __number__ min;
  struct __number__ max;
} Range;



typedef struct __symbol__
{
  char *data;
  Int length;
} Symbol;

typedef struct __place__
{
  void *p;
} Place;

typedef struct __string__
{
  struct __character__ *chars;
  Int length;
} String;

typedef struct __sequence__
{
  struct __program__ *dolist;
  Int length;
} Sequence;

typedef struct __program__ 
{
  Format kind;
  union {
    struct __symbol__ sym;
    struct __collection__ exp;
    struct __type__ typ;
    struct __sequence__ seq;
    struct __string__ str;
    struct __number__ num;
  };
} Program;

typedef struct __unit__
{
  Format which;
  union {
    struct __image__ i;
    struct __function__ f;
    struct __type__ t;
    struct __collection__ c;
    struct __range__ r;
    struct __vector__ v; 
    struct __number__ n;
    struct __character__ h;
    struct __word__ w;
    struct __byte__ b;
    struct __place__ a;
    struct __string__ s;
    struct __symbol__ y;
    struct __program__ p;
    struct __sequence__ q;
  };
} Unit;
Symbol ErrSymbol = {.data = "?", .length = 1};
Unit ErrSym;

char *Buffer;
Int BufferSize, BufferLength, Pos, Offset;
char ReadChar();
char PeekChar();
void LineBreak();

Program *Read();
Unit *Compute(Program *exp);
void Print(Unit *exp);
void Discard(Unit *u);
void Delete(Program *p);
void Quit();

void main()
{
  Program *p;
  Unit *r;
  Loop:
  p = Read();
  if (p) r = Compute(p);
  else goto Error;
  if (r) Print(r);
  else goto Error;
  Error:
  Print(&ErrSym);
  if (p) Delete(p);
  if (r) Discard(r);
  p = 0, r = 0;
  goto Loop;
  return;
}