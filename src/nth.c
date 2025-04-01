#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <locale.h>
#include <signal.h>
#include <stdbit.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#include <dirent.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <termios.h>
#include <unistd.h>

typedef unsigned char Byte;
typedef unsigned int Nat;
struct termios terminal_save, terminal_raw;
Byte *input_buffer;

Nat buffer_length;
Nat buffer_size;
Nat buffer_position;
#define bufferincrement 1024;


void setup_terminal() 
{
  tcgetattr(STDOUT_FILENO, &terminal_save);
  cfmakeraw();
}

void setup_program()
{
  input_buffer = malloc(sizeof(Byte) * bufferincrement);
  buffer_length = 0;
  buffer_size = 0 + bufferincrement;
}

void do_upon_exit(void) 
{
  tcsetattr(STDOUT_FILENO, 0, &terminal_save);
  free(input_buffer);
}

void fill_buffer() 
{
  Byte c[128];
  Nat count;
  while(count = read(STDIN_FILENO, c, 128), count > 0)
  {
    for (int i = 0; i < )
  }
}

void clear_buffer () 
{

}

/*
* reader state machine
*
* character is either a symbol
*/

Byte grab_char ()
{
  if (buffer_position >= buffer_length)
    fill_buffer();
  buffer_position++;
  return input_buffer[buffer_position - 1];
}

int main () 
{
  setup_terminal(), setup_program(), atexit(do_upon_exit);
}
