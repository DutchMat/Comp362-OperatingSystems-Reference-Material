#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define LINEBUFFER 256
// Name: Charles Pelton
// Lab: Lab07
// Date: 10/14/2018
// TODO done: add declarations of types and functions in here
typedef struct node{
struct node *next;
char *nodeName;
}NODE;

NODE* createNode(char *name);
int findNodeNum(char *name);
void createGraph(char ***graph);
void addVertexNode(NODE *node);
NODE* getFirstNode();
void createEdge(char ***graph, int node1, int node2);
void displayGraph(char ***graph);
void findDeadlock(char ***graph);
char* deadlockTraverseDepthFirst(char ***graph, char *vertex, int loc);
void freeGraph(char ***graph);
void freeNodes();

