#include "deadlock.h"
// Name: Charles Pelton
// Lab: Lab07
// Date: 10/14/2018
// TODO done: follow the lab description to implement an adjacency table-based deadlock detection
int nodeSize = 0;
NODE *firstNode = NULL;
int deadlock = 0;
int main()
{
   // TODO done: modify this main as needed for your implementation

   char *line = malloc(LINEBUFFER * sizeof(char));
   char *delim = " ={},";
   char *rest;
   char *next;
   char *next2;
   NODE *process;
   char **processes;
   int count = 0;
while(!feof(stdin)){ // when stdin is over, the parse is over
   scanf("%[^\n]\n", line); // line delimitation 
   rest = strdup(line);
   if(rest != NULL){
	 next = strsep(&(rest), delim);
	 if(next != '\0'){
	 switch(next[0]){
         case 'V':
		while (rest != NULL)
   		{
      		next = strsep(&(rest), delim); // adds processes
      		if (*next != '\0'){ // ends at EOL
		process = createNode(next);
                addVertexNode(process);
                }
                }
               createGraph(&processes);
              break;
         case 'E':
		while (rest != NULL)
   		{
      		next = strsep(&(rest), delim);
		if(count == 0){ // start
      		if (*next != '\0'){ // ends at EOL
		next2 = next;
		count++;
		}
                }
		else if(count == 1){ // end
		if (*next != '\0'){ // ends at EOL
		count--;
		createEdge(&processes, findNodeNum(next), findNodeNum(next2)); 
		}
		}
		//next2 = strsep(&(rest), delim);
      	//	if (*next2 != '\0'){ // ends at EOL CAUSES ERRORS
	//	printf("%s\n",next2);
	//	}
   		}
              break;
         default:
             printf("\nSytax Error; Use V = {Node}+, for processes present and E = {Node, Node}+, for which directed edges are present.\n");
             return 1;
             }
	}
   } 
}
displayGraph(&processes);
findDeadlock(&processes);
// time to free
   free(line);
   freeGraph(&processes);
   freeNodes(getFirstNode());
return 0;
}

NODE* createNode(char *name){ // makes a node struct
NODE* process = malloc(sizeof(NODE));
process->nodeName = malloc(LINEBUFFER* sizeof(char));
process->nodeName = name;
process->next = NULL;
return process;
}

int findNodeNum(char *name){
NODE *traverse = getFirstNode();
for(int i = 0; i < nodeSize; i++){ // through the list
if(strcmp((traverse->nodeName),name) == 0){ // compare
return i;
}
traverse = traverse->next; // last is null
}
return -1; // not found
}

void addVertexNode(NODE *node){ // adds node to chain
NODE *traverse = getFirstNode();
if(firstNode == NULL){
 firstNode = node;
}
else{
while(traverse->next != NULL){
traverse = traverse->next;
}
traverse->next = node;
}
nodeSize++;
}

NODE* getFirstNode(){ // returns first node
return firstNode;
}


void createGraph(char ***graph){ // makes graph
(*graph) = malloc((nodeSize) * sizeof(char *));
for(int row = 0; row < nodeSize; row++){
		(*graph)[row] = malloc((nodeSize) * sizeof(char));
	for(int col = 0; col < nodeSize; col++){
		(*graph)[row][col] = 0;
	}
}
}

void createEdge(char ***graph, int node1, int node2){ // makes edge
(*graph)[node2][node1] = 1;
}

void displayGraph(char ***graph){
printf("\nGRAPH ADJACENCY MATRIX\n\n"); // output
for(int row = 0; row < nodeSize; row++){ // row
		for(int col = 0; col < nodeSize; col++){ //col
			printf("%d ", (int)((*graph)[row][col])); // prints number and spaces
		}
		printf("\n"); // a next line
	}
}

void findDeadlock(char ***graph){
char *traversed = malloc(nodeSize * sizeof(char));
for(int set = 0; set < nodeSize; set++){ // sets vertex to 0
traversed[set] = 0;
}  
for(int walk = 0; walk < nodeSize; walk++){ // goes down all the vertex and tries to find loops from starting locations/NEW LOOPS
if(traversed[walk] == 0)
traversed = deadlockTraverseDepthFirst(&(*graph), &(*traversed), walk); // begin recursion
if(deadlock == 1){ // global deadlock
printf("\n>> DEADLOCKED <<");
goto END; // OH NOOOOOOOOOOOOOOOO
}
}
printf("\n<< NO DEADLOCK >>"); // YAY
END: printf("\n");
free(traversed); 
deadlock = 0;
}

char* deadlockTraverseDepthFirst(char ***graph, char *vertex, int loc){
if(vertex[loc] == 1 || deadlock == 1){ // base
deadlock = 1;
return vertex;
}
vertex[loc] = 1;

for(int i = 0; i < nodeSize; i++){
if((*graph)[loc][i] == 1){ // look at graph
deadlockTraverseDepthFirst(&(*graph), &(*vertex), i); // recursion
}
if(deadlock == 1){
return vertex;
}
}
vertex[loc] = 0;
return vertex;
}

void freeGraph(char ***graph){
for(int row = 0; row < nodeSize; row++){ // obviously reverse allocation
		free((*graph)[row]);
	}
	free(*graph);
}

void freeNodes(){
NODE *remove1 = getFirstNode();
NODE *remove2;
for(int i = 0; i < nodeSize; i++){
remove2 = remove1;
remove1 = remove1->next;
free(remove2);
}
nodeSize = 0;
}
