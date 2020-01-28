#include "replace.h"
// Name: Charles Pelton
// Lab: Lab09
// Date:10/28/2018

void freeNodes(NODE *node){
NODE *current;
	while(topNode->next != NULL){
	current = topNode;
	topNode = topNode->next;
	free(current);
	}
free(topNode);
} // The reason why there is a second submission

int main(){
   printf("To use the function, pipe an input file though the terminal or specify the first number to be the table size and the rest as processes. To end the program, perform CTRL-d\n");
   char *line = malloc(LINEBUFFER * sizeof(char));
   char *delim = " ={},";
   char *rest;
   char *next;
	scanf("%[^\n]\n", line); // line delimitation 
   	rest = strdup(line);
	next = strsep(&(rest), delim);
	createTable(next[0]-48);
	printf("\nSize of Table is %d\n\n", next[0]-48);
	NODE *temp;
	while(!feof(stdin)){ // when stdin is over, the parse is over
		scanf("%[^\n]\n", line); // line delimitation 
   		rest = strdup(line);
		while(rest != NULL){
		temp = calloc(1,sizeof(NODE));
	 	next = strsep(&(rest), delim);
		temp->value = next[0]-48;
		temp->next = NULL; temp->prev = NULL;
            	addProcess(temp);
		}
	}
printf("\nNumber of Faults: %d\n", faults);
freeNodes(topNode);
return 0;
}

void createTable(int size){ // new table limit, sets defaults
tableSize = size;
topNode = NULL;
currentSize = 0;
faults = 0;
}

void initAdd(NODE *node){ // adds
	if(topNode == NULL){
		topNode = node;
	}
	else{
		NODE *temp = topNode;
		node->next = temp;
		temp->prev = node;
		topNode = node;
	}
currentSize++;
}

void addProcess(NODE *node){
	if(inTable(node)){ // hit and move
	// filler
	}
	else if(currentSize < tableSize){ // add and fault
		printf("*");
		initAdd(node);
		faults++;
	}
	else{ // replace, move and fault
	printf("*");
	replace(node);
	faults++;
	}
	display(topNode);
	printf("\n");
}

int inTable(NODE *node){
	NODE *temp = topNode;
	while(temp != NULL){
		if(temp->value == node->value){ // hit
			if(topNode->value != node->value){
			setPriority(temp);}
			free(node);
			printf(">");
			return 1;
		}
		temp = temp->next;
	}
return 0;
}

void setPriority(NODE *node){ // its here
if(node->prev != NULL){
node->prev->next = node->next;
}
if(node->next != NULL){
node->next->prev = node->prev;
}
topNode->prev = node;
node->next = topNode;
topNode = node;
}

void replace(NODE *node){
NODE *temp = topNode;
if(tableSize == 1){ // don't need to replace
free(temp);
topNode = node;
}
else{ // replace
NODE *temp = topNode;
	while(temp->next != NULL){ // go to last/oldest
		temp = temp->next;
	}
temp->prev->next = node; // replace everything about it
node->prev = temp->prev;
if(temp->next != NULL){
node->next = temp->next;
temp->next->prev = node;
}
free(temp); // free disjoint node
setPriority(node); // set to newest node
}
}

void display(){ // displays the nodes
NODE *temp = topNode;
	while(temp != NULL){
	printf("%d ", temp->value);
	temp = temp->next;
	}
}
