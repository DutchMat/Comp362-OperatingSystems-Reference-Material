#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Name: Charles Pelton
// Lab: Lab08
// Date: 10/21/18
typedef struct proc
	{
      int pid;
      int base;
      time_t timeStamp;
	} PROC;

void initInverted(PROC **, int memSize, int frameSize);
long translate(PROC *, int, int, int);
long lookUp(PROC *, int, int);
long findOldest(PROC *table);
static int pSize;
static int tableSize;
