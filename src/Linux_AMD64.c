#include "nth.h"

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

char *Buffer = 0;
Int BufferSize = 0;
Int BufferLength = 0;
Int Pos = 0;
Int Offset = 0;

char *Nest = 0;
Int Depth = 0;

void WinSizeCh(int signum)
{
  ioctl(Out, TIOCGWINSZ, &window);
}

void Exit()
{
  tcsetattr(0, TCSANOW, &restore);
  exit(0);
}

void StartShell() 
{
  atexit(Exit);
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
  return;
}

void ExitShell()
{
  Exit();
  return;
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

char PeekChar()
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

Program *CreateSymbol(char *atom, Int size)
{
  Program *tmp = malloc(sizeof(Program));
  tmp->size = size;
  tmp->atom = malloc(sizeof(char) * size);
  strncpy(tmp->atom, size, atom);
  tmp->type = symbol;
  return tmp;
}

void Append(Program *p, Program *q)
{
  if (p->type == expression)
  {
    p->size ++;
    p->list = realloc(p->list, sizeof(Program*) * p->size);
    p->list[p->size - 1] = q;
  }
}

Program *Read(Int Quoted)
{
    Program *p = 0, tmp = 0;
    p = malloc(sizeof(Program));
    p->size = 0;
    p->list = 0;
    char c = 0;
    Skip:
    c = NextChar();
    switch(c)
    {
      case '(':
        p->type = expression;
        goto ReadExpression;
      case '{':
        p->type = expression;
        Append(p, CreateSymbol("fields"));
        goto ReadExpression;
      case '[':
        p->type = expression;
        Append(p, CreateSymbol("members"));
        goto ReadExpression;
      case '\'':
        p->type = expression;
        goto ReadQuote;
      case '\n':
        ShowHint();
      case ' ':
        goto TryAgain;
      case '"':
        p->type = string;
        goto ReadString;
      case ')': case '}': case ']':
        write(Out, "Unexpected closing brace.", 25);
        goto Error;
      case ',':
        write(Out, "Unexpected sequence constructor.", 30);
        goto Error;
      default:
        p->type = symbol;
        goto ReadSymbol;
    }
}

void Print(Unit data)
{
  return;
}

Unit *Eval(Program *p)
{

}

void (*Compile())(Program* expr, Collection* scope, void* txt, Int txtl)
{
  return 0;
}