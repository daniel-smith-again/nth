#define InfoString \
"Welcome to the nth language utility.\r\n\
Copyight (C) Daniel Smith daniel.smith.again@gmail.com"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef unsigned long long int Int;
typedef Int Word;
typedef unsigned char Byte;

typedef struct __program__ 
{
  Int size;
  enum {Symbol, String, Number, Expression} type;
  union {
    struct __program__ **expression;
    char *symbol;
    char *string;
    struct {char *numerator; Int nml; char*denominator; Int dnl;};
  };
} Program;

Program Error = { .size = 1, .type = Symbol, .symbol = "?"};

char *Buffer;
Int BufferSize, BufferLength, Pos, Offset;
char ReadChar();
char PeekChar();
void LineBreak();

Program *Read();
Program *Compute(Program *exp);
void (*Compile())(Program *exp);
void Print(Program *data);
void Delete();

void InitImage();
void StartShell();
void ExitShell();

int main ()
{
  Program *program;
  StartShell();
  Begin: program = Read();
  if (program)
    //Print(Compute(program));
    Print(program), LineBreak();
  else 
  {
    Print(&Error);
    LineBreak();
    if (Buffer) free(Buffer), Buffer = 0, BufferSize = 0; BufferLength = 0; Offset = 0, Pos = 0;
    if (program) Delete(program), program = 0;
  }
  goto Begin;
  return 0;
}

#ifdef Linux_AMD64

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

struct termios raw, restore;
struct winsize window;
void WinSizeCh(int signum)
{
  ioctl(Out, TIOCGWINSZ, &window);
}

void StartShell()
{
  atexit(ExitShell);
  tcgetattr(0, &restore);
  raw = restore;
  raw.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_cflag &= ~(CSIZE | PARENB);
	raw.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &raw);
	ioctl(Out, TIOCGWINSZ, &window);
	signal(SIGWINCH, WinSizeCh);
	//write(Out, "\x1b[2J\x1b[H", 7);
	write(Out, "\r\n", 2);
	write(Out, InfoString, sizeof(InfoString));
	write(Out, "\r\n", 2);
  return;
}

void LineBreak()
{
  write(Out, "\r\n", 2);
}

void ExitShell()
{
  write(Out, "\r\n", 2);
  tcsetattr(0, TCSANOW, &restore);
  exit(1);
}

char PeekChar()
{
  char c = ReadChar();
  Pos--;
  return c;
}

void Echo()
{
  char* str = 0;
  write(Out, "\x1b[2K\r", 5);
  if (Offset > window.ws_col)
  {
    str = 0, str = malloc(sizeof(char) * (window.ws_col - 1));
    if (str) strncpy(str, &Buffer[BufferLength - (window.ws_col - 1)], window.ws_col - 1);
    else ExitShell();
    write(Out, str, window.ws_col - 1);
  }
  else
  {
    str = 0, str = malloc(sizeof(char) * (window.ws_col - 1));
    if (str) strncpy(str, &Buffer[BufferLength - Offset], Offset);
    else ExitShell();
    write(Out, str, Offset);
  }
  free(str);

}

char ReadChar()
{
  char c[128];
  int r = 0;
  if (BufferLength == 0 || Pos >= BufferLength)
  {
    BufferLength = 0, Pos = 0, BufferSize = 1024, Buffer = realloc(Buffer, sizeof(char) * BufferSize);
    for (;;)
    {
      r = 0;
      if (r = read(In, c, 128), r)
      {
        if (r > 1) continue;
        if (c[0] == 27) ExitShell();
        else if (c[0] == 8 || c[0] == 127)
        {
          if (Offset > 0 && BufferLength > 0)
            BufferLength --, Offset--, Echo();
        }
        else if (c[0] > 9 && c[0] < 14)
        {
          write(Out, "\r\n", 2);
          BufferLength ++;
          if (BufferLength >= BufferSize)
            BufferSize += 1028, Buffer = realloc(Buffer, sizeof(char) * BufferSize);
          Buffer[BufferLength - 1] = ' ';
          Offset = 0;
          Echo();
          goto End;
        }
        else if (c[0] >= ' ')
        {
          BufferLength++;
          if (BufferLength >= BufferSize)
            BufferSize += 1028, Buffer = realloc(Buffer, sizeof(char) * BufferSize);
          Buffer[BufferLength - 1] = c[0];
          Offset++;
          Echo();
        }
      }
    }
  }
  End:
  c[0] = Buffer[Pos], Pos++;
  return c[0];
}

