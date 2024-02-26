#define InfoString \
"Welcome to the nth language programming utility\r\n\
Copyright (C) Daniel Smith daniel.smith.again@gmail.com"
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
#include <dlfcn.h>
#include <elf.h>
#define In STDIN_FILENO
#define Out STDOUT_FILENO
typedef unsigned long long int Int;
typedef enum {image, function, type, collection, string, range, vector, program, symbol, byte, word} Format;
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
  struct __image__ *parent;
} Image;
typedef struct __function__
{
  struct __byte__ IsCompiled;
  void *text;
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
  char sign;
  char *numerator;
  char nl;
  char *denominator;
  char dl;
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

typedef struct __program__ 
{
  enum {Sym, Exp, Str, Num} kind;
  union {
    struct __symbol__ sym;
    struct {struct __program__ *e; Int length;};
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
    //struct __word__ w;
    Int w; //machine word
    //struct __byte__ b;
    unsigned char b; //machine byte
    //struct __place__ a;
    void * a; //place
    struct __string__ s;
    struct __symbol__ y;
    struct __program__ p;

  };
} Unit;
Unit ErrSym = {.which = (Format)symbol, .y = (Symbol){.data = "?", .length = 1}};

void StartShell();
void ExitShell();

char *Buffer;
Int BufferSize, BufferLength, Pos, Offset;
char *Nesting;
Int Nestingl;
char ReadChar();
char PeekChar();
void LineBreak();

Program *Read();
void InitImage();
Unit *Compute(Program *exp);
void Print(Unit *exp);
void Discard(Unit *u);
void Delete(Program *p);
void Quit();

void main()
{
  StartShell();
  InitImage();
  Program *p;
  Unit *r;
  Nesting = malloc(sizeof(char) * 0);
  Nestingl = 0;
  Loop:
  if (Nesting) free(Nesting);
  Nesting = malloc(sizeof(char) * 0);
  Nestingl = 0;
  p = Read();
  if (p) r = Compute(p);
  else goto Error;
  if (r) Print(r), LineBreak();
  else goto Error;
  goto Loop;
  Error:
  Print(&ErrSym), LineBreak();
  if (p) Delete(p);
  if (r) Discard(r);
  p = 0, r = 0;
  goto Loop;
  return;
}

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
          Buffer[BufferLength - 1] = '\n';
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

void LineBreak()
{
  write(Out, "\r\n", 2);
}


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
    case '\n': 
      ReadChar();
      for (Int n = Nestingl; n > 0; n--)
        write(Out, &Nesting[n - 1], 1);
      goto TryAgain;
    case ')': ReadChar(); return 0;
    case '(':
      ReadChar();
      Nestingl++;
      Nesting = realloc(Nesting, sizeof(char) * Nestingl);
      Nesting[Nestingl - 1] = ')';
      e->kind = Exp;
      e->length = 0;
      e->e = malloc(sizeof(Program) * e->length);
      for (c = PeekChar(); c != ')'; c = PeekChar())
        if (c == ' ')
        {
          ReadChar(); 
          continue;
        }
        else if (c == '\n')
        {
          ReadChar();
          for (Int n = Nestingl; n > 0; n--)
            write(Out, &Nesting[n - 1], 1);
          continue;
        }
        else 
        {
          tmp = Read();
          if (tmp)
          {
            e->length++;
            e->e = realloc(e->e, sizeof(Program) * e->length);
            e->e[e->length - 1] = *tmp;
          }
          else return 0;
        }
      ReadChar();
      if (Nestingl > 0) Nestingl--;
      return e;
    case '"':
      ReadChar();
      Nestingl++;
      Nesting = realloc(Nesting, sizeof(char) * Nestingl);
      Nesting[Nestingl - 1] = '"';
      e->kind = Exp;
      e->length = 1;
      e->e = malloc(sizeof(Program) * e->length);
      e->e[0].kind = Sym;
      e->e[0].sym.data = malloc(sizeof(char) * 0);
      e->e[0].sym.length = 0;
      for (c = PeekChar(); c != '"'; c = PeekChar())
      {
        if (c == '\n') c = ' ';
        if (c == '\\')
        {
          ReadChar();
          c = PeekChar();
          if (c == '\n') c = ' ';
          if (c == '(')
          {
            e->length++;
            e->e = realloc(e->e, sizeof(Program) * e->length);
            tmp = Read();
            if (tmp) e->e[e->length - 1] = *tmp, free(tmp);
            else return 0;
            e->length++;
            e->e = realloc(e->e, sizeof(Program) * e->length);
            e->e[e->length - 1].kind = Sym;
          }
          else {
            e->e[e->length - 1].length++;
            e->e[e->length - 1].sym.data = realloc(e->e[e->length - 1].sym.data, sizeof(char) * e->e[e->length - 1].sym.length);
            e->e[e->length - 1].sym.data[e->e[e->length - 1].sym.length - 1] = c;
            ReadChar();
          }
        }
        else {
            e->e[e->length - 1].length++;
            e->e[e->length - 1].sym.data = realloc(e->e[e->length - 1].sym.data, sizeof(char) * e->e[e->length - 1].sym.length);
            e->e[e->length - 1].sym.data[e->e[e->length - 1].sym.length - 1] = c;
            ReadChar();
        }
      }
      if (Nestingl > 0) Nestingl--;
      ReadChar();
      if (e->length < 2)
      {
        str = e->e[e->length - 1].sym.data;
        strl = e->e[e->length - 1].sym.length;
        free(e->e);
        e->kind = Str;
        e->str.length = strl;
        e->str.chars = (Character*)str;
        return e;
      }
      else {
        e->kind = Exp;
        for (Int n = 0; n < e->length; n++)
          if (e->e[n].kind == Sym)
          {
            str = e->e[n].sym.data;
            strl = e->e[n].sym.length;
            e->e[n].kind = Str;
            e->e[n].str.chars = (Character*)str;
            e->e[n].str.length = strl;
          }
        e->length++;
        e->e = realloc(e->e, sizeof(Program) * e->length);
        for (Int n = e->length - 1; n > 0; n--)
          e->e[n] = e->e[n - 1];
        e->e[0].kind = Sym;
        e->e[0].sym.data = malloc(sizeof(char));
        e->e[0].sym.length = 1;
        e->e[0].sym.data[0] = '+';
        return e;
      }
      break;
    default:
      strl = 0, str = malloc(sizeof(char) * strl);
      for (c = PeekChar(); c != ' ' && c != '(' && c != ')' && c != '"' && c != '\n'; c = PeekChar())
        strl++, str = realloc(str, sizeof(char) * strl), str[strl - 1] = c, ReadChar();
      e->kind = Num;
      e->num.nl = 0;
      e->num.dl = 0;
      for (Int n = 0, radix = 0; n < strl; n++)
        if (str[n] >= '0' && str[n] <= '9')
          if (radix) e->num.dl++;
          else e->num.nl++;
        else if (n < strl - 1 && !radix && str[n] == '.') radix = 1;
        else if (n < strl - 1 && !radix && str[n] == '/') radix = 2;
        else if (n == 0 && str[n] == '-') e->num.sign = 1;
        else if (n == 0 && str[n] == '+') e->num.sign = 0;
        else e->kind = Sym;
      if (e->kind == Num)
      {
        if (e->num.sign) e->num.sign = 1;
        else e->num.sign = 0;
        if (e->num.nl < strl && str[e->num.nl] == '.')
        {
          e->num.dl ++;
          e->num.denominator = malloc(sizeof(char) * e->num.dl);
          for (Int n = 0; n < e->num.dl; n++)
            if (n == 0) e->num.denominator[n] = '1';
            else e->num.denominator[n] = '0';
          e->num.nl = strl - 1;
          e->num.numerator = malloc(sizeof(char) * strl - 1);
          for (Int n = 0, m = 0; n < strl; n++)
            if (str[n] == '.') continue;
            else e->num.numerator[m] = str[n], m++;
        }
        else if (e->num.nl < strl && str[e->num.nl] == '/')
        {
          e->num.numerator = malloc(sizeof(char) * e->num.nl);
          e->num.denominator = malloc(sizeof(char) * e->num.dl);
          strncpy(e->num.numerator, str, e->num.nl);
          strncpy(e->num.denominator, &str[e->num.nl + 1], e->num.dl);
        }
        else
        {
          e->num.dl = 0;
          e->num.numerator = malloc(sizeof(char) * e->num.nl);
          strncpy(e->num.numerator, str, e->num.nl);
        }
        strl = 0;
        for (Int n = 0; n < e->num.nl; n++)
          if (e->num.numerator[n] == '0') strl++;
          else break;
        for (Int n = 0; n < e->num.nl; n++)
          e->num.numerator[n] = e->num.numerator[n + strl];
        e->num.nl -= strl, e->num.numerator = realloc(e->num.numerator, sizeof(char) * e->num.nl);
        free(str);
        return e;
      }
      else
      {
        e->kind = Sym;
        e->sym.length = strl;
        e->sym.data = str;
      }
      return e;
  }
}
void AddNumber()
{

}
Image *environment;
Image *toplevel;
void InitImage()
{
}

Unit *Compute(Program *exp)
{
  Unit *u = malloc(sizeof(Unit));
  u->which = program;
  u->p = *exp;
  return u;
}

void PrintProgram(Program p)
{
  switch(p.kind)
  {
    case Sym:
      write(Out, p.sym.data, p.sym.length);
      break;
    case Exp:
      write(Out, "(", 1);
      for (Int n = 0; n < p.length; n++)
        PrintProgram(p.e[n]), write(Out, " ", (n < p.length - 1 ? 1 : 0));
      write(Out, ")", 1);
      break;
    case Str:
      write(Out, "\"", 1);
      write(Out, (char*)p.str.chars, p.str.length);
      write(Out, "\"", 1);
      break;
    case Num:
      write(Out, p.num.numerator, p.num.nl);
      if (p.num.dl) write(Out, "/", 1), write(Out, p.num.denominator, p.num.dl);
  }
}

void Print(Unit *exp)
{
  switch(exp->which)
  {
    case symbol:
      write(Out, exp->y.data, exp->y.length);
      break;
    case collection:
      break;
    case program:
      PrintProgram(exp->p);
      break;
  }
}

void Discard(Unit *u)
{

}

void Delete(Program *p)
{

}

void Quit()
{

}