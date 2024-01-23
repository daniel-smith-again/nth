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

typedef struct Program__ {
	enum {Symbol, Expression, Sequence, Collection, Type, Quote, Insert, Spread} type;
	union{
		char* symbol;
		struct Program__ **collection;
	};
	Int size;
	struct Program__ *parent;
} Program;

Program *Parse();
Program *Eval(Program *p);
void Print(Program *r);
void Discard(Program *p);

char *Buffer;
Int BufferLength;
Int BufferSize;
Int Pos;
Int Offset;
void BufferAppend(char c) {
	BufferLength++;
	if (BufferLength >= BufferSize) {
		BufferSize += 1028;
		Buffer = realloc(Buffer, sizeof(char) * BufferSize);
	}
	Buffer[BufferLength - 1] = c;
}

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
void SetRawMode();
void Exit();

int main () {
	atexit(Exit);
	SetRawMode();

	BufferLength = 0;
	BufferSize = 1028;
	Buffer = malloc(sizeof(char) * BufferSize);
	Offset = 0;

	Program *Input;

	char c[128];
	Int r;

	Repeat:
	r = read(In, c, sizeof(c));
	if (r == 1) {
		switch(c[0]) {
			case 27: //escape key
				exit(0); break;
			case 10:
			case 11:
			case 12:
			case 13: //all the ways to line break
				BufferAppend(' ');
				write(Out, "\r\n", 2);
				Offset = 0;
				Input = Parse(0);
				Input;
				Eval(Input);
				Discard(Input);
				break;
			case 8: //backspace
			case 127: //delete
				if (Offset > 0) {
					Offset--;
					BufferLength --;
					Echo();
				}
				break;
			default:
				BufferAppend(c[0]);
				Offset++;
				Echo();
				break;
		}
	}
	else {

	}
	goto Repeat;
	return 0;
}

void SetRawMode() {
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

	write(Out, "\x1b[2J\x1b[H", 7);
	write(Out, InfoString, sizeof(InfoString));
	write(Out, "\r\n", 1);
}

void Exit() {
	if (Buffer != 0) {free(Buffer);}
	tcsetattr(0, TCSANOW, &restore);
}

void Discard(Program *p) {
	switch(p->type) {
		case Symbol:
			free(p->symbol);
			break;
		case Expression:
		case Sequence:
		case Collection:
		case Type:
		case Quote:
		case Insert:
		case Spread:
			for (Int i = 0; i < p->size; i++) {
				Discard(p->collection[i]);
			}
			free(p->collection);
			break;
		default:
			break;
	}
}

Program *Parse() {
	Int Quoted = 0;
	char *Nest;
	Int Level = 0;
	Program *top;
	Program *tmp;
	Program *p;
	top = malloc(sizeof(Program));
	p = top;
	p->type = Expression;
	p->size = 0;
	p->parent = 0;
	for (Int i = 0; i < BufferLength; i ++) {
		switch(Buffer[i]) {
			case '(':
				tmp = malloc(sizeof(Program));
				tmp->type = Expression;
				tmp->size = 0;
				tmp->parent = p;
				p->size++;
				p->collection = realloc(p->collection, sizeof(Program*) * p->size);
				p->collection[p->size - 1] = tmp;
				p = tmp;
				break;
			case '{':
				tmp = malloc(sizeof(Program));
				tmp->type = Collection;
				tmp->size = 0;
				tmp->parent = p;
				p->size++;
				p->collection = realloc(p->collection, sizeof(Program*) * p->size);
				p->collection[p->size - 1] = tmp;
				p = tmp;
				break;
			case '[':
				tmp = malloc(sizeof(Program));
				tmp->type = Type;
				tmp->size = 0;
				tmp->parent = p;
				p->size++;
				p->collection = realloc(p->collection, sizeof(Program*) * p->size);
				p->collection[p->size - 1] = tmp;
				p = tmp;
				break;
			case ')':
				if (Level == 0 || Nest[Level - 1] != '(') {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return 0;
				}
				else 
					goto CloseExpression;
			case '}':
				if (Level == 0 || Nest[Level - 1] != '{') {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return 0;
				}
				else
					goto CloseExpression;
			case ']':
				if (Level == 0 || Nest[Level - 1] != '[') {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return 0;
				}
				CloseExpression:
				p = p->parent;
				if (p->type == Sequence)
					p = p->parent;
				Level --;
				break;
			case '\'':
				break;
				if (i < BufferLength - 4 && 
					Buffer[i + 1] == '\'' &&
					Buffer[i + 2] == '\'' &&
					Buffer[i + 3] == '\'') {
						write(Out, "\r\n?\r\n", 5);
						Discard(top);
						return 0;
					} 
			case ' ':
				break;
			case ',':
				if (i == 0 || p->parent->type != Sequence || p->type != Expression) {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return 0;
				}
				else if (p->parent->type = Sequence) {
					p->parent->size++;
					p->parent->collection = realloc(p->parent->collection, sizeof(Program*) * p->parent->size);
					p->parent->collection[p->parent->size - 1] = p;
					tmp = malloc(sizeof(Program));
					tmp->parent = p->parent;
					tmp->size = 0;
					p = tmp;
				}
				else if (p->type == Expression) {
					tmp = malloc(sizeof(Program));
					tmp->type = Sequence;
					tmp->size = 2;
					tmp->parent = p->parent;
					tmp->collection = malloc(sizeof(Program*) * tmp->size);
					tmp->collection[0] = p;
					p->parent = tmp;
					p = malloc(sizeof(Program));
					p->parent = tmp;
					tmp->collection[1] = p;
				}
				break;
			default:
				tmp = malloc(sizeof(Program));
				tmp->type = Symbol;
				tmp->parent = p;
				for (tmp->size = 1; tmp->size + i < BufferLength; tmp->size ++) {
					switch(Buffer[i + tmp->size - 1]) {
						case '(': case '{': case '[': case ')': case '}': case ']': case ' ': case ',':
							goto Break;
					}
				}
				Break:
				tmp->symbol = malloc(sizeof(char) * tmp->size);
				for (Int n = 0; n < tmp->size; n++)
					tmp->symbol[n] = Buffer[i + n];
				i += tmp->size;
				tmp->parent = p;
				p->size++;
				p->collection = realloc(p->collection, sizeof(Program*) * p->size);
				p->collection[p->size - 1] = tmp;
				break;
		}
	}
	free(Buffer);
	Buffer = 0;
	BufferLength = 0;
	BufferSize = 0;
	return top;
}

