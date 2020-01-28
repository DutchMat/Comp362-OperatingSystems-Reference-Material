#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define LINEBUFFER 512
// Name: Charles Pelton
// Lab: Lab09
// Date:10/28/2018
typedef struct biLinkedNode{
int value;
struct biLinkedNode *next;
struct biLinkedNode *prev;
} NODE;

void createTable(int);
void addProcess(NODE*);
void setPriority(NODE*);
int inTable(NODE*);
void replace(NODE*);
void display();
static int tableSize;
static int currentSize;
static int faults;
static NODE *topNode;
