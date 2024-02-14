#ifndef __nth_h__
#define __nth_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef Linux_AMD64

typedef uint64_t Int;
typedef Int Word;
typedef uint8_t Byte;
typedef Byte* Symbol;

#endif

typedef struct __program__ 
{
  Int size; 
  enum {symbol, string, number, expression} type; 
  union{
    struct __program__* list;
    Symbol *atom;
    char *string;
    Symbol *number;
  };
} Program;

typedef struct __number__ 
{
  Byte* Numerator;
  Byte* Denominator;
} Number;

typedef struct __range__ 
{
  struct __number__ min;
  struct __number__ max;
} Range;

typedef struct __collection__ 
{
  Int size;
  Symbol* names;
  Symbol* types;
  void* fields;
} Collection;

typedef struct __vector__ 
{
  Int size;
  Symbol type;
  void* Elements;
} Vector;

typedef struct __type__
{
  Int size;
  Symbol* members;
} Type;

typedef struct __boolean__
{
  Byte value;
} Boolean;

typedef struct __function__
{
  Int size;
  Symbol* parameters;
  Symbol result;
  Program* definition;
  Vector handlers;
} Function;

typedef struct __unit__ {
  enum {byte, word, number, range, collection, vector, type, boolean, function} format;
  union{
    Byte getByte;
    Word getWord;
    Number getNumber;
    Range getRange;
    Collection getCollection;
    Vector getVector;
    Type getType;
    Boolean getBoolean;
    Function getFunction;
    Program getProgram;
    Symbol getSymbol;
  };
} Unit;

void StartShell();
void ExitShell();
void InitSession();
void (*Compile())(Program* expr, Collection* scope, void* txt, Int txtl);
Program *Read();
void Print(Unit data);
Unit *Eval(Program *p);

#endif //nth_h__