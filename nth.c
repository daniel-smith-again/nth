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
#define InfoString \
"Welcome to the nth compiler utility.\r\n\
Copyright (C) Daniel Smith daniel.smith.again@gmail.com\r\n"

typedef unsigned long long int Int;

char *Buffer;
Int BufferSize;
Int BufferLength;
void BufferAppend(char c) {
	BufferLength++;
	if (BufferLength >= BufferSize) {
		BufferSize += 1028;
		Buffer = realloc(Buffer, sizeof(char) * BufferSize);
	}
	Buffer[BufferLength - 1] = c;
}
Int Offset; //you can only backspace the current terminal line you're on.
char *Nest;
Int Inner;
typedef struct prog {
	enum {Symbol, Expression} type;
	union {
		struct {char *data; Int length;} symbol;
		struct {struct prog *datalist; Int length;} expression;
	};
	struct prog *parent;
} Program;
Program *AST;
Int Bi;
Program *Parse(Program *parent);
Program *Eval();



void Print(Program *data);

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
	Int r;

	write(Out, "\x1b[2J\x1b[H", 7);
	//write(Out, "\x1b[107;30m", 9);
	write(Out, InfoString, sizeof(InfoString));
	//write(Out, "\n\r\x1b[0m", 6);
	write(Out, "\n", 1);

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
				if (Inner == 0 && BufferLength > 0) {
					Parse(0);
				}
				else if (BufferLength > 0) {
					BufferAppend(' ');
				}
				break;
			case 8:
			case 127: //backspace
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
									goto Repeat;
								}
							}
							else {
								goto Repeat;
							}
							break;
						case ',':
							if (Inner > 0 && Nest[Inner - 1] == '(') {
								BufferAppend(',');
							}
							else {
								goto Repeat;
							}
					}
					write(Out, c, 1);
					BufferAppend(c[0]);
					if (Inner == 0 && BufferLength > 0) {
						Parse(0);
					}
					Offset ++;
				}
		}
		Repeat:
	}
	else {
		if (c[1] == '[') {
		}
	}
	goto ReadInput;
	Exit:
	if (Buffer != NULL) free(Buffer);
	if (Nest != NULL) free(Nest);
	tcsetattr(0, TCSANOW, &restore);
	return 0;
}

Program *Parse(Program *parent) 
{
	if (parent == 0) {
		AST = malloc(sizeof(Program));
		Bi = 0;
	}
	switch(Buffer[Bi]) {
		case '(':
			break;
		case '{':
			break;
		case '[':
			break;
		case ')':
			break;
		case '}':
			break;
		case ']':
			break;
		case '\'':
			break;
		case ',':
			break;
		default:
			break;
	}
}
