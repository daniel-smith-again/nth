#include "nth.h"

struct termios raw, restore;
struct winsize Window; //fields: ws_row, ws_col

collection *Image;

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
	Init(Image);
        Repeat:
	r = read(In, c, sizeof(c));
	if (r == 1) {
		switch(c[0]) {
			case 27: 
				if (BufferLength == 0) {
					exit(0);
				}
				else {
					write(Out, "\r\n", 2);
					free(Buffer), Buffer = 0;
					BufferLength = 0;
					BufferSize = 0;
					Offset = 0;
				}
				break;
			case 10: case 11: case 12: case 13: //all the ways to line break
				if (BufferLength == 0) 
					break;
				Input = Parse(0);
				if ((Int)Input == 1) {
					free(Buffer);
					Buffer = 0;
					BufferLength = 0;
					BufferSize = 0;
				}
				else if (Input != 0) {
					write(Out, "\r\n", 2);
					//FancyPrint(Input);
					Eval(Input, Image);
					write(Out, "\r\n", 2);
					Discard(Input);
					Input = 0;
				}
				else if (Input == 0) {
					switch (Buffer[BufferLength - 1]) {
						case ')': case '}': case ']':
							break;
						default:
							BufferAppend(' ');
							Offset = 0;
							write(Out, "\r\n", 2);
							break;
					}
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
	//write(Out, "\x1b[2J\x1b[H", 7);
	write(Out, "\r\n", 2);
	write(Out, InfoString, sizeof(InfoString));
	write(Out, "\r\n", 2);
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
		case Expression: case Sequence: case Collection: case Type: case Quote: case Unquote: case Requote:
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
	Int InString = 0;
	Int StringEscape = 0;
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
			case '"':
				if (InString) {
					InString = 0;
					p = p->parent;
				}
				else {
					InString = 1;
					tmp = malloc(sizeof(Program));
					tmp->type = String;
					tmp->size = 0;
					tmp->parent = p;
					p->size ++;
					p->collection = realloc(p->collection, sizeof(Program*) * p->size);
					p->collection[p->size - 1] = tmp;
					p = tmp;
				}
				break;
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
				if (p->type == String) InString = 1;
				if (p->type == Sequence) p = p->parent;
				Level --;
				if (Nest[Level - 1] == '\'') Level --, Quoted = 0;
				for (Int n = 0; n < Level; n++)
					if (Nest[n] == '\'') {
						Quoted = 1; 
						break;
					}
				break;
			case '\'':
				Int QuoteSize = 1;
				for (; i + QuoteSize < BufferLength; QuoteSize++) 
					if (Buffer[i + QuoteSize] != '\'')
						break;
				i += QuoteSize - 1;
				if (QuoteSize == 1) {
					Level++;
					Nest[Level - 1] = '\'';
					tmp = malloc(sizeof(Program));
					tmp->type = Quote;
				}
				else if (QuoteSize < 4) {
					for (Int n = 0; n < Level; n++) 
						if (Nest[n] == '\'') 
							goto InsideQuote;
					goto OutsideQuote;
					InsideQuote:
					tmp = malloc(sizeof(Program));
					if (QuoteSize == 2) tmp->type = Unquote;
					else tmp->type = Requote;
				}
				else {
					OutsideQuote:
					write(Out, "\r\n?\r\n", 5);
					Discard(top);
					return (Program*)1;
				}
				tmp->size = 0;
				tmp->parent = p;
				p->size++;
				p->collection = realloc(p->collection, sizeof(Program) * p->size);
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
				makesym:
				if (InString) {
					tmp = malloc(sizeof(Program));
					tmp->type = Symbol;
					tmp->symbol = malloc(sizeof(char) * 0);
					tmp->parent = p;
					for (Int escape = 0; i < BufferLength; i++) {
						switch(Buffer[i]) {
							case '\\':
								if (!escape) {
									escape = 1;
									break;
								}
								else
									escape = 0;
									goto DefaultAppChar;
								break;
							case '"':
								if (!escape) {
									i --;
									goto StringBreak;
								}
								else
									escape = 0;
									goto DefaultAppChar;
								break;
							case '(':
								if (escape) {
									i --;
									InString = 0;
									goto StringBreak;
								}
								else 
									escape = 0;
									goto DefaultAppChar;
								break;
							default:
								if (escape) {
									write(Out, "\r\n?\r\n", 5);
									Discard(top);
									return (Program*)1;
								}
								DefaultAppChar:
								tmp->size ++;
								tmp->symbol = realloc(tmp->symbol, sizeof(char) * tmp->size);
								tmp->symbol[tmp->size - 1] = Buffer[i];
						}
					}
					StringBreak:
					tmp->parent = p;
					p->size ++;
					p->collection = realloc(p->collection, sizeof(Program*) * p->size);
					p->collection[p->size - 1] = tmp;
				}
				else {
					tmp = malloc(sizeof(Program));
					tmp->type = Symbol;
					tmp->parent = p;
					for (tmp->size = 1; tmp->size + i < BufferLength; tmp->size ++)
						switch(Buffer[i + tmp->size]) {
							case '(': case '{': case '[': case ')': case '}': case ']': case ' ': case ',':
								goto SymBreak;
						}
					SymBreak:
					tmp->symbol = malloc(sizeof(char) * tmp->size);
					for (Int n = 0; n < tmp->size; n++)
						tmp->symbol[n] = Buffer[i + n];
					i += (tmp->size - 1);
					tmp->parent = p;
					p->size++;
					p->collection = realloc(p->collection, sizeof(Program*) * p->size);
					p->collection[p->size - 1] = tmp;
					while (p->type == Quote || p->type == Unquote || p->type == Requote) {
						if (Nest[Level - 1] == '\'' && p->type == Quote) {
							Level--;
						}
						p = p->parent;
					}
				}
				break;
		}
	}
	if (InString) {
		write(Out, "\x1b[90m\"\x1b[0m", 10);
		Discard(top);
		return 0;
	}
	else if (Level > 0) {
		Discard(top);
		write(Out, "\x1b[90m", 5);
		for (Int n = Level; n > 0; n--) {
			switch(Nest[n - 1]) {
				case '(':
					write(Out, ")", 1); break;
				case '{':
					write(Out, "}", 1); break;
				case '[':
					write(Out, "]", 1); break;
				default:
					break;
			}
		}
		write(Out, "\x1b[0m", 4);
		return 0;
	}
	else {
		free(Buffer);
		Buffer = 0;
		BufferLength = 0;
		BufferSize = 0;
		return top;
	}
}

