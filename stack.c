#define WordSize 30

FILE * file_dot_stack;

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
		return stack;
	}
	else
	{
		Node * new = calloc(1, sizeof(Node));
		new->Value = value;
		new->Next = stack;
		new->Number = stack->Number + 1;
		return new;
	}
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
	file_dot_stack = NULL;

	file_dot_stack = fopen("stack.dot", "w");
	assert(file_dot_stack);
	fprintf(file_dot_stack, "digraph G {\n");
	NodeToDot(stack);
	fprintf(file_dot_stack, "}");
	fclose(file_dot_stack);

	system("dot -v -Tpng -o stack stack.dot");
	system("rm stack.dot");
	printf("\n---|Вывод в Dot завершен|---\n");
}

void NodeToDot(Node * stack)
{
	assert(stack);

	fprintf(file_dot_stack, "\tN%d [label=\"", stack->Number);
	fprintf(file_dot_stack, "value - %d, number - %d", stack->Value, stack->Number);
	fprintf(file_dot_stack, "\"]\n");
	if (stack->Next)
	{
		fprintf(file_dot_stack, "\tN%d->N%d\n", stack->Number, stack->Next->Number);
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
