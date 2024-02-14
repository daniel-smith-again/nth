#define InfoString \
"Welcome to the nth language interpreter.\r\n\
Copyright (C) Daniel Smith daniel.smith.again@gmail.com"

#ifdef Linux_AMD64
#define Target "Compiled for Linux x86-64."
#endif

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


typedef unsigned long long int Int;
typedef enum {Boolean, Number, Range, Byte, Word, Symbol, String, Collection, Type, Vector, Function} Primitive;
typedef struct nth_unit 
{
  struct nth_unit *Kind;
  Int Size;
  Primitive Which;
  union 
  {
    struct 
    {
      char *Numerator;
      char *Denominator;
    } Number;
    struct 
    {
      char *Data;
    } Symbol;
    struct 
    {
      char *Data;
    } String;
    struct 
    {
      char **Names;
      struct nth_unit **Fields;
    } Collection;
    struct
    {
      enum {Qualified, Enumerated} Kind;
      union 
      {
        struct nth_unit **Members;
        struct nth_unit *Predicate;
      };
    } Type;
    struct 
    {
      char Value;
    } Boolean;
    struct 
    {
      char *Type;
      struct nth_unit **Elements;
    } Vector;
    struct 
    {
      Int Value;
    } Word;
    struct 
    {
      unsigned char Value;
    } Byte;
    struct 
    {
      struct nth_unit *Max;
      struct nth_unit *Min;
    } Range;
    struct 
    {
      struct nth_unit *Definition;
      struct nth_unit *Parameters;
      struct nth_unit *Result;
      struct nth_unit *Handlers;
      struct nth_unit *Closure;
    } Function;
  };
} Unit;

Unit* Image;
Unit* TopLevel;
Unit* Result;

