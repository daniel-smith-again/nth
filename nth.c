#include <stdio.h>
#include <stdlib.h>
/* linux specific includes */
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

unsigned long long int Expression_Count = 0;
struct termios ui, restore;
struct winsize window_size;

int main ()
{
	tcgetattr(0, &restore);
	ui = restore;
	ui.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	ui.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	ui.c_cflag &= ~(CSIZE | PARENB);
	ui.c_cflag |= CS8;
	tcsetattr(0, TCSANOW, &ui);
	ioctl(0, TIOCGWINSZ, &window_size);

	char c;
	char *buffer;
	unsigned long long int buffer_length = 0;
	unsigned long long int buffer_size = 0;
	unsigned long long int RBalance = 0, CBalance = 0, SBalance = 0;
	unsigned long long int quoted = 0;
FILL_BUFFER:
	while(read(0, &c, 1) > 0)
	{
		if (buffer_size == 0) buffer_size = 100, buffer = malloc(sizeof(char) * buffer_size);
		buffer_length ++;
		if (buffer_size <= buffer_length)
		{
			buffer_size = buffer_size * 2;
			char *tmp = malloc(sizeof(char) * buffer_size);
			for (int i = 0; i < buffer_length; i ++)
				tmp[i] = buffer[i];
			free(buffer);
			buffer = tmp;
		}
		switch(c) 
		{
			case '\n':
			case '\r':
			{
				if (RBalance == 0 && 
				    CBalance == 0 && 
				    SBalance == 0 &&
				    quoted == 0) 
				    	goto EXIT;
				else
				{
					c = '\n';
					write(1, "\n\r", 2);
					break;
				}
			}
			case 4:
			{
				goto EXIT;
				break;
			}
			case '(':
			{
				if (!quoted) RBalance++;
				break;
			}
			case '[':
			{
				if (!quoted) SBalance++;
				break;
			}
			case '{':
			{
				if (!quoted) CBalance++;
				break;
			}
			case ')':
			{
				if (!quoted) if (RBalance > 0) RBalance--;
				break;
			}
			case ']':
			{
				if (!quoted) if (SBalance > 0) SBalance--;
				break;
			}
			case '}':
			{
				if (!quoted) if (CBalance > 0) CBalance--;
				break;
			}
			case '\"':
			{
				if (buffer_length > 1 && buffer[buffer_length - 1] != '\\')
				{
					if (quoted) 
						quoted = 0;
					else
						quoted = 1;
				}
			}
			case 27:
			{
				do {
					read(0, &c, 1);
				} while (c < '@');
				c = 0;
				break;
			}

			case 8:
			case 127:
			{
				if (buffer[buffer_length] != '\n') buffer_length --;
				write(1, "\x1b[D\x1b[K", 6);
				c = 0;
				break;
			}			
		}
		if (c != 0) write(1, &c, 1), buffer[buffer_length] = c;
	}
EXIT:
	write(1, "\n", 1);
	tcsetattr(0, TCSANOW, &restore);
	exit(0);
	return 0;
}