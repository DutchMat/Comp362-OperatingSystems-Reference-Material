#define MEM_SIZE 512*512
#define PAGE_SIZE 4096
#include "inverted.h"
#include <dlfcn.h>
// Name: Charles Pelton
// Lab: Lab08
// Date: 10/21/18
PROC *table;
// Main for Dynamic Library Testing
int main(int argc, char **argv)
{
   void *dylibHandle;
   char *error;
   
   dylibHandle = dlopen ("libinverted.so", RTLD_LAZY);
   if (!dylibHandle)
   {
      fprintf (stderr, "ERR: %s\n", dlerror());
      exit(1);
   }
   void (*initInverted)(PROC**,int,int) = dlsym(dylibHandle, "initInverted");
   long (*translate)(PROC*,int,int,int) = dlsym(dylibHandle, "translate");
   (*initInverted)(&table, MEM_SIZE, PAGE_SIZE);
   if ((error = dlerror()) != NULL)
   {
      fprintf (stderr, "ERR: %s\n", error);
      exit(1);
   }
   int pid; int page; int offset; long physical;
   srand(time(NULL));
   for(int a = 0; a < (MEM_SIZE/PAGE_SIZE)*2; a++){
   pid = rand() % 8;
   page = rand() % 32;
   offset = rand() % PAGE_SIZE;
   physical = (*translate)(table, pid, page, offset);
   if ((error = dlerror()) != NULL)
   {
      fprintf (stderr, "ERR: %s\n", error);
      exit(1);
   }
   printf("translate(%d, %d, %d) = %ld\n",pid,page,offset, physical);
   }
    table = NULL;
    free(table);
    dlclose(dylibHandle);
   return 0;
}
