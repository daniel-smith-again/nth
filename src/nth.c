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
    struct {char *numerator; char*denominator;};
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
  Begin:
  program = Read();
  if (program)
    //Print(Compute(program);
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
#define Out STDOUT_FILENO
#define In STDIN_FILENO
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
    str = 0;
    str = malloc(sizeof(char) * (window.ws_col - 1));
    if (str) strncpy(str, &Buffer[BufferLength - (window.ws_col - 1)], window.ws_col - 1);
    else ExitShell();
    write(Out, str, window.ws_col - 1);
  }
  else
  {
    str = 0;
    str = malloc(sizeof(char) * (window.ws_col - 1));
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
    BufferLength = 0;
    Pos = 0;
    BufferSize = 1024;
    Buffer = realloc(Buffer, sizeof(char) * BufferSize);
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
          BufferLength++;
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
  }
  return;
}

#endif

Program *Read()
{
  Program *e = malloc(sizeof(Program)), *tmp = 0;
  char c = 0;
  char *str = 0;
  Int strl = 0, isNum = 0, Nmrl = 0, Dnml = 0;
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
    default:
      strl = 0, str = malloc(sizeof(char) * strl);
      for (c = PeekChar(); c != ' ' && c != '(' && c != ')' && c != '"' ; c = PeekChar())
      {
        strl++;
        str = realloc(str, sizeof(char) * strl);
        str[strl - 1] = c;
        ReadChar();
      }
      e->type = Symbol;
      e->size = strl;
      e->symbol = str;
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