#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define WordSize 30

FILE * file_dot;

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

int main()
{

	Node * stack = CreateStack();

	ManualStack();
	UserQuery(stack);

	DeleteStack(stack);

	return 0;
}

Node * CreateStack()
{
	return NULL;
}

Node * PushStack(Node * stack, Data value)
{
	if (stack == NULL)
	{
		stack = calloc(1, sizeof(Node));
		stack->Next = NULL;
		stack->Value = value;
		stack->Number = 1;
	}
	else
	{
		stack->Next =  PushStack(stack->Next, value);
		stack->Number++;
	}

	return stack;
}

void PrintStack(Node * stack)
{
	assert(stack);

	printf("%d", stack->Value);
	if (stack->Next)
	{
		printf(" ");
		PrintStack(stack->Next);
	}
}

void DotStack(Node * stack)
{
	assert(stack);
	file_dot = NULL;

	file_dot = fopen("tree.dot", "w");
	assert(file_dot);
	fprintf(file_dot, "digraph G {\n");
	NodeToDot(stack);
	fprintf(file_dot, "}");
	fclose(file_dot);

	system("dot -v -Tpng -o tree tree.dot");
	system("rm tree.dot");
	printf("\nSuccesfully\n");
}

void NodeToDot(Node * stack)
{
	assert(stack);

	fprintf(file_dot, "\tN%d [label=\"", stack->Number);
	fprintf(file_dot, "value - %d, number - %d", stack->Value, stack->Number);
	fprintf(file_dot, "\"]\n");
	if (stack->Next)
	{
		fprintf(file_dot, "\tN%d->N%d\n", stack->Number, stack->Next->Number);
		NodeToDot(stack->Next);
	}
}

Data PopStack(Node ** stack)
{
	assert(&stack);

	Node * cur = *stack;
	Data value = cur->Value;
	*stack = cur->Next;

	free(cur);

	return value;
}

Data CopyTopStack(Node * stack)
{
	return stack->Value;
}

int SizeStack(Node * stack)
{
	if(!stack)
		return 0;
	else
		return stack->Number;
}

int IsEmptyStack(Node * stack)
{
	return stack == NULL;
}

void ManualStack()
{
	printf("\n");
	printf("---|Manual for stack|---\n");
	printf("'end'\t\t--\texit\n");
	printf("'pop'\t\t--\tget the top\n");
	printf("'print'\t\t--\tprint stack\n");
	printf("'size'\t\t--\tprint size of stack\n");
	printf("'dot'\t\t--\tprint to dot\n");
	printf("'push n'\t--\tput on the stack\n");
	printf("'top'\t\t--\tprint the top of the stack\n");
	printf("---|Manual for stack|---\n");
	printf("\n");
}

void DeleteStack(Node * stack)
{
	if(stack)
	{
		if(stack->Next)
			DeleteStack(stack->Next);
		free(stack);
	}
}

void UserQuery(Node * stack)
{
	char end[] = "end";
	char print[] = "print";
	char size[] = "size";
	char pop[] = "pop";
	char push[] = "push";
	char dot[] = "dot";
	char top[] = "top";

	char * word = calloc(WordSize, sizeof(char));
	int n;

	scanf("%s", word);

	while(strcmp(word, end))
	{
		if(!strcmp(word, print))
		{
			if (IsEmptyStack(stack))
				printf("Empty\n");
			else
			{
				PrintStack(stack);
				printf("\n");
			}
		}
		else if(!strcmp(word, dot))
		{
			if (IsEmptyStack(stack))
				printf("Empty\n");
			else
				DotStack(stack);
		}
		else if(!strcmp(word, size))
		{
			printf("%d\n", SizeStack(stack));
		}
		else if(!strcmp(word, pop))
		{
			if (IsEmptyStack(stack))
				printf("Empty\n");
			else
				printf("%d\n", PopStack(&stack));
		}
		else if(!strcmp(word, push))
		{
			scanf("%d", &n);
			stack = PushStack(stack, n);
		}
		else if(!strcmp(word, top))
		{
			if (IsEmptyStack(stack))
				printf("Empty\n");
			else
				printf("%d\n", CopyTopStack(stack));
		}
		else
		{
			printf("Wrond command\n");
			ManualStack();
		}

		scanf("%s", word);
	}

	free(word);
}
