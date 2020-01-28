#include "cpuScheduler.h"
// Name: Charles Pelton
// Lab: Lab05
// Date: 9/30/18
int main()
{
    int time = 0; // simulated time

    ALGORITHM_PARAMS parameters = {NULL, "", NULL, 0, 0, NULL}; // simulation parameters

    // read the algorithm type and time quantum if necessary
    scanf("%s", parameters.algorithm);

    //check which algorithm was passed, set values accordingly
    if (strcmp(parameters.algorithm, "RR") == 0)
    {
        scanf("%d", &parameters.quantum);
        parameters.step = &rrStep;
    }
    else if (strcmp(parameters.algorithm, "FCFS") == 0)
        parameters.step = &fcfsStep;
    else if (strcmp(parameters.algorithm, "SJF") == 0)
        parameters.step = &sjfStep;
    else if (strcmp(parameters.algorithm, "SRTF") == 0)
        parameters.step = &srtfStep;
    else
    {
        printf("The job type input is not a valid input!");
        exit(EXIT_FAILURE);
    }

    // skip over the enf of line marker
    scanf("\n");

    printf("\nALGORITHM: %s", parameters.algorithm);
    if (strcmp(parameters.algorithm, "RR") == 0)
        printf("%3d", parameters.quantum);
    printf("\n\n");

    createProcessTable(PROCESS_TABLE_CAPACITY); //create process table
    createReadyQueue(READY_QUEUE_CAPACITY);//create queue with size == number of processes

    readProcessTable(); //populate process table
    displayProcessTable();
	//displayQueue();

    if ((parameters.newProcess = arrivingProcess(time)) != NULL)
        addProcessToReadyQueue(parameters.newProcess);

    while (processesLeftToExecute())
    {
        parameters.time = time;

        doStep(parameters.step, &parameters);

        displayTimeTick(time, parameters.cpu);

        if (parameters.cpu != NULL)
            parameters.cpu->burstTime--;

        time++;

        if ((parameters.newProcess = arrivingProcess(time)) != NULL)
            addProcessToReadyQueue(parameters.newProcess);
    }
	if(parameters.cpu != NULL)
    	parameters.cpu->offTime = (parameters.time) + 1; // something for all ending offsets

    printAverageWaitTime();

    cleanUp();

    return EXIT_SUCCESS;
}

// step executor
void doStep(void (*func)(void *), void *param)
{
    func(param);
}

// function implementing a step of FCFS
void fcfsStep(void *param)
{
    ALGORITHM_PARAMS *p = (ALGORITHM_PARAMS *) param;
	 // TODO done: implement
	if(p->cpu == NULL){ // first fetch
	fcfswork: p->cpu = fetchProcessFromReadyQueue(0); // fetches
	}
	if(p->cpu != NULL){
		if((p->cpu)->burstTime == 0){ // sets for all but last
		(p->cpu)->offTime = (p->time);
		goto fcfswork; // grabs next process
		}
	}
}

// function implementing a step of SJF
void sjfStep(void *param)
{
    ALGORITHM_PARAMS *p = (ALGORITHM_PARAMS *) param;
	// TODO done: implement
	if(p->cpu == NULL){ // first fetch
	sjfwork: p->cpu = fetchProcessFromReadyQueue(findShortestJob()); // fetches
	}
	if(p->cpu != NULL){
		if((p->cpu)->burstTime == 0){ // sets for all but last
		(p->cpu)->offTime = (p->time);
		goto sjfwork; // grabs next process
		}
	}
}

// function implementing a step of SRTF
void srtfStep(void *param)
{
    ALGORITHM_PARAMS *p = (ALGORITHM_PARAMS *) param;
	// TODO done: implement
	if(p->cpu == NULL){ // shortest fetch
	srtfwork: p->cpu = fetchProcessFromReadyQueue(findShortestJob()); // fetches
	}
	if(p->cpu != NULL){
	if((p->cpu)->burstTime == 0){ // sets for all but last
	(p->cpu)->offTime = (p->time);
	goto srtfwork; // grabs next process
	}
	else if(getProcessFromReadyQueue(findShortestJob())->burstTime < (p->cpu)->burstTime){ // is there a shorter operation?
	(p->cpu)->offTime = (p->time);
	addProcessToReadyQueue(p->cpu); //push cpu back into queue
	goto srtfwork; // grabs shortest process
	}
	}
}

// function implementing a step of RR
void rrStep(void *param)
{
    ALGORITHM_PARAMS *p = (ALGORITHM_PARAMS *) param;
	static int rrCounter; // counter to keep track of how many more ticks the process in cpu has left
	// TODO done: implement
	if(p->cpu == NULL){ // next fetch
	rrwork: p->cpu = fetchProcessFromReadyQueue(0); // fetches
		 rrCounter = p->quantum; // sets QUANTUM TIME
	}
	if(p->cpu != NULL){
	if((p->cpu)->burstTime == 0){ // sets for all but last
	(p->cpu)->offTime = (p->time);
	goto rrwork; // grabs next process
	}
	else if(rrCounter == 0){ // rr is over :c
	(p->cpu)->offTime = (p->time);
	addProcessToReadyQueue(p->cpu); //push cpu back into queue
	goto rrwork; // grabs next process in rotation
	}
	rrCounter--;
	}
	
}

//fills the processTable with processes from input
int readProcessTable()
{
    PROCESS tempProcess = {"", 0, 0, 0, 0};

    char *line = NULL;
    char *currPos;
    size_t len = 0;

    int counter = 0;
    int offset = 0;

    while (getline(&line, &len, stdin) != -1)
    {
        currPos = line;
        sscanf(currPos, "%s%n", tempProcess.name, &offset);
        currPos += offset;
        sscanf(currPos, "%d%n", &tempProcess.entryTime, &offset);
        currPos += offset;
        sscanf(currPos, "%d", &tempProcess.burstTime);
	tempProcess.waitTime = tempProcess.burstTime; // small change

        addProcessToTable(tempProcess);

        free(line);
        line = NULL;

        counter++;
    }

    free(line);

    return counter;
}

void displayTimeTick(int time, PROCESS *cpu)
{
    printf("T%d:\nCPU: ", time);
    if (cpu == NULL)
        printf("<idle>\n");
    else
        printf("%s(%d)\n", cpu->name, cpu->burstTime);

    displayQueue();
    printf("\n\n");
}






