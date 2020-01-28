#include "inverted.h"

#define MEM_SIZE 512*512
#define PAGE_SIZE 4096
// Name: Charles Pelton
// Lab: Lab08
// Date: 10/21/18
PROC *table;
// MAIN for Regular, Static and Implicit Libarary Testing
int main(int argc, char **argv)
{
   initInverted(&table, MEM_SIZE, PAGE_SIZE);
   int pid; int page; int offset;
   srand(time(NULL));
   for(int a = 0; a < (MEM_SIZE/PAGE_SIZE)*2; a++){
   pid = rand() % 8;
   page = rand() % 32;
   offset = rand() % PAGE_SIZE;
   printf("translate(%d, %d, %d) = %ld\n",pid,page,offset, translate(table, pid, page, offset));
   }
    table = NULL;
    free(table);	
   return 0;
}
