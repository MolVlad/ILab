#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define WordSize 30

FILE * file_dot;

typedef char * Data;
typedef struct Node
{
	int Number;
	struct Node * Next;
	struct Node * Prev;
	Data Value;
} Node;

//создать еще одну структуру которая будет содержать размер, хвост, голову, а переменные list станут node
//на вечерок дел в общем

Node * CreateList();
Node * PushListBegin(Node * list, Data value);
Node * PushListEnd(Node * list, Data value);
Data PopListBegin(Node ** list);
Data PopListEnd(Node ** list);
void PrintListBegin(Node * list);
void PrintListEnd(Node * list);
int SizeList(Node * list);
void DotList(Node * list);
void NodeToDot(Node * list);
Data CopyTopList(Node * list);
Data CopyDownList(Node * list);
int IsEmptyList(Node * list);
void ManualList();
void UserQuery(Node * list);
void DeleteList(Node * list);

int main()
{
	Node * list = CreateList();

	ManualList();
	UserQuery(list);

	DeleteList(list);

	return 0;
}

Node * CreateList()
{
	return NULL;
}

Node * PushList(Node * list, Data value)
{
	if (list == NULL)
	{
		list = calloc(1, sizeof(Node));
		list->Next = NULL;
		list->Prev = NULL;
		list->Value = value;
		list->Number = 1;
	}
	else
	{
		list->Next =  PushList(list->Next, value);
		list->Number++;
	}

	return list;
}

void PrintList(Node * list)
{
	assert(list);

	printf("%d", list->Value);
	if (list->Next)
	{
		printf(" ");
		PrintList(list->Next);
	}
}

Node * MulList(Node * list)
{
	if(IsEmptyList(list))
	{
		printf("Empty\n");
	}
	else
	{
		Data a = PopList(&list);
		Data b = PopList(&list);
		list = PushList(list, a * b);
	}

	return list;
}

Node * AddList(Node * list)
{
	if(IsEmptyList(list))
	{
		printf("Empty\n");
	}
	else
	{
		Data a = PopList(&list);
		Data b = PopList(&list);
		list = PushList(list, a + b);
	}

	return list;
}

Node * SubList(Node * list)
{
	if(IsEmptyList(list))
	{
		printf("Empty\n");
	}
	else
	{
		Data a = PopList(&list);
		Data b = PopList(&list);
		list = PushList(list, a - b);
	}

	return list;
}

Node * DivList(Node * list)
{
	if(IsEmptyList(list))
	{
		printf("Empty\n");
	}
	else
	{
		Data a = PopList(&list);
		Data b = PopList(&list);
		list = PushList(list, a / b);
		list = PushList(list, a % b);
	}

	return list;
}

void DotList(Node * list)
{
	assert(list);
	file_dot = NULL;

	file_dot = fopen("tree.dot", "w");
	assert(file_dot);
	fprintf(file_dot, "digraph G {\n");
	NodeToDot(list);
	fprintf(file_dot, "}");
	fclose(file_dot);

	system("dot -v -Tpng -o tree tree.dot");
	system("rm tree.dot");
	printf("\nSuccesfully\n");
}

void NodeToDot(Node * list)
{
	assert(list);

	fprintf(file_dot, "\tN%d [label=\"", list->Number);
	fprintf(file_dot, "value - %d, number - %d", list->Value, list->Number);
	fprintf(file_dot, "\"]\n");
	if (list->Next)
	{
		fprintf(file_dot, "\tN%d->N%d\n", list->Number, list->Next->Number);
		NodeToDot(list->Next);
	}
}

Data PopList(Node ** list)
{
	assert(&list);

	Node * cur = *list;
	Data value = cur->Value;
	*list = cur->Next;

	free(cur);

	return value;
}

Data CopyTopList(Node * list)
{
	return list->Value;
}

int SizeList(Node * list)
{
	if(!list)
		return 0;
	else
		return list->Number;
}

int IsEmptyList(Node * list)
{
	return list == NULL;
}

void ManualList()
{
	printf("\n");
	printf("---|Manual for list|---\n");
	printf("'end'\t\t--\texit\n");
	printf("'pop'\t\t--\tget the top\n");
	printf("'print'\t\t--\tprint list\n");
	printf("'size'\t\t--\tprint size of list\n");
	printf("'dot'\t\t--\tprint to dot\n");
	printf("'push n'\t--\tput on the list\n");
	printf("'add'\t\t--\tpop two elements, add and push\n");
	printf("'sub'\t\t--\tpop two elements, sub and push\n");
	printf("'mul'\t\t--\tpop two elements, mul and push\n");
	printf("'div'\t\t--\tpop two elements, div, push div, then push mod\n");
	printf("'top'\t\t--\tprint the top of the list\n");
	printf("---|Manual for list|---\n");
	printf("\n");
}

void DeleteList(Node * list)
{
	if(list)
	{
		if(list->Next)
			DeleteList(list->Next);
		free(list);
	}
}

void UserQuery(Node * list)
{
	char end[] = "end";
	char print[] = "print";
	char size[] = "size";
	char pop[] = "pop";
	char push[] = "push";
	char dot[] = "dot";
	char top[] = "top";
	char add[] = "add";
	char sub[] = "sub";
	char mul[] = "mul";
	char div[] = "div";

	char * word = calloc(WordSize, sizeof(char));
	int n;

	scanf("%s", word);

	while(strcmp(word, end))
	{
		if(!strcmp(word, print))
		{
			if (IsEmptyList(list))
				printf("Empty\n");
			else
			{
				PrintList(list);
				printf("\n");
			}
		}
		else if(!strcmp(word, dot))
		{
			if (IsEmptyList(list))
				printf("Empty\n");
			else
				DotList(list);
		}
		else if(!strcmp(word, size))
		{
			printf("%d\n", SizeList(list));
		}
		else if(!strcmp(word, pop))
		{
			if (IsEmptyList(list))
				printf("Empty\n");
			else
				printf("%d\n", PopList(&list));
		}
		else if(!strcmp(word, push))
		{
			scanf("%d", &n);
			list = PushList(list, n);
		}
		else if(!strcmp(word, top))
		{
			if (IsEmptyList(list))
				printf("Empty\n");
			else
				printf("%d\n", CopyTopList(list));
		}
		else if(!strcmp(word, add))
		{
			if (IsEmptyList(list))
				printf("Empty\n");
			else if(list->Number == 1)
				printf("Not enough\n");
			else
				list = AddList(list);
		}
		else if(!strcmp(word, sub))
		{
			if (IsEmptyList(list))
				printf("Empty\n");
			else if(list->Number == 1)
				printf("Not enough\n");
			else
				list = SubList(list);
		}
		else if(!strcmp(word, mul))
		{
			if (IsEmptyList(list))
				printf("Empty\n");
			else if(list->Number == 1)
				printf("Not enough\n");
			else
				list = MulList(list);
		}
		else if(!strcmp(word, div))
		{
			if (IsEmptyList(list))
				printf("Empty\n");
			else if(list->Number == 1)
				printf("Not enough\n");
			else
				list = DivList(list);
		}
		else
		{
			printf("Wrond command\n");
			ManualList();
		}

		scanf("%s", word);
	}

	free(word);
}