void Print(Program *data) 
{
  switch(data->type) {
    case Symbol:
      write(Out, data->symbol, data->size);
      break;
    case Expression:
      write(Out, "(", 1);
      for (Int n = 0; n < data->size; n++)
      {
        Print(data->expression[n]);
        if (n < data->size - 1)
        {
          write(Out, " ", 1);
        }
      }
      write(Out, ")", 1);
      break;
    case Number:
      write(Out, data->numerator, data->nml);
      if (data->dnl)
        write(Out, "/", 1), write(Out, data->denominator, data->dnl);
      break;
    case String:
      write(Out, "\"", 1);
      write(Out, data->string, data->size);
      write(Out, "\"", 1);
      break;
  }
  return;
}
#endif

Program *Read()
{
  Program *e = malloc(sizeof(Program)), *tmp = 0;
  char c = 0;
  char *str = 0;
  Int strl = 0;
  TryAgain:
  c = PeekChar();
  switch(c)
  {
    case ' ': ReadChar(); goto TryAgain;
    case ')': return 0;
    case '(':
      ReadChar();
      e->type = Expression;
      e->size = 0;
      e->expression = malloc(0);
      for (c = PeekChar(); c != ')'; c = PeekChar())
        if (c == ' ') 
        {
          ReadChar();
          continue;
        }
        else 
        {
          tmp = Read();
          if (tmp)
          {
            e->size++;
            e->expression = realloc(e->expression, sizeof(Program*) * e->size);
            e->expression[e->size - 1] = tmp;
          }
          else return 0;
        }
      ReadChar();
      return e;
    case '"':
      ReadChar();
      strl = 0, str = malloc(sizeof(char) * strl);
      for (c = PeekChar(); c != '"'; c = PeekChar())
      {
        if (c == '\\')
          ReadChar(), c = PeekChar();
        strl++, str = realloc(str, sizeof(char) * strl), str[strl - 1] = c, ReadChar();
      }
      e->type = String, e->size = strl, e->string = str, ReadChar();
      return e;
    default:
      strl = 0, str = malloc(sizeof(char) * strl);
      for (c = PeekChar(); c != ' ' && c != '(' && c != ')' && c != '"' ; c = PeekChar())
        strl++, str = realloc(str, sizeof(char) * strl), str[strl - 1] = c, ReadChar();
      e->type = Number;
      e->nml = 0;
      e->dnl = 0;
      for (Int n = 0, radix = 0; n < strl; n++)
        if (str[n] >= '0' && str[n] <= '9')
          if (radix) e->dnl++;
          else e->nml++;
        else if (n < strl - 1 && !radix && (str[n] == '.' || str[n] == '/')) radix = 1;
        else e->type = Symbol;
      if (e->type == Number)
      {
        if (e->nml < strl && str[e->nml] == '.')
        {
          e->dnl ++;
          e->denominator = malloc(sizeof(char) * e->dnl);
          for (Int n = 0; n < e->dnl; n++)
            if (n == 0) e->denominator[n] = '1';
            else e->denominator[n] = '0';
          e->nml = strl - 1;
          e->numerator = malloc(sizeof(char) * strl - 1);
          for (Int n = 0, m = 0; n < strl; n++)
            if (str[n] == '.') continue;
            else e->numerator[m] = str[n], m++;
        }
        else if (e->nml < strl && str[e->nml] == '/')
        {
          e->numerator = malloc(sizeof(char) * e->nml);
          e->denominator = malloc(sizeof(char) * e->dnl);
          strncpy(e->numerator, str, e->nml);
          strncpy(e->denominator, &str[e->nml + 1], e->dnl);
        }
        else
        {
          e->dnl = 0;
          e->numerator = malloc(sizeof(char) * e->nml);
          strncpy(e->numerator, str, e->nml);
        }
        e->type = Number;
        e->size = 0;
        free(str);
        return e;
      }
      else
      {
        e->type = Symbol;
        e->size = strl;
        e->symbol = str;
      }
      return e;
  }
}

void Delete(Program *n)
{
  switch(n->type)
  {
    case Symbol:
      free(n->symbol);
    break;
    case String:
      free(n->string);
      break;
    case Number:
      free(n->numerator), free(n->denominator);
      break;
    case Expression:
      for (Int i = 0; i < n->size; i++)
      {
        Delete(n->expression[i]);
        free(n->expression[i]);
      }
      free(n->expression);
    break;
  }
  return;
}

typedef enum {Image, Function, Type, Collection, Range, Vector, ProgramModel, SymbolConstant} Format;
struct __image__;
struct __function__;
struct __type__;
struct __collection__;
struct __range__;
struct __vector__;
struct __number__;
struct __word__;
struct __byte__;
struct __place__;
struct __symbol__;



typedef struct __image__
{
  struct __symbol__* names;
  struct __unit__* data;
} image;

typedef struct __function__ 
{
  Byte IsCompiled;
  char *definition;
  Int definitionl;
  Program *body;
  void (*compiledBody)();
  struct __symbol__ *parameters;
  Int parametersl;
  struct __unit__ *types;
  struct __unit__ *result;
  struct __function__ *handlers;
  Int handlersl;
  struct __image__ *closure;
} function;

