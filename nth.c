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
Int Offset;
char *Nest;
Int Inner;



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
				if (Inner == 0 && BufferLength > 0) {Eval();}
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
									write(Out, c, 1);
									if (Inner == 0) {Eval();}
									goto Repeat;
								
								}
								else {
									goto Repeat;
								}
							}
							else {
								goto Repeat;
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

typedef struct prog {
	enum {Symbol, Expression} type;
	union {
		struct {char *data; Int length;} symbol;
		struct {struct prog *datalist; Int length;} expression;
	};
	struct prog *parent;
} Program;

void ActuallyEval(Program *P);

void Eval() {

	Program *program = NULL;
	Program *node = NULL;
	Int n = 0;
	Int doflag = 0;
	Int nesting = 0;
	Program *np;
	char* tmp;
	for (Int n = 0; n < BufferLength; n ++) {
		switch(Buffer[n]) {
			case '(':
				np = malloc(sizeof(Program));
				np->type = Expression;
				np->expression.datalist = NULL;
				np->expression.length = 0;
				if (program == NULL) {
					program = np;
					node = program;
					node->parent = NULL;
				}
				else {
					np->parent = node;
					node->expression.length++;
					node->expression.datalist = realloc(node->expression.datalist, sizeof(Program) * (node->expression.length));
					node->expression.datalist[node->expression.length - 1] = *np;
				}
				node = np;
				nesting++;
				break;
			case '{':
				np = malloc(sizeof(Program));
				np->type = Expression;
				np->expression.datalist = malloc(sizeof(Program));
				np->expression.length = 1;
				np->expression.datalist[0].type = Symbol;
				np->expression.datalist[0].parent = np;
				np->expression.datalist[0].symbol.data = malloc(sizeof(char) * 6);
				tmp = np->expression.datalist[0].symbol.data;
				tmp[0] = 'f', tmp[1] = 'i', tmp[2] = 'e', tmp[3] = 'l', tmp[4] = 'd', tmp[5] = 's';
				np->expression.datalist[0].symbol.length = 6;
				if (program == NULL) {
					program = np;
					node = program;
					node->parent = NULL;
				}
				else {
					np->parent = node;
					node->expression.length++;
					node->expression.datalist = realloc(node->expression.datalist, sizeof(Program) * (node->expression.length));
					node->expression.datalist[node->expression.length - 1] = *np;
				}
				node = np;
				nesting++;
				break;
			case '[':
				np = malloc(sizeof(Program));
				np->type = Expression;
				np->expression.datalist = malloc(sizeof(Program));
				np->expression.length = 1;
				np->expression.datalist[0].type = Symbol;
				np->expression.datalist[0].parent = np;
				np->expression.datalist[0].symbol.data = malloc(sizeof(char) * 7);
				tmp = np->expression.datalist[0].symbol.data;
				tmp[0] = 'm', tmp[1] = 'e', tmp[2] = 'm', tmp[3] = 'b', tmp[4] = 'e', tmp[5] = 'r', tmp[6] = 's';
				np->expression.datalist[0].symbol.length = 7;
				if (program == NULL) {
					program = np;
					node = program;
					node->parent = NULL;
				}
				else {
					np->parent = node;
					node->expression.length++;
					node->expression.datalist = realloc(node->expression.datalist, sizeof(Program) * (node->expression.length));
					node->expression.datalist[node->expression.length - 1] = *np;
				}
				node = np;
				nesting++;
				break;
			case '\'':
				np = malloc(sizeof(Program));
				np->type = Expression;
				np->expression.datalist = malloc(sizeof(Program));
				np->expression.length = 1;
				np->expression.datalist[0].type = Symbol;
				np->expression.datalist[0].parent = np;
				np->expression.datalist[0].symbol.data = malloc(sizeof(char) * 5);
				tmp = np->expression.datalist[0].symbol.data;
				tmp[0] = 'q', tmp[1] = 'u', tmp[2] = 'o', tmp[3] = 't', tmp[4] = 'e';
				np->expression.datalist[0].symbol.length = 5;
				if (program == NULL) {
					program = np;
					node = program;
					node->parent = NULL;
				}
				else {
					np->parent = node;
					node->expression.length++;
					node->expression.datalist = realloc(node->expression.datalist, sizeof(Program) * (node->expression.length));
					node->expression.datalist[node->expression.length - 1] = *np;
				}
				node = np;
				nesting++;
				break;
			case ')':
			case '}':
			case ']':
				node = node->parent;
				if (nesting > 0) nesting--;
				break;
			case ' ':
                                //space doesn't do anything aside from delimiting two symbols.
                                //can be safely discarded here.
				break;
			case ',':
				//do this thing here
                                /*
                                i need a way to go from (a, b, c) to (do (a) (b) (c))
                                but the tricky bit is i can't use backtracking
                                I have to insert expressions as i go
                                */
			default:
				Int sym_length = 1;
				for (Int i = n; i < BufferLength; i++) {
					switch(Buffer[i]) {
						case '(': case'{': case '[': case ']': case '}': case ')': case ' ': case ',':
							break;
						default: 
							sym_length++;
							break;
					}
				}
				char *str = malloc(sizeof(char) * sym_length);
				for (Int i = 0; i < sym_length; i++) {
					str[i] = Buffer[n+i];
				}
				np = malloc(sizeof(Program));
				np->type = Symbol;
				np->symbol.data = str;
				np->symbol.length = sym_length;

				if (program == NULL) {
					program = np;
					node = program;
					node->parent = NULL;
				}
				else {
					np->parent = node;
					node->expression.length++;
					node->expression.datalist = realloc(node->expression.datalist, sizeof(Program) * (node->expression.length));
					node->expression.datalist[node->expression.length - 1] = *np;
				}
				n += sym_length;
				break;
		}
	}
	if (nesting == 0 && program != NULL) {
		ActuallyEval(program);
		free(program);
		program = NULL;
	}
	else write(Out, "?\r\n", 3);

	free(Buffer);
	free(Nest);
	BufferSize = 0;
	BufferLength = 0;
	Inner = 0;
	Buffer = malloc(sizeof(char) * 1028);
	BufferSize = 1028;
	Nest = malloc(sizeof(char));
}

void ActuallyEval(Program *P) {
	write(Out, "\n\rResult\n\r", 10);
}