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
typedef enum {Expression, Sequence, Collection, Type, Symbol, Quote, String, Number} NodeType;
typedef unsigned long long int Register;
typedef unsigned char Byte;
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
	
	char *c;
	void *Program_Model;
ReadExpression:
ReadSequence:
ReadCollection:
ReadType:
ReadSymbol:
ReadQuote:
ReadString:
EXIT:
	write(1, "\n", 1);
	tcsetattr(0, TCSANOW, &restore);
	exit(0);
	return 0;
}
