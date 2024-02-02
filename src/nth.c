#include "nth.h"

struct termios raw, restore;
struct winsize Window; //fields: ws_row, ws_col

collection *Image;


char *Buffer;
Int BufferLength;
Int BufferSize;
Int Pos;
Int Offset;
char readchar();
char PeekChar();

char*Nest;
Int Level;

void BufferAppend(char c) {
	BufferLength++;
	if (BufferLength >= BufferSize) {
		BufferSize += 1028;
		Buffer = realloc(Buffer, sizeof(char) * BufferSize);
	}
	Buffer[BufferLength - 1] = c;
}


void WinSizeCh (int signum) {
	ioctl(Out, TIOCGWINSZ, &Window);
}
void Echo() {
	write(Out, "\x1b[2K\r", 5);
	if (Offset > Window.ws_col)
		write(Out, &Buffer[BufferLength - (Window.ws_col - 1)], Window.ws_col - 1);
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
	Pos = 0;
	Offset = 0;
	Program *Input;
	char c;
	Int r;
	Init(Image);
        Repeat:
	Input = Read(0);
	//write(Out, "\r\n", 2);
	if (Input) {
		Eval(Input, Image);
		write(Out, "\r\n", 2);
		Discard(Input);
		Input = 0;
	}
	else {
		free(Buffer);
		BufferLength = 0;
		BufferSize = 0;
		Pos = 0;
		write(Out, "?\r\n", 3);
	}
	goto Repeat;
	return 0;
}

void SetRawMode() {
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
	ioctl(Out, TIOCGWINSZ, &Window);
	signal(SIGWINCH, WinSizeCh);
	//write(Out, "\x1b[2J\x1b[H", 7);
	write(Out, "\r\n", 2);
	write(Out, InfoString, sizeof(InfoString));
	write(Out, "\r\n", 2);
}

void Exit() {
	//if (Buffer != 0) {free(Buffer);}
	tcsetattr(0, TCSANOW, &restore);
	exit(0);
}

void ShowHint() {
	for (Int n = 0; n < Level; n++) {

	}
}

Int IsDelimiter(char c) {
	switch(c) {
		case '(':
		case '{':
		case '[':
		case ' ':
		case ',':
		case ')':
		case '}':
		case ']':
			return 1;
		default:
			return 0;
	}
}

Int IsPunct(char c) {
	switch(c) {
		case '(':
		case '{':
		case '[':
		case ' ':
		case ',':
		case ')':
		case '}':
		case ']':
		case '\'':
			return 1;
		default:
			return 0;
	}
}

char peekchar() {
	char c = readchar();
	Pos--;
	return c;
}

char readchar() {
	char c = 0;
	Int r = 0;
	if (BufferLength == 0 || Pos >= BufferLength) {
		BufferLength = 0;
		Pos = 0;
		BufferSize = 1024;
		Buffer = realloc(Buffer, sizeof(char) * BufferSize);
		for (;;) {
			if (read(In, &c, 1)) {
				if (c == 27) Exit();
				if (c == 8 || c == 127) {
					if (Offset > 0 && BufferLength > 0) {
						Offset --;
						BufferLength --;
						Echo();
					}
					continue;
				}
				if (c == 10 || c == 11 || c == 12 || c == 13) {
					Offset = 0;
					BufferLength++;
					if (BufferLength >= BufferSize)
						BufferSize += 1024, Buffer = realloc(Buffer, BufferSize);
					Buffer[BufferLength - 1] = ' ';
					write(Out, "\r\n", 2);
					goto End;
				}
				if (c >= ' ') {
					//BufferAppend(c);
					BufferLength++;
					if (BufferLength >= BufferSize)
						BufferSize += 1024, Buffer = realloc(Buffer, BufferSize);
					Buffer[BufferLength - 1] = c;
					Offset ++;
					Echo();
				}
			}
		}
	}
	End:
	c = Buffer[Pos];
	Pos ++;
	return c;
}

void Append(Program *p, Program *q) {
	p->size ++;
	if (p->collection)
		p->collection = realloc(p->collection, sizeof(Program*) * p->size);
	else
		p->collection = malloc(sizeof(Program*) * p->size);
	q->parent = p;
	p->collection[p->size - 1] = q;
}

