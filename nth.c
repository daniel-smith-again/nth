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
struct winsize window_size;
typedef unsigned long long int Register;
typedef unsigned char Byte;

struct termios ui, restore;
void init_interface();
void exit_interface();

void *construct_program_model();
void *compute_program_model();

void display(void *d);
void *read();

int main ()
{
	char *c;
	void *Program_Model;
	init_interface();
	
EXIT: exit_interface();
	return 0;
}

init_interface() {
	tcgetattr(0, &restore);
	ui = restore;
	ui.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	ui.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	ui.c_cflag &= ~(CSIZE | PARENB);
	ui.c_cflag |= CS8;
	tcsetattr(0, TCSANOW, &ui);
	ioctl(0, TIOCGWINSZ, &window_size);
}

exit_interface() {
	write(1, "\n", 1);
	tcsetattr(0, TCSANOW, &restore);
	exit(0);
}