#include <stdlib.h>
#include <stdio.h>
#define InfoString \
"Welcome to the nth language programming utility\r\n\
Copyright (C) Daniel Smith daniel.smith.again@gmail.com"
typedef unsigned char Byte;
typedef long long unsigned int Word;
typedef Byte Bool;
typedef void* Place;
typedef unsigned int Character;

void* Read();
void *Compute(void *program);
void Print(void *data);

void StartShell();
void LineBreak();
void ExitShell();

void main()
{
  StartShell();
  void *p, *r;
  Loop:
    p = Read();
    if (p) r = Compute(p);
    else goto Err;
    if (r) Print(r);
    else goto Err;
    goto Loop;
  Err:
    if (p) free(p);
    if (r) free(r);
    goto Loop;
  ExitShell();
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
#include <dlfcn.h>
#include <elf.h>
#define In STDIN_FILENO
#define Out STDOUT_FILENO
typedef unsigned long long int Int;
struct termios raw, restore;
struct winsize window;
void WinSizeCh(int signum)
{
  ioctl(Out, TIOCGWINSZ, &window);
}

void LineBreak()
{
  write(Out, "\n\r", 2);
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

void *Read()
{
  char *Buffer;
  Int BufferSize;
  Int BufferLength = 0;
  Int Pos;
  Int Offset;
  void *program;
  void Echo()
  {
    char* str = 0;
    write(Out, "\x1b[2K\r", 5);
    if (Offset > window.ws_col)
    {
      str = 0, str = malloc(sizeof(char) * (window.ws_col - 1));
      if (str) 0;//strncpy(str, &Buffer[BufferLength - (window.ws_col - 1)], window.ws_col - 1);
      else ExitShell();
      write(Out, str, window.ws_col - 1);
    }
    else
    {
      str = 0, str = malloc(sizeof(char) * (window.ws_col - 1));
      if (str) 0;//strncpy(str, &Buffer[BufferLength - Offset], Offset);
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
  char PeekChar()
  {
    char c = ReadChar();
    Pos--;
    return c;
  }
  void *Construct()
  {
    char c = PeekChar();
  }
  program = malloc(0);
  program = Construct();
  return program;
}

void *Compute(void *program)
{

}

void Print(void *data)
{

}

#endif