Program *Eval(Program *p) {
	switch(p->type) {
		case Symbol:
			write(Out, p->symbol, p->size);
			break;
		case Expression:
			write(Out, "(", 1);
			for (Int n = 0; n < p->size; n++) {
				Eval(p->collection[n]);
			}
			write(Out, ")", 1);
			break; 
		case Sequence:
			write(Out, "(", 1);
			for (Int n = 0; n < p->size; n++) {
				Eval(p->collection[n]);
				write(Out, ",", 1);
			}
			write(Out, ")", 1);
		case Collection:
			write(Out, "{", 1);
			for (Int n = 0; n < p->size; n++) {
				Eval(p->collection[n]);
			}
			write(Out, "}", 1);
		case Type:
			write(Out, "[", 1);
			for (Int n = 0; n < p->size; n++) {
				Eval(p->collection[n]);
			}
			write(Out, "]", 1);
	}
	Program *r;
	return r;
}

void Print(Program *p) {

}
/*




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
							break;
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
							if (Inner == 0 || Nest[Inner - 1] != '(' || Buffer[BufferLength - 1] == ',' || Buffer[BufferLength - 1] == '(') {
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



	/*
	Program *node;
	Program *tmp;
	if (Bi >= BufferLength) return 0;
	node = malloc(sizeof(Program));
        StartOver:
	switch(Buffer[Bi]) {
		case '(':
                        node = 0;
			tmp->type = Expression;
			tmp->size = 0;
			while(Buffer[Bi] != ')') {
				if (Bi < BufferLength) Bi++;
				else break;
                                if (Buffer[Bi] == ',') {
                                        if (Bi < BufferLength) Bi++;
                                        if (node == 0) {
                                                node = malloc(sizeof(Program));
                                                node->type = Expression;
                                                node->size = 1;
                                                node->collection = malloc(sizeof(Program*));
                                                node->collection[0]->type = Symbol;
                                                node->collection[0]->size = 2;
                                                node->collection[0]->parent = node;
                                                node->collection[0]->data = malloc(sizeof(char) * 2);
                                                node->collection[0]->data[0] = 'd';
                                                node->collection[0]->data[1] = 'o';
                                        }
                                        node->size ++;
                                        node->collection = realloc(node->collection, sizeof(Program*) * node->size);
                                        node->collection[node->size - 1] = tmp;
                                        tmp = malloc(sizeof(Program));
                                        tmp->type = Expression;
                                        tmp->size = 0;
                                }
				tmp->size ++;
				tmp->collection = realloc(tmp->collection, sizeof(Program*) * tmp->size);
				tmp->collection[tmp->size - 1] = Parse();
				tmp->collection[tmp->size - 1]->parent = tmp;
                        }
                        if (node == 0) {node = tmp;}
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
                        if (Bi < BufferLength) Bi++;
			node->type = Expression;
			node->size = 2;
			node->collection = malloc(sizeof(Program*) * node->size);
			tmp = malloc(sizeof(Program));
			tmp->type = Symbol;
			tmp->size = 5;
			tmp->data = malloc(sizeof(char) * tmp->size);
			tmp->data[0] = 'q', tmp->data[1] = 'u', tmp->data[2] = 'o', tmp->data[3] = 't', tmp->data[4] = 'e';
			tmp->parent = node;
			node->collection[node->size - 2] = tmp;
                        node->collection[node->size - 1] = Parse();
		case ' ':
                        if (Bi < BufferLength) Bi++;
                        goto StartOver;
                        break;
		case ',':
                        if (Bi < BufferLength) Bi++;
                        goto StartOver;
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
*/