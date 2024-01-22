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
Int Quoted;
typedef struct prog {
	enum {Symbol, Expression, Quote, Insert, Spread} type;
	union {
		char *data;
		struct prog **collection;
	};
	Int size;
	struct prog *parent;
} Program;
Program *Input;
Int Bi;
Program *Parse();
Program *Eval();
void Print(Program *data);

struct termios raw, restore;
struct winsize Window; //fields: ws_row, ws_col
void WinSizeCh (int signum) {
	ioctl(Out, TIOCGWINSZ, &Window);
}
void Echo() {
	write(Out, "\x1b[2K\r", 5);
	if (Offset > Window.ws_col) {
		write(Out, &Buffer[BufferLength - Window.ws_col], Window.ws_col);
	}
	else {
		write(Out, &Buffer[BufferLength - Offset], Offset);
	}
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
	Quoted = 0;

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
				Offset = 0;
				write(Out, "\r\n", 2);
				if (Inner == 0 && BufferLength > 0) {
					Input = Parse();
					Input;
				}
				else if (BufferLength > 0) {
					BufferAppend(' ');
				}
				Echo();
				break;
			case 8:
			case 127: //backspace
				if (Offset > 0 && BufferLength > 0) {
					BufferLength--;
					Offset--;
					Echo();
				}
				break;
			default:
				if (c[0] >= ' ') {
					switch(c[0]) {
						case '(':
						case '[':
						case '{':
							Inner++;
							Nest = realloc(Nest, sizeof(char) * Inner);
							Nest[Inner-1] = c[0];
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
									if (Nest[Inner - 1] == '\'') {
										Inner--;
										Quoted = 0;
									}								
								}
								else {
									goto Repeat;
								}
							}
							else {
								goto Repeat;
							}
							break;
						case '\'':
							if (Quoted == 0) {
								Quoted = 1;
								Inner++;
								Nest = realloc(Nest, sizeof(char) * Inner);
								Nest[Inner-1] = c[0];
							}
							else {
								if (Buffer[BufferLength - 1] == '\'' && Nest[Inner - 1] == '\'') {
									goto Repeat;
								}
								else if (BufferLength > 4) {
									if ((Buffer[BufferLength - 2] == '\'') &&
									    (Buffer[BufferLength - 3] == '\'')) {
										switch(Buffer[BufferLength - 4]) {
											case '(': case '{': case '[': case ')': case '}': case ']': case ' ': case ',':
												goto Repeat;
										}
									    }
								}
							}
							break;
						case ',':
							if (Inner == 0 || Nest[Inner - 1] != '(') {
								goto Repeat;
							}
					}
					BufferAppend(c[0]);
					if (Inner == 0 && BufferLength > 0) {
						//Input = Parse(0);
					}
					Offset ++;
					Echo();
				}
		}
		Repeat:
	}
	else {
		if (c[1] == '[') {
			//do fancy editing here
			//fill in later
		}
	}
	goto ReadInput;
	Exit:
	if (Buffer != NULL) free(Buffer);
	if (Nest != NULL) free(Nest);
	tcsetattr(0, TCSANOW, &restore);
	return 0;
}

Program *Parse() {
	write(Out, "PARSING", 7);
	Program *node;
	Program *tmp;
	if (Bi >= BufferLength) return 0;
	node = malloc(sizeof(Program));
	switch(Buffer[Bi]) {
		case '(':
			node->type = Expression;
			node->size = 0;
			while(Buffer[Bi] != ')') {
				if (Bi < BufferLength) Bi++;
				else break;
				node->size ++;
				node->collection = realloc(node->collection, sizeof(Program*) * node->size);
				node->collection[node->size - 1] = Parse();
				node->collection[node->size - 1]->parent = node;
			}
			break;
		case '{':
			node->type = Expression;
			node->size = 1;
			node->collection = malloc(sizeof(Program*) * node->size);
			tmp = malloc(sizeof(Program));
			tmp->type = Symbol;
			tmp->size = 6;
			tmp->data = malloc(sizeof(char) * tmp->size);
			tmp->data[0] = 'f', tmp->data[1] = 'i', tmp->data[2] = 'e', tmp->data[3] = 'l', tmp->data[4] = 'd', tmp->data[5] = 's';
			tmp->parent = node;
			node->collection[node->size - 1] = tmp;
			while(Buffer[Bi] != '}') {
				if (Bi < BufferLength) Bi++;
				else break;
				node->size ++;
				node->collection = realloc(node->collection, sizeof(Program*) * node->size);
				node->collection[node->size - 1] = Parse();
				node->collection[node->size - 1]->parent = node;
			}
			break;
		case '[':
			node->type = Expression;
			node->size = 1;
			node->collection = malloc(sizeof(Program*) * node->size);
			tmp = malloc(sizeof(Program));
			tmp->type = Symbol;
			tmp->size = 7;
			tmp->data = malloc(sizeof(char) * tmp->size);
			tmp->data[0] = 'm', tmp->data[1] = 'e', tmp->data[2] = 'm', tmp->data[3] = 'b', tmp->data[4] = 'e', tmp->data[5] = 'r', tmp->data[6] = 's';
			tmp->parent = node;
			node->collection[node->size - 1] = tmp;
			while(Buffer[Bi] != ']') {
				if (Bi < BufferLength) Bi++;
				else break;
				node->size ++;
				node->collection = realloc(node->collection, sizeof(Program*) * node->size);
				node->collection[node->size - 1] = Parse();
				node->collection[node->size - 1]->parent = node;
			}
			break;
		case '\'':
			node->type = Expression;
			node->size = 1;
			node->collection = malloc(sizeof(Program*) * node->size);
			tmp = malloc(sizeof(Program));
			tmp->type = Symbol;
			tmp->size = 5;
			tmp->data = malloc(sizeof(char) * tmp->size);
			tmp->data[0] = 'q', tmp->data[1] = 'u', tmp->data[2] = 'o', tmp->data[3] = 't', tmp->data[4] = 'e';
			tmp->parent = node;
			node->collection[node->size - 1] = tmp;
		case ' ':
		case ',':
			break;
		default:
			node->type = Symbol;
			node->size = 0;
			node->collection = 0;
			for (Int i = Bi; i < BufferLength; i++) {
				switch(Buffer[i]) {
					case '(': case '{': case '[': case ')': case '}': case ']': case ',': case ' ':
						goto LoopEnd;
					default:
						node->size ++;
				}
			}
			LoopEnd:
			node->data = malloc(sizeof(char) * node->size);
			for (Int i = 0; i < node->size; i++)
				node->data[i] = Buffer[Bi + i];
			Bi += node->size;
	}
	return node;
}