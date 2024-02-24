#include <stdlib.h>
#include <stdio.h>
#define InfoString \ 
"Welcome to the nth language programming utility\r\n\
Copyright (C) Daniel Smith daniel.smith.again@gmail.com"
typedef long long unsigned int Int;
typedef struct {
  Int type;
} Data;

void Read();
void Compute();
void Print();

#ifdef Linux_AMD64
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <elf.h>
#define In STDIN_FILENO
#define Out STDOUT_FILENO
typedef unsigned long long int Int;
