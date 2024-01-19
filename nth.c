#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define In STDIN_FILENO
#define Out STDOUT_FILENO

char * Buffer;
unsigned long long int BufferLength;
unsigned long long int BufferSize;

//this is for advanced editing. 
//leave it for later...
typedef struct {char *line; unsigned long long int length;} Line;
Line *Lines;
unsigned long long int Row;
unsigned long long int Column;

struct termios raw, restore;

int main () {

	tcgetattr(0, &restore);
	raw = restore;
	raw.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	raw.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	raw.c_cflag &= ~(CSIZE | PARENB);
	raw.c_cflag |= CS8;
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &raw);

	char c[128];
	unsigned long long int r;
	Buffer = malloc(sizeof(char) * 1028);
	BufferLength = 0;
	BufferSize = 1024;

	Lines = malloc(sizeof(Line)); //for sanity, but don't touch yet
	Lines[0].line = &(Buffer[0]); //ditto
	Lines[0].length = 0; //ddiittttoo

	write(Out, "\x1b[2J\x1b[H", 7);

	ReadInput:
	r = read(In, c, 128);
	if (r == 1) {
		if (c[0] < ' ') {
			switch (c[0]) {
				case 27: //esc 
					goto Exit; break;
				case 8: //backspace
				case 127: //delete
					write(Out, "!", 1);
					if (BufferLength > 0)
						BufferLength --;
					if (BufferLength < BufferSize / 2 && BufferLength > 1028) {
						Buffer = realloc(Buffer, sizeof(char) * ((BufferLength / 1028) + 1028));
					}
					break;
				case 10:
				case 12:
				case 13:
					c[0] = '\n';
					goto AppendCharacter; 
					break;
			}
		}
		else {
			AppendCharacter:
			BufferLength++;
			if (BufferLength >= BufferSize) {
				Buffer = realloc(Buffer, sizeof(char) * (BufferSize + 1028));
			}
			Buffer[BufferLength - 1] = c[0];
		}
		write(Out, "\x1b[H", 3);
		//write(Out, "\n", 1);
		write(Out, Buffer, BufferLength);
	}
	goto ReadInput;
	Exit:
	free(Buffer);
	tcsetattr(0, TCSANOW, &restore);
	return 0;
}