Program *Read(Int InsideQuote) {
	Program *p = 0, *tmp = 0;
	p = malloc(sizeof(Program));
	p->size = 0;
	p->collection = 0;
	p->symbol = 0;
	TryAgain:
	char c = readchar();
	switch(c) {
		case '(':
			p->type = Expression;
			goto ReadExpression;
		case '{':
			p->type = Collection;
			goto ReadExpression;
		case '[':
			p->type = Type;
			goto ReadExpression;
		case '\'':
			p->type = Quote;
			goto ReadQuote;
		case ' ':
			goto TryAgain;
			break;
		case '"':
			p->type = String;
			goto ReadString;
			break;
		case ')': case '}': case ']': case ',':
			goto Error;
		default:
			p->type = Symbol;
			goto ReadSymbol;
	}
	ReadExpression:
	for (;;) {
		c = peekchar();
		switch(c) {
			case ' ':
				readchar();
				break;
			case ',':
				if (p->type != Expression && p->type != Sequence)
					goto Error;
				p->type = Sequence;
				tmp = malloc(sizeof(Program));
				tmp->type = Symbol;
				tmp->size = 1;
				tmp->symbol = malloc(sizeof(char));
				tmp->symbol[0] = ',';
				Append(p, tmp);
				readchar();
				break;
			case ')':
				readchar();
				if (p->type == Expression || p->type == Sequence) return p;
				else goto Error;
				break;
			case '}':	
				readchar();
				if (p->type == Collection) return p;
				else goto Error;
				break;
			case ']':
				readchar();
				if (p->type == Type) return p;
				else goto Error;
				break;
			default:
				tmp = Read(InsideQuote);
				if (tmp) Append(p, tmp);
				else goto Error;
		}
	}
	ReadQuote:
		c = peekchar();
		if (c == '\'') {
			if (!InsideQuote) 
				goto Error;
			readchar();
			c = peekchar();
			if (c == '\'') p->type = Requote, readchar();
			else p->type = Unquote;
		}
		tmp = Read(1);
		if (tmp) {
			Append(p, tmp);
			return p;
		}
		else goto Error;
	ReadString:
		tmp = malloc(sizeof(Program));
		tmp->type = Symbol;
		tmp->size = 0;
		tmp->collection = 0;
		tmp->symbol = malloc(sizeof(char) * 0);
		for (c = readchar(); c != '"'; c = readchar()) {
			if (c == '\\') {
				c = peekchar();
				if (c == '(') {
					Append(p, tmp);
					tmp = Read(InsideQuote);
					if (tmp) {
						Append(p, tmp);
						tmp = malloc(sizeof(Program));
						tmp->type = Symbol;
						tmp->size = 0;
						tmp->collection = 0;
						tmp->symbol = malloc(sizeof(char) * 0);
					}
					else goto Error;
					continue;
				}
				else {
					tmp->size++;
					tmp->symbol = realloc(tmp->symbol, sizeof(char) * tmp->size);
					tmp->symbol[tmp->size - 1] = c;
					readchar();
				}
			}
			else {
				tmp->size++;
				tmp->symbol = realloc(tmp->symbol, sizeof(char) * tmp->size);
				tmp->symbol[tmp->size - 1] = c;
			}
		}
		Append(p, tmp);
		return p;
	ReadSymbol:
		p->size = 1;
		p->symbol = malloc(sizeof(char));
		p->symbol[0] = c;
	for (c = peekchar(); !IsDelimiter(c); c = peekchar()) {
		p->size++;
		p->symbol = realloc(p->symbol, sizeof(char) * p->size);
		p->symbol[p->size - 1] = readchar();
	}
	return p;
	Error:
	if (p) Discard(p);
	if (tmp) Discard(tmp);
	return 0;
}

void Discard(Program *p) {
	switch(p->type) {
		case Symbol:
			free(p->symbol);
			break;
		case Expression: case Sequence: case Collection: case Type: case Quote: case Unquote: case Requote:
			for (Int i = 0; i < p->size; i++)
				Discard(p->collection[i]);
			free(p->collection);
			break;
		default:
			break;
	}
}

Program *FancyPrint(Program *p) {
	if (p == 0) return 0;
	switch(p->type) {
		case Symbol:
			//write(Out, " ", 1);
			write(Out, p->symbol, p->size);
			//write(Out, " ", 1);
			break;
		case String:
			write(Out, "\"", 1);
			write(Out, p->symbol, p->size);
			write(Out, "\"", 1);
			break; 
		case Expression:
			write(Out, "(", 1);
			for (Int n = 0; n < p->size; n++) 
				FancyPrint(p->collection[n]), n < p->size - 1 ? write(Out, " ", 1) : 0;
			write(Out, ")", 1);
			break; 
		case Sequence:
			write(Out, "(", 1);
			for (Int n = 0; n < p->size; n++)
				FancyPrint(p->collection[n]), n < p->size - 1 ? write(Out, " ", 1) : 0;
			write(Out, ")", 1);
			break;
		case Collection:
			write(Out, "{", 1);
			for (Int n = 0; n < p->size; n++) 
				FancyPrint(p->collection[n]), n < p->size - 1 ? write(Out, " ", 1) : 0;
			write(Out, "}", 1);
			break;
		case Type:
			write(Out, "[", 1);
			for (Int n = 0; n < p->size; n++) 
				FancyPrint(p->collection[n]), n < p->size - 1 ? write(Out, " ", 1) : 0;
			write(Out, "]", 1);
			break;
		case Quote:
			write(Out, "' ", 2);
			FancyPrint(p->collection[0]);
			break;
		case Unquote:
			write(Out, "'' ", 3);
			FancyPrint(p->collection[0]);
			break;
		case Requote:
			write(Out, "''' ", 4);
			FancyPrint(p->collection[0]);
			break;
	}
	Program *r;
	return r;
}

void Print(Program *p) {

}