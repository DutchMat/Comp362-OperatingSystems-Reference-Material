 #include "processQueue.h"
// Name: Charles Pelton
// Lab: Lab05
// Date: 9/30/18
PROCESS *processTable; // an array of all processes
int processTableSize = 0;
int currentMax = 0; // additional global to keep track of maximum
PROCESS **readyQueue; // an array of pointers to the processes in the processTable
int readyQueueSize = 0;
int queueMax = 0; // additional global to keep track of maximum

// constructor of the process table
void createProcessTable(int capacity)
{
    currentMax = capacity;
    processTable = (PROCESS *) malloc(capacity * sizeof(PROCESS));
}

// constructor of the ready queue
void createReadyQueue(int capacity)
{
    queueMax = capacity;
    readyQueue = (PROCESS **) malloc(capacity * sizeof(PROCESS *));
}

// adds a process and expands the table if necessary
void addProcessToTable(PROCESS process)
{
    // TODO done: complete
if(processTableSize >= currentMax){
	if(currentMax == 0){ // if not initalized
    		createProcessTable(PROCESS_TABLE_CAPACITY);
		goto addP; // skips expansion
	}
 	processTable = realloc(processTable, 2 * currentMax * sizeof(PROCESS)); //reallocate and expand array
 	currentMax = currentMax * 2;
	}

	addP: processTable[processTableSize] = process; //adds at top
	      //addProcessToReadyQueue(&(processTable[processTableSize]));
	      processTableSize++; // increments
}

void displayProcessTable()
{
    printf("PROCESSES:\nName\t\tEntry\tBurst\n");
    // TODO done: complete
	for(int place = 0; place < processTableSize; place++){
    printf("%s\t\t%d\t%d\n",processTable[place].name, processTable[place].entryTime, processTable[place].burstTime);
	}

    printf("\n");
}

// finds the shortest job in the ready queue and returns its location in the queue
int findShortestJob()
{
	// TODO done: complete
    int shortest = INT_MAX; // integer MAX
    int index = 0;
	for(int place = 0; place < readyQueueSize; place++){ // moves through ready queue
	 	if(shortest > readyQueue[place]->burstTime){ // 
			shortest = readyQueue[place]->burstTime; // new shortest
			index = place; // place of shortest
		}
	}

    return index;
}


// returns one process that arrives at the current time to the readyQueue
PROCESS * arrivingProcess(int time)
{
    // TODO done: complete
	for(int place = 0; place < processTableSize; place++){ // moves through process queue
		if(processTable[place].entryTime == time){
			// we assume at max, one entry at a single time
			return &(processTable[place]); // returns at earliest
		}
	}
	return NULL;
}

// determines if any processes in the process queue have more to execute
bool processesLeftToExecute()
{
    // TODO done: complete
	for(int place = 0; place < processTableSize; place++){ // moves through process queue
		if(processTable[place].burstTime > 0){ // checks if all process bursts are 0
		return true; // return some int not 0
		}
	}

    return false; //return 0 if all processes are complete
}


// adds a pointer and expands the ready queue if necessary
void addProcessToReadyQueue(PROCESS *pointer)
{
    // TODO done: complete
	if(readyQueueSize >= queueMax){
	if(queueMax == 0){ // if not initalized
    		createReadyQueue(READY_QUEUE_CAPACITY);
		goto addQ; // skips expansion
	}
 	readyQueue = realloc(readyQueue, 2 * queueMax * sizeof(PROCESS *)); //reallocate and expand array
 	queueMax = queueMax * 2;
	}

	addQ: readyQueue[readyQueueSize] = pointer; //adds at back
	      readyQueueSize++; // increments
	
}

// exposes a specific process in the ready queue (it stays in the queue)
PROCESS *getProcessFromReadyQueue(int index)
{
    	return readyQueue[index];
}

// gets a specific process from the ready queue (it gets removed from the queue)
PROCESS *fetchProcessFromReadyQueue(int index)
{
   if((index) >= readyQueueSize){
		return NULL;}
  
   return removeProcessFromReadyQueue(index);

}

// removes the process at index from the ready queue and returns a pointer to it
PROCESS *removeProcessFromReadyQueue(int index)
{
        PROCESS *removed = readyQueue[index];
	// TODO done: complete
	readyQueueSize--;
	for(int traverse = index; traverse < readyQueueSize; traverse++){
	readyQueue[traverse] = readyQueue[traverse + 1];
	}
    return removed;
}

// get size of the ready queue
int getReadyQueueSize()
{
    return readyQueueSize;
}

// displays the contents of the ready queue
void displayQueue()
{
     printf("QUEUE:");
    // TODO done: complete
	for(int place = 0; place < readyQueueSize; place++){
    printf(" %s(%d)", readyQueue[place]->name, readyQueue[place]->burstTime);
	}

    printf("\n");
	
}

//calculates the average wait time using information in the proc_queue
//and prints the value.
void printAverageWaitTime()
{
    // TODO done: complete
	double waitTime = 0;
	for(int place = 0; place < processTableSize; place++){
		waitTime = waitTime + ((processTable[place].offTime) - ((processTable[place].waitTime) + (processTable[place].entryTime)));
	}
		waitTime = (waitTime/processTableSize);
	printf("Average Wait Time: %.2lf\n", waitTime);
}

// clean-up the table and the queue
void cleanUp()
{
    free(processTable);
    free(readyQueue);
}




