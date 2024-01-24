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
	union{ char* symbol; struct Program__ **collection; };
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
	if (Offset > Window.ws_col)
		write(Out, &Buffer[BufferLength - Window.ws_col], Window.ws_col);
	else
		write(Out, &Buffer[BufferLength - Offset], Offset);
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
			case 27: exit(0); break;//escape key
			case 10: case 11: case 12: case 13: //all the ways to line break
				BufferAppend(' ');
				write(Out, "\r\n", 2);
				Offset = 0;
				Input = Parse(0);
				if ((Int)Input == 1) {
					free(Buffer);
					Buffer = 0;
					BufferLength = 0;
					BufferSize = 0;
				}
				else if (Input != 0) {
					write(Out, "\r\n", 2);
					Eval(Input);
					write(Out, "\r\n", 2);
					Discard(Input);
					Input = 0;
				}
				break;
			case 8: case 127:
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
	} else { } //in case I want to handle arrow keys
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
		case Expression: case Sequence: case Collection: case Type: case Quote: case Insert: case Spread:
			for (Int i = 0; i < p->size; i++)
				Discard(p->collection[i]);
			free(p->collection);
			break;
		default:
			break;
	}
}

Program *Parse() {
	Int Quoted = 0;
	char *Nest = malloc(sizeof(char) * 0);
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
				Level++;
				Nest = realloc(Nest, sizeof(char) * Level);
				Nest[Level - 1] = '(';
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
				Level++;
				Nest = realloc(Nest, sizeof(char) * Level);
				Nest[Level - 1] = '{';
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
				Level++;
				Nest = realloc(Nest, sizeof(char) * Level);
				Nest[Level - 1] = '[';
				break;
			case ')':
				if (Level == 0 || Nest[Level - 1] != '(') {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return (Program *)1;
				}
				else goto CloseExpression;
			case '}':
				if (Level == 0 || Nest[Level - 1] != '{') {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return (Program *)1;
				}
				else goto CloseExpression;
			case ']':
				if (Level == 0 || Nest[Level - 1] != '[') {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return (Program *)1;
				}
				CloseExpression:
				p = p->parent;
				if (p->type == Sequence) p = p->parent;
				Level --;
				if (Nest[Level - 1] == '\'') Level --, Quoted = 0;
				break;
			case '\'':
				Int QuoteSize = 1;
				for (; i + QuoteSize < BufferLength; QuoteSize++)
					if (Buffer[i + QuoteSize] != '\'') 
                                                break;
				i += (QuoteSize - 1);
				if (QuoteSize == 1) {
					Quoted = 1;
					Level++;
					Nest[Level - 1] = '\'';
					tmp = malloc(sizeof(Program));
					tmp->type = Quote;
				}
				else if (QuoteSize > 3 || Quoted == 0) {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return (Program*)1;
				}
				else {
					tmp = malloc(sizeof(Program));
					if (QuoteSize == 2) tmp->type = Insert;
					else tmp->type = Spread;
				}
				tmp->size = 0;
				tmp->parent = p;
				p->size++;
				p->collection = realloc(p->collection, sizeof(Program*) * p->size);
				p->collection[p->size - 1] = tmp;
				p = tmp;
				break;
			case ' ':
				break;
			case ',':
				if(Nest[Level-1] != '(') {
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return (Program*)1;
				}
				else if (p->parent->type == Sequence) {
					tmp = p->parent;
					tmp->size++;
					tmp->collection = realloc(tmp->collection, sizeof(Program*) * tmp->size);
					p = malloc(sizeof(Program));
					tmp->collection[tmp->size - 1] = p;
					p->parent = tmp;
					p->type = Expression;
					p->size = 0;				
				}
				else {
					tmp = malloc(sizeof(Program));
					tmp->type = Sequence;
					tmp->size = 2;
					tmp->parent = p->parent;
					p->parent->collection[p->parent->size - 1] = tmp;
					tmp->collection = malloc(sizeof(Program*) * tmp->size);
					tmp->collection[0] = p;
					p->parent = tmp;
					p = malloc(sizeof(Program));
					p->parent = tmp;
					p->type = Expression;
					tmp->collection[1] = p;
				}
				break;
			default:
				tmp = malloc(sizeof(Program));
				tmp->type = Symbol;
				tmp->parent = p;
				for (tmp->size = 1; tmp->size + i < BufferLength; tmp->size ++)
					switch(Buffer[i + tmp->size]) {
						case '(': case '{': case '[': case ')': case '}': case ']': case ' ': case ',':
							goto Break;
					}
				Break:
				tmp->symbol = malloc(sizeof(char) * tmp->size);
				for (Int n = 0; n < tmp->size; n++)
					tmp->symbol[n] = Buffer[i + n];
				i += (tmp->size - 1);
				tmp->parent = p;
				p->size++;
				p->collection = realloc(p->collection, sizeof(Program*) * p->size);
				p->collection[p->size - 1] = tmp;
				if (p->type == Quote || p->type == Insert || p->type == Spread) {
					if (Nest[Level - 1] == '\'' && p->type == Quote)
						Level --, Quoted = 0;
					p = p->parent;
				}
				break;
		}
	}
	
	if (Level > 0) {
		Discard(top);
		return 0;
	}
	else {
		free(Buffer);
		Buffer = 0;
		BufferLength = 0;
		BufferSize = 0;
		return top;
	}
} //complete parser in 327 lines no big deal or anything...

Program *Eval(Program *p) {
	if (p == 0) return 0;
	switch(p->type) {
		case Symbol:
			write(Out, " ", 1);
			write(Out, p->symbol, p->size);
			write(Out, " ", 1);
			break;
		case Expression:
			write(Out, "(", 1);
			for (Int n = 0; n < p->size; n++) Eval(p->collection[n]);
			write(Out, ")", 1);
			break; 
		case Sequence:
			write(Out, "(", 1);
			for (Int n = 0; n < p->size; n++) {
				Eval(p->collection[n]);
				if (n < p->size - 1)
					write(Out, ", ", 2);
			}
			write(Out, ")", 1);
			break;
		case Collection:
			write(Out, "{", 1);
			for (Int n = 0; n < p->size; n++) Eval(p->collection[n]);
			write(Out, "}", 1);
			break;
		case Type:
			write(Out, "[", 1);
			for (Int n = 0; n < p->size; n++) Eval(p->collection[n]);
			write(Out, "]", 1);
			break;
		case Quote:
			write(Out, "'", 1);
			Eval(p->collection[0]);
			break;
		case Insert:
			write(Out, "''", 2);
			Eval(p->collection[0]);
			break;
		case Spread:
			write(Out, "'''", 3);
			Eval(p->collection[0]);
			break;
	}
	Program *r;
	return r;
}

void Print(Program *p) {

}