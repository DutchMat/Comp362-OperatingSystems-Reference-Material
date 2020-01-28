#include "message.h"
int main(int argc, char *argv[]){
	if (argc < 4)
	{
		printf("Usage: monitor <initial temperature> <# of nodes> <# nodes temperatures>+\n");
		exit(1);
	}
if (mq_unlink(MONITOR_QUEUE) == 0)
		printf("MONITOR: Message queue %s removed.\n", MONITOR_QUEUE);

struct mq_attr attr; //settings

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

        TEMPERATURE temp;
	temp.temperature = strtol(argv[1], NULL, 0);
if ((temp.msqid = mq_open(MONITOR_QUEUE, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr)) < 0)
		oops("MONITOR: Error opening a server queue.", errno);
	
	printf("MONITOR: Message queue %s created.\n", MONITOR_QUEUE);
        int nodeNumber = strtol(argv[2], NULL, 0);
if((argc - nodeNumber) < 3){
printf("Not enough temperatures, Usage: monitor <initial temperature> <# of nodes> <# nodes temperatures>+, where <# nodes temperatures>+ has each node temperature set, seperated by a space \n");
		exit(1);
} 
MESSAGE received, send[nodeNumber];
char *nodeName[nodeNumber];
pid_t pid;
float array[nodeNumber];
float sum = 0;
for(int i; i < nodeNumber; i++){
send[i].stable = false;
array[i] = strtol(argv[i+3], NULL, 0);;
sum = sum + array[i];
send[i].nodeId = i;
nodeName[i] = malloc(MEMSIZE);
sprintf(nodeName[i],"/%s%d",NODE_NAME_PREFIX,i);

if (mq_unlink(nodeName[i]) == 0){
		printf("MONITOR: Message queue %s removed.\n", nodeName[i]);}

	if ((send[i].temperature.msqid = mq_open(nodeName[i], O_RDWR | O_CREAT, S_IWUSR | S_IRUSR, &attr)) < 0){
		oops("MONITOR: Error opening a client queue.", errno);}
		printf("MONITOR: Message queue %s created.\n", nodeName[i]);

pid = fork(); // duplicate the processes/data objects
if(pid == 0){ // its a child
	printf("MONITOR: NODE_%d created.\n", i);

	if (execlp("node","node", nodeName[i], argv[i+3], NULL) < 0){
 	      	oops("Execlp Failed! Node not created.", errno);}
		exit(EXIT_SUCCESS);
	} // I am the parent, repeat

}
float newTemp;
newTemp = (2 * temp.temperature + sum) / 6;
temp.temperature = newTemp;
printf("MONITOR TEMPERATURE: %.2f\n",newTemp);
bool accounted = false;
bool numberOfNodes[nodeNumber];
bool nodeFinished[nodeNumber];
int totalNodes = nodeNumber;
int count = 0;
while (true){ // parent plays here
for(int i; i < nodeNumber; i++){ // sends data to all nodes
		send[i].temperature.temperature = temp.temperature;
		numberOfNodes[i] = false;
		if (mq_send(send[i].temperature.msqid, (const char*) &send[i], sizeof(send[i]), 0) < 0) // parent sends data to appropriate node              		
		oops("SRV: Cannot respond to a client.", errno);
}
		
		if (accounted){ // parent terminates
			printf("STABLE TEMPERATURE DETECTED.\nMONITOR TERMINATING...\n");

			if (mq_unlink(MONITOR_QUEUE) == 0){
		printf("MONITOR: Message queue %s removed.\n", MONITOR_QUEUE);
				}

			for(int i; i < nodeNumber; i++){
			if (mq_unlink(nodeName[i]) == 0){
		printf("MONITOR: Message queue %s removed.\n", nodeName[i]);
			free(nodeName[i]);
				}
				}
	
		exit(EXIT_SUCCESS);
			}


		sum = 0; //prime
		count = 0;
	while(!accounted){ //parent calculates
		if (mq_receive(temp.msqid, (char*) &received, MAX_MSG_SIZE, NULL) >= 0){ // parent receives data from node

		if(!(numberOfNodes[received.nodeId])){
			printf("MONITOR: %s%d REPORTS: %.2f DEGREES\n",NODE_NAME_PREFIX,received.nodeId,received.temperature.temperature);
		sum = sum + received.temperature.temperature; 
		array[received.nodeId] = received.temperature.temperature;
		count++;
		}// calculate	
		if(count == totalNodes){
		accounted = true; }
		
		}
	}
	
		newTemp = (2 * temp.temperature + sum) / 6; //calcuate
		printf("MONITOR TEMPERATURE: %.2f\n",newTemp);

		for(int i; i < nodeNumber; i++){
			if(newTemp == array[i]){
			totalNodes--;
			send[i].stable = true;}
		}
	temp.temperature = newTemp;
		for(int i; i < nodeNumber; i++){
			if(!send[i].stable){
				accounted = false;
			}	
		}

}
return 0;
}
