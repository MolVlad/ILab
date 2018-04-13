#define WordSize 30

FILE * file_dot;

void NodeToDot(Node * stack);

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
		stack->Next = PushStack(stack->Next, value);
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

Node * MulStack(Node * stack)
{
	if(IsEmptyStack(stack))
	{
		printf("Empty\n");
	}
	else
	{
		Data a = PopStack(&stack);
		Data b = PopStack(&stack);
		stack = PushStack(stack, a * b);
	}

	return stack;
}

Node * AddStack(Node * stack)
{
	if(IsEmptyStack(stack))
	{
		printf("Empty\n");
	}
	else
	{
		Data a = PopStack(&stack);
		Data b = PopStack(&stack);
		stack = PushStack(stack, a + b);
	}

	return stack;
}

Node * SubStack(Node * stack)
{
	if(IsEmptyStack(stack))
	{
		printf("Empty\n");
	}
	else
	{
		Data a = PopStack(&stack);
		Data b = PopStack(&stack);
		stack = PushStack(stack, a - b);
	}

	return stack;
}

Node * DivStack(Node * stack)
{
	if(IsEmptyStack(stack))
	{
		printf("Empty\n");
	}
	else
	{
		Data a = PopStack(&stack);
		Data b = PopStack(&stack);
		stack = PushStack(stack, a / b);
		stack = PushStack(stack, a % b);
	}

	return stack;
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

void DeleteStack(Node * stack)
{
	if(stack)
	{
		if(stack->Next)
			DeleteStack(stack->Next);
		free(stack);
	}
}
