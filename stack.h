#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>

typedef int Data;
typedef struct Node
{
	int Number;
	struct Node * Next;
	Data Value;
} Node;

Node * CreateStack();
Node * PushStack(Node * stack, Data value);
Data PopStack(Node ** stack);
void PrintStack(Node * stack);
int SizeStack(Node * stack);
void DotStack(Node * stack);
void NodeToDot(Node * stack);
Data CopyTopStack(Node * stack);
int IsEmptyStack(Node * stack);
void ManualStack();
void UserQuery(Node * stack);
void DeleteStack(Node * stack);
Node * MulStack(Node * stack);
Node * AddStack(Node * stack);
Node * SubStack(Node * stack);
Node * DivStack(Node * stack);