typedef enum {_Symbol, _String, _Number, _Expression, _Sequence, _Collection, _Type, _Quote, _Unquote, _Requote} Form;
typedef struct nth_program 
{
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
char NextChar ();
char PeekChar ();
void Echo();
void BufferAppend();

char *Nest;
Int Depth;
void ShowHint();

Program *Read(Int Quoted);
void Delete(Program *p);

void Init();
void Eval(Program *p);
void Desugar(Program *p);
Unit *Convert(Program *p);
void Discard(Unit *n);

void Print();

struct termios raw, restore;
struct winsize Window;
void SetRawMode();
void Exit();
void WinSizeCh (int signum) {
  ioctl(Out, TIOCGWINSZ, &Window);
}

int main() 
{
  atexit(Exit);
  SetRawMode();
  Buffer = malloc(sizeof(char) * 0);
  Image = malloc(sizeof(Unit) * 0);
  Nest = malloc(sizeof(char) * 0);
  BufferLength = 0;
  BufferSize = 0;
  Pos = 0;
  Offset = 0;
  Program *Input;
  int r;
  Init();
  Repeat:
    Input = Read(0);
    if (Input) 
    {
      Desugar(Input);
      Eval(Input);
      write(Out, "\r\n", 2);
      //Print(Result);
      //Delete(Input);
      //Discard(Result);
      Input = 0;
    }
    else
    {
      if (Buffer) free(Buffer);
      Buffer = 0;
      BufferLength = 0;
      BufferSize = 0;
      Pos = 0;
      write(Out, "\r\n", 2);
    }
    Nest = realloc(Nest, sizeof(char) * 0);
    Depth = 0;
    goto Repeat;
    return 0;
}

void SetRawMode() {
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
	ioctl(Out, TIOCGWINSZ, &Window);
	signal(SIGWINCH, WinSizeCh);
	//write(Out, "\x1b[2J\x1b[H", 7);
	write(Out, "\r\n", 2);
	write(Out, InfoString, sizeof(InfoString));
  write(Out, "\r\n", 2);
  write(Out, Target, sizeof(Target));
	write(Out, "\r\n", 2);
}

void Exit() {
	//if (Buffer != 0) {free(Buffer);}
	tcsetattr(0, TCSANOW, &restore);
	exit(0);
}

void Echo() 
{
	write(Out, "\x1b[2K\r", 5);
	char *str;
	if (Offset > Window.ws_col) 
  {
    str = 0;
		str = malloc(sizeof(char) * (Window.ws_col - 1));
		if (str) strncpy(str, &Buffer[BufferLength - (Window.ws_col - 1)], Window.ws_col - 1);
		else Exit();
    write(Out, str, Window.ws_col - 1);
	}
	else 
  {
    str = 0;
		str = malloc(sizeof(char) * (Window.ws_col - 1));
		if (str) strncpy(str, &Buffer[BufferLength - Offset], Offset);
    else Exit();
		write(Out, str, Offset);
	}
	free(str);
}

void ShowHint() 
{
  write(Out, "\x1b[90m", 5);
  for (Int n = Depth; n > 0; n--)
    switch(Nest[n-1])
    {
      case '(': write(Out, ")", 1); break;
      case '{': write(Out, "}", 1); break;
      case '[': write(Out, "]", 1); break;
      case '"': write(Out, "\"", 1); break;
    }
  write(Out, "\x1b[0m", 4);
}

Int IsDelimiter(char c) 
{
  switch(c) 
  {
    case '(':
		case '{':
		case '[':
		case ' ':
		case '\n':
		case ',':
		case ')':
		case '}':
		case ']':
			return 1;
		default:
			return 0;
  }
}

Int IsPunct(char c) 
{
	switch(c) 
  {
		case '(':
		case '{':
		case '[':
		case ' ':
		case '\n':
		case ',':
		case ')':
		case '}':
		case ']':
		case '\'':
			return 1;
		default:
			return 0;
	}
}

char PeekChar () 
{
  char c = NextChar();
  Pos--;
  return c;
}

char NextChar ()
{
  char c[128];
  Int r = 0;
  if (BufferLength == 0 || Pos >= BufferLength)
  {
    BufferLength = 0;
    Pos = 0;
    BufferSize = 0;
    Buffer = realloc(Buffer, sizeof(char) * BufferSize);
    for (;;)
    {
      if (r = read(In, c, 128), r)
      {
        if (r > 1) continue;
        if (c[0] == 27) Exit();
        if (c[0] == 8 || c[0] == 127)
        {
          if (Offset > 0 && BufferLength > 0)
            Offset--, BufferLength--, Echo();
          continue;
        }
        if (c[0] > 9 && c[0] < 14) 
        {
          BufferLength++;
          if (BufferLength >= BufferSize)
            BufferSize += 1024, Buffer = realloc(Buffer, sizeof(char) * BufferSize);
          Buffer[BufferLength - 1] = '\n';
          write(Out, "\n\r", 2);
          Offset = 0;
          goto End;
        }
        if (c[0] >= ' ') 
        {
          BufferLength++;
          if (BufferLength >= BufferSize)
            BufferSize += 1024, Buffer = realloc(Buffer, sizeof(char) * BufferSize);
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

void Append(Program *p, Program *q)
{
  p->Size ++;
  if (p->Collection)
    p->Collection = realloc(p->Collection, sizeof(Program*) * p->Size);
  else
    p->Collection = malloc(sizeof(Program*) * p->Size);
  q->Parent = p;
  p->Collection[p->Size -  1] = q; 
}

Program *Read(Int Quoted)
{
  Program *p = 0, *tmp = 0;
  p = malloc(sizeof(Program));
  p->Size = 0;
  p->Collection = 0;
  TryAgain:
  char c = NextChar();
  switch(c)
  {
    case '(': 
      p->Type = _Expression;
      goto ReadExpression;
    case '{':
      p->Type = _Collection;
      goto ReadExpression;
    case '[':
      p->Type = _Type;
      goto ReadExpression;
    case '\'':
      p->Type = _Quote;
      goto ReadQuote;
    case '\n':
      ShowHint();
    case ' ':
      goto TryAgain;
      break;
    case '"':
      p->Type = _String;
      goto ReadString;
      break;
    case ')': case '}': case ']':
      write(Out, "Unexpected closing brace.", 25);
      goto Error;
    case ',':
      write(Out, "Unexpected sequence delimiter.", 30);
      goto Error;
    default:
      p->Type = _Symbol;
      goto ReadSymbol;
  }
  ReadExpression:
  Depth ++, Nest = realloc(Nest, sizeof(char) * Depth), Nest[Depth - 1] = c;
  for (;;)
  {
    c = PeekChar();
    switch(c)
    {
      case '\n':
        ShowHint();
      case ' ':
        NextChar();
        break;
      case ',':
        if (p->Type != _Expression && p->Type != _Sequence)
        {
          write(Out, "Cannot construct a sequence here.", 33);
          goto Error;
        }
        p->Type = _Sequence;
        tmp = malloc(sizeof(Program));
        tmp->Type = _Symbol;
        tmp->Size = 1;
        tmp->Symbol = malloc(sizeof(char));
        tmp->Symbol[0] = ',';
        Append(p, tmp);
        NextChar();
        break;
      case ')':
        NextChar();
        if (p->Type == _Expression || p->Type == _Sequence)
        {
          Depth--;
          return p;
        }
        else 
        {
          write(Out, "Mismatched braces.", 18);
          goto Error;
        }
        break;
      case '}':
        NextChar();
        if (p->Type == _Collection)
        {
          Depth--;
          return p;
        }
        else
        {
          write(Out, "Mismatched braces.", 18);
          goto Error;
        }
        break;
      case ']':
        NextChar();
        if (p->Type == _Type)
        {
          Depth--;
          return p;
        }
        else
        {
          write(Out, "Mismatched braces.", 18);
          goto Error;
        }
        break;
      default:
        tmp = Read(Quoted);
        if (tmp) Append(p, tmp);
        else goto Error;
    }
  }
  ReadQuote:
    c = PeekChar();
    if (c == '\'')
    {
      if (!Quoted)
      {
        write(Out, "Cannot use '' or ''' outside a '.", 33);
        goto Error;
      }
      NextChar();
      c = PeekChar();
      if (c == '\'') p->Type = _Requote, NextChar();
      else p->Type = _Unquote;
      c = PeekChar();
      if (c == '\'')
      {
        write(Out, "'''' is not a valid syntax operator.", 36);
        goto Error;
      }
    }
    tmp = Read(1);
    if (tmp) 
    {
      Append(p, tmp);
      return p;
    }
    else goto Error;
  ReadString:
    Depth ++;
    Nest = realloc(Nest, sizeof(char) * Depth);
    Nest[Depth - 1] = '"';
    tmp = malloc(sizeof(Program));
    tmp->Type = _Symbol;
    tmp->Size = 0;
    tmp->Symbol = malloc(sizeof(char) * 0);
    for (c = NextChar(); c != '"'; c = NextChar())
    {
      if (c == '\\')
      {
        c = PeekChar();
        if (c == '(')
        {
          Append(p, tmp);
          tmp = Read(Quoted);
          if (tmp) 
          {
            Append(p, tmp);
            tmp = malloc(sizeof(Program));
            tmp->Type = _Symbol;
            tmp->Size = 0;
            tmp->Symbol = malloc(sizeof(char) * 0);
          }
          else goto Error;
          continue;
        }
        else 
        {
          tmp->Size++;
          tmp->Symbol = realloc(tmp->Symbol, sizeof(char) * tmp->Size);
          tmp->Symbol[tmp->Size - 1] = c;
          NextChar();
        }
      }
      else 
      {
        tmp->Size++;
        tmp->Symbol = realloc(tmp->Symbol, sizeof(char) * tmp->Size);
        tmp->Symbol[tmp->Size - 1] = c;
      }
    }
    Append(p, tmp);
    Depth--;
    return p;
  ReadSymbol:
    p->Size = 1;
    p->Symbol = malloc(sizeof(char));
    p->Symbol[0] = c;
    for (c = PeekChar(); !IsDelimiter(c); c = PeekChar())
    {
      p->Size++;
      p->Symbol = realloc(p->Symbol, sizeof(char) * p->Size);
      p->Symbol[p->Size - 1] = NextChar();
    }
    return p;
  Error:
    if (p) Delete(p);
    if (tmp) Delete(tmp);
  return 0;
}

void Delete(Program *p)
{
  switch(p->Type) 
  {
		case _Symbol:
			if (p->Symbol)
				free(p->Symbol), p->Symbol = 0;
			break;
		case _Expression: case _Sequence: case _Collection: case _Type: case _Quote: case _Unquote: case _Requote:
			for (Int i = 0; i < p->Size; i++)
				Delete(p->Collection[i]);
			free(p->Collection);
			p->Collection = 0;
			p->Size = 0;
			break;
		default:
			break;
	}
}

void Init() 
{
  Image = malloc(sizeof(Unit));
  Image->Which = Collection;
    
}

void Desugar(Program *p)
{
  switch(p->Type)
  {
    case _Symbol:
      p->Type = _Number;
      for (Int n = 0, radix = 0; n < p->Size; n++)
        if (n < p->Size - 1 && (p->Symbol[n] == '+' || p->Symbol[n] == '-')) continue;
        else if ((p->Symbol[n] == '.' || p->Symbol[n] == '/') && !radix && n < p->Size - 1)
          if (p->Symbol[n - 1] >= '0' && p->Symbol[n - 1] <= '9')
          {
            radix = 1;
            continue;
          }
          else goto NaN;
        else if (p->Symbol[n] >= '0' && p->Symbol[n] <= '9') continue;
        else goto NaN;
        break;
        NaN: p->Type = _Symbol; break;
    case _Sequence:
      break;
    case _Collection:
      for (Int n = 0; n < p->Size; n++)
        Desugar(p->Collection[n]);
      p->Type = _Expression;
      p->Size++;
      p->Collection = realloc(p->Collection, sizeof(Program*) * p->Size);
      for (Int n = p->Size; n > 1; n--)
        p->Collection[n - 1] = p->Collection[n - 2];
      p->Collection[0] = malloc(sizeof(Program));
      p->Collection[0]->Type = _Symbol;
      p->Size = 6;
      p->Collection[0]->Symbol = malloc(sizeof(char) * 6);
      strncpy(p->Collection[0]->Symbol, "fields", 6);
      p->Collection[0]->Parent = p;
      break;
    case _Type:
     for (Int n = 0; n < p->Size; n++)
        Desugar(p->Collection[n]);
      p->Type = _Expression;
      p->Size++;
      p->Collection = realloc(p->Collection, sizeof(Program*) * p->Size);
      for (Int n = p->Size; n > 1; n--)
        p->Collection[n - 1] = p->Collection[n - 2];
      p->Collection[0] = malloc(sizeof(Program));
      p->Collection[0]->Type = _Symbol;
      p->Size = 7;
      p->Collection[0]->Symbol = malloc(sizeof(char) * 7);
      strncpy(p->Collection[0]->Symbol, "members", 7);
      p->Collection[0]->Parent = p;
      break;
    case _Quote:
      for (Int n = 0; n < p->Size; n++)
        Desugar(p->Collection[n]);
      p->Type = _Expression;
      p->Size++;
      p->Collection = realloc(p->Collection, sizeof(Program*) * p->Size);
      for (Int n = p->Size; n > 1; n--)
        p->Collection[n - 1] = p->Collection[n - 2];
      p->Collection[0] = malloc(sizeof(Program));
      p->Collection[0]->Type = _Symbol;
      p->Size = 5;
      p->Collection[0]->Symbol = malloc(sizeof(char) * 5);
      strncpy(p->Collection[0]->Symbol, "quote", 5);
      p->Collection[0]->Parent = p;
      break;
    case _Unquote:
     for (Int n = 0; n < p->Size; n++)
        Desugar(p->Collection[n]);
      p->Type = _Expression;
      p->Size++;
      p->Collection = realloc(p->Collection, sizeof(Program*) * p->Size);
      for (Int n = p->Size; n > 1; n--)
        p->Collection[n - 1] = p->Collection[n - 2];
      p->Collection[0] = malloc(sizeof(Program));
      p->Collection[0]->Type = _Symbol;
      p->Size = 7;
      p->Collection[0]->Symbol = malloc(sizeof(char) * 7);
      strncpy(p->Collection[0]->Symbol, "unquote", 7);
      p->Collection[0]->Parent = p;
      break;
    case _Requote:
     for (Int n = 0; n < p->Size; n++)
        Desugar(p->Collection[n]);
      p->Type = _Expression;
      p->Size++;
      p->Collection = realloc(p->Collection, sizeof(Program*) * p->Size);
      for (Int n = p->Size; n > 1; n--)
        p->Collection[n - 1] = p->Collection[n - 2];
      p->Collection[0] = malloc(sizeof(Program));
      p->Collection[0]->Type = _Symbol;
      p->Size = 7;
      p->Collection[0]->Symbol = malloc(sizeof(char) * 7);
      strncpy(p->Collection[0]->Symbol, "requote", 7);
      p->Collection[0]->Parent = p;
      break;
    case _Expression:
      for (Int n = 0; n < p->Size; n++)
        Desugar(p->Collection[n]);
      break;
    case _String:
      Program *tmp;
      if (p->Size == 1 && p->Collection[0]->Type == _Symbol)
      {
        tmp = p->Collection[0];
        free(p->Collection);
        p->Size = tmp->Size;
        p->Symbol = tmp->Symbol;
        p->Type = _String;
      }
      else
      {
        tmp = malloc(sizeof(Program));
        tmp->Size = p->Size + (p->Size - 1);
        tmp->Type = _Expression;
        tmp->Parent = p->Parent;
        tmp->Collection = malloc(sizeof(Program*) * tmp->Size);
        for (Int n = 0, m = 0; n < p->Size; n++, m++)
        {
          tmp->Collection[m] = p->Collection[n];
          if (tmp->Collection[m]->Type == _Symbol)
            tmp->Collection[m]->Type = _String;
          if (n < p->Size - 1)
          {
            m++;
            tmp->Collection[m] = malloc(sizeof(Program));
            tmp->Collection[m]->Type = Symbol;
            tmp->Collection[m]->Size = 1;
            tmp->Collection[m]->Symbol = malloc(sizeof(char));
            tmp->Collection[m]->Symbol[0] = '+';
            tmp->Collection[m]->Parent = tmp;
          }
        }
        free(p->Collection);
        p->Collection = tmp->Collection;
        p->Size = tmp->Size;
        p->Type = tmp->Type;
      }
      free(tmp);
      break;
  }
}

void Eval(Program *p)
{
  
}

void Print()
{

}

void Discard(Unit *n)
{

}