Program *FancyPrint(Program *p) {
	if (p == 0) return 0;
	switch(p->type) {
		case Symbol:
			write(Out, " ", 1);
			write(Out, p->symbol, p->size);
			write(Out, " ", 1);
			break;
		case String:
			for (Int n = 0; n < p->size; n++) {
				if (p->collection[n]->type == String && p->collection[n]->size == 1) {
					write(Out, "\"", 1);
					write(Out, p->collection[n]->symbol, p->collection[n]->size);
					write(Out, "\"", 1);
				} 
				else 
					FancyPrint(p->collection[n]);
			}
			break;
		case Expression:
			write(Out, "(", 1);
			for (Int n = 0; n < p->size; n++) FancyPrint(p->collection[n]);
			write(Out, ")", 1);
			break; 
		case Sequence:
			write(Out, "(", 1);
			for (Int n = 0; n < p->size; n++) {
				FancyPrint(p->collection[n]);
				if (n < p->size - 1)
					write(Out, ", ", 2);
			}
			write(Out, ")", 1);
			break;
		case Collection:
			write(Out, "{", 1);
			for (Int n = 0; n < p->size; n++) FancyPrint(p->collection[n]);
			write(Out, "}", 1);
			break;
		case Type:
			write(Out, "[", 1);
			for (Int n = 0; n < p->size; n++) FancyPrint(p->collection[n]);
			write(Out, "]", 1);
			break;
		case Quote:
			write(Out, " '", 2);
			FancyPrint(p->collection[0]);
			break;
		case Unquote:
			write(Out, " ''", 3);
			FancyPrint(p->collection[0]);
			break;
		case Requote:
			write(Out, " '''", 4);
			FancyPrint(p->collection[0]);
			break;
	}
	Program *r;
	return r;
}

void Print(Program *p) {

}