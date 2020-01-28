#include "inverted.h"

// Name: Charles Pelton
// Lab: Lab08
// Date: 10/21/18
/*
 * initializes the table for the given sizes of the memory and frame
 */
void initInverted(PROC **table, int msize, int fsize)
{
    // TODO done: implement
	*table = calloc(msize/fsize, sizeof(PROC));
	pSize = fsize;
	tableSize = msize/fsize;
	time(NULL);
}

/*
 * translate a logical address <pid, page, offset> into a physical address
 * if there is no entry for the pid and the page number in the table, it has to be added
 * if there is no room in the table for the new entry, then the oldest entry has to be removed
 */
long translate(PROC *table, int pid, int page, int offset)
{
    // TODO done: implement
	int found = lookUp(table, pid, page);
	if(found == -1){
	found = findOldest(table);
	table[found].pid = pid;
	table[found].base = page;
	sleep(1);
	time(&(table[found].timeStamp));
	}
    return ((found*pSize) + offset);
}

/*
 * find the entry in the table for a specific pid and page
 */
long lookUp(PROC *table, int pid, int page)
{
    int i;
    // TODO done: implement
	for(i = 0; i < tableSize; i++){
	if((table[i].pid == pid) && (table[i].base == page)){
		sleep(1);
		time(&(table[i].timeStamp));
		return i;
		}
	}
    return -1;
}

/*
 * find the oldest entry in the table
 */
long findOldest(PROC *table)
{
    int min;
    int i;

    // TODO done: implement
	if(difftime(table[0].timeStamp,0) == 0){
	sleep(1);
	time(&(table[0].timeStamp));
	return 0;
	}
	min = 0;
	for(i = 1; i < tableSize; i++){
		if(difftime(table[i].timeStamp,0) == 0){
			sleep(1);
			time(&(table[i].timeStamp));
			return i;
		}
		if(difftime(table[i].timeStamp,table[min].timeStamp) < 0){
			min = i;
		}
	}
    return min;
}