typedef struct __type__
{
  enum {qualified, enumerated} which;
  union {
    struct __function__ *pred;
    struct __unit__ **members;
  };
  Int membersl;
} type;

typedef struct __collection__
{
  struct __unit__ *fields;
  char **names;
  Int fieldsl;
} collection;

typedef struct __range__
{
  struct __number__ *min;
  struct __number__ *max;
} range;

typedef struct __vector__
{
  struct __unit__ *kind;
  struct __unit *indicies;
  Int length;
} vector;

typedef struct __number__
{
  enum {Natural, Integer, Rational} subset;
  Byte sign;
  unsigned char *numerator;
  Int numeratorl;
  unsigned char *denominator;
  Int denominatorl;
} number;

typedef struct __word__ 
{
  unsigned long long int w;
} word;

typedef struct __byte__
{
  unsigned char b;
} byte;

typedef struct __symbol__
{
  Int length;
  char* data;
} symbol;

typedef struct __place__
{
  void *p;
} place;

typedef struct __unit__ 
{
  Format kind;
  union {
    struct __image__ i;
    struct __function__ f;
    struct __type__ t;
    struct __collection__ c;
    struct __range__ r;
    struct __vector__ v;
    struct __number__ n;
    struct __word__ w;
    struct __byte__ b;
    struct __place__ p;
    struct __symbol__ s;
  };
} Unit;

#ifdef Linux_AMD64

image *Env;
Int EnvLevel;
image *TopLevel;

void InitImage()
{
  Env = malloc(sizeof(image));
  TopLevel = Env;
  EnvLevel = 0;
  
}

Int IsPrimRoutine(Program *p)
{
  if (p->size == 0) return 0;
  if (p->type != Expression) return 0;
  if (p->size == 1)
  {
    if (0 == strncmp(p->symbol, "'", 1)) return 1;
    if (0 == strncmp(p->symbol, "+", 1)) return 20;
    if (0 == strncmp(p->symbol, "-", 1)) return 21;
    if (0 == strncmp(p->symbol, "*", 1)) return 22;
    if (0 == strncmp(p->symbol, "/", 1)) return 23;
    if (0 == strncmp(p->symbol, "^", 1)) return 24;
    if (0 == strncmp(p->symbol, "#", 1)) return 50;
    if (0 == strncmp(p->symbol, "?", 1)) return 52;
  }
  else if (p->size == 2)
  {
    if (0 == strncmp(p->symbol, "''", 2)) return 2;
    if (0 == strncmp(p->symbol, "do", 2)) return 9;
    if (0 == strncmp(p->symbol, "if", 2)) return 11;
    if (0 == strncmp(p->symbol, "/^", 2)) return 25;
    if (0 == strncmp(p->symbol, "//", 2)) return 26;
    if (0 == strncmp(p->symbol, "||", 2)) return 30;
    if (0 == strncmp(p->symbol, "*/", 2)) return 31;
    if (0 == strncmp(p->symbol, "/.", 2)) return 32;
    if (0 == strncmp(p->symbol, "or", 2)) return 41;
    if (0 == strncmp(p->symbol, "##", 2)) return 51;
    if (0 == strncmp(p->symbol, "at", 2)) return 61;
    if (0 == strncmp(p->symbol, "in", 2)) return 62;
  }
  else if (p->size == 3)
  {
    if (0 == strncmp(p->symbol, "'''", 3)) return 3;
    if (0 == strncmp(p->symbol, "let", 3)) return 8;
    if (0 == strncmp(p->symbol, "mod", 3)) return 3;
    if (0 == strncmp(p->symbol, "and", 3)) return 40;
    if (0 == strncmp(p->symbol, "not", 3)) return 42;
    if (0 == strncmp(p->symbol, "the", 3)) return 53;
  }
  else if (p->size == 4)
  {
    if (0 == strncmp(p->symbol, "take", 4)) return 5;
    if (0 == strncmp(p->symbol, "some", 4)) return 6;
    if (0 == strncmp(p->symbol, "with", 4)) return 10;
    if (0 == strncmp(p->symbol, "from", 4)) return 13;
    if (0 == strncmp(p->symbol, "ceil", 4)) return 29;
  }
  else if (p->size == 5)
  {
    if (0 == strncmp(p->symbol, "given", 5)) return 12;
    if (0 == strncmp(p->symbol, "floor", 5)) return 28;
  }
  else if (p->size == 6 && strncmp(p->symbol, "define", 6) == 0) return 14;
  else if (p->size == 7 && strncmp(p->symbol, "collect", 7) == 0) return 4;
  else if (p->size == 8 && strncmp(p->symbol, "function", 8) == 0) return 7;
}

void (*Compile())(Program *exp)
{
  
  return 0;
}

#endif