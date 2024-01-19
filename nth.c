#include <stdio.h>
#include <stdlib.h>

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

char *Buffer;
unsigned long long int BufferSize;
unsigned long long int BufferLength;
unsigned long long int Offset;
char *Nest;
unsigned long long int Inner;

void Eval();
void Print();

struct termios raw, restore;
struct winsize Window; //fields: ws_row, ws_col
void WinSizeCh (int signum) {
	ioctl(Out, TIOCGWINSZ, &Window);
}

int main () {

	tcgetattr(0, &restore);
	raw = restore;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &raw);

	ioctl(Out, TIOCGWINSZ, &Window);
	signal(SIGWINCH, WinSizeCh);

	char c[128];
	unsigned long long int r;

	write(Out, "\x1b[2J\x1b[H", 7);

	Buffer = malloc(sizeof(char) * 1028);
	BufferSize = 1028;
	BufferLength = 0;
	Offset = 0;
	Inner = 0;
	Nest = 0;

	ReadInput:
	r = read(In, c, 128);
	if (r == 1) {
		switch(c[0]) {
			case 27: //escape key
				goto Exit;
				break;
			case 10:
			case 11:
			case 12:
			case 13:
				write(Out, "\x0a\x0d", 2);
				Offset = 0;
				if (Inner == 0) {Eval();}
				break;
			case 8:
			case 127:
				if (Offset > 0 && BufferLength > 0) {
					write(Out, "\b\x1b[K", 4);
					BufferLength--;
					Offset--;
				}
			default:
				if (c[0] >= ' ') {
					switch(c[0]) {
						case '(':
						case '[':
						case '{':
							Inner++;
							Nest = realloc(Nest, sizeof(char) * Inner);
							Nest[Inner-1] = c[0];
							//write(Out, Nest, Inner);
							break;
						case ')':
						case ']':
						case '}':
							if (Inner > 0) {
								if (
									(Nest[Inner-1] == '(' && c[0] == ')') ||
									(Nest[Inner-1] == '[' && c[0] == ']') ||
									(Nest[Inner-1] == '{' && c[0] == '}')
								) {
									Inner--;
								}
								else {
									goto NestingError;
								}
							}
							else {
								goto NestingError;
							}
							break;
					}
					write(Out, c, 1);
					BufferLength++;
					if (BufferLength >= BufferSize) {
						BufferSize += 1028;
						Buffer = realloc(Buffer, sizeof(char) * BufferSize);
						Buffer[BufferLength - 1] = c[0];
					}
					Offset ++;
				}
		}
		NestingError:
		//write(Out, c, 1);
	}
	else {
		if (c[1] == '[') {
		}
	}
	goto ReadInput;
	Exit:
	if (Buffer) free(Buffer);
	if (Nest) free(Nest);
	tcsetattr(0, TCSANOW, &restore);
	return 0;
}

void Eval() {
	free(Buffer);
	free(Nest);
	BufferSize = 0;
	BufferLength = 0;
	Inner = 0;
	Buffer = malloc(sizeof(char) * 1028);
	BufferSize = 1028;
}
