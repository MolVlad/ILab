#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//---|для стека|---

#define WordSize 30

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
void DeleteStack(Node * stack);
Node * MulStack(Node * stack);
Node * AddStack(Node * stack);
Node * SubStack(Node * stack);
Node * DivStack(Node * stack);
int SizeStack(Node * stack);

//---|для лексического анализа|---

#define Plus 1
#define Minus 0

enum {BIN_OPERATOR, NUMBER, REGISTER, PUNCTUATION, FUNCTION, END};

enum {PLUS, MINUS, DIVIDE, MULTIPLY, DEGREE};			// BIN_OPERATOR
enum {RAX, RBX, RCX, RDX};					// REGISTER
enum {IN, OUT, PUSH, POP, ADD, SUB, MUL, DIV};			// FUNCTION
enum {BRACKET_OPENING, BRACKET_CLOSE, NEW_LINE, EQUALITY};	//PUNCTUATION

typedef struct
{
	int Type;
	int Value;
	int Num_Str;
	int Num_Word;
} Token;

FILE * file_dot_token;
int dot_token_counter;
int num_str, num_word, size_tokens_cur, size_tokens_max;
const int initial_quality = 5;

char * FileToStr();
Token * LexicalAnalysis(char * str);
int SizeOfFile(FILE * file);
void TokensToDot(Token * token);
void DotToken(Token * tokens);
char * GetSemantics(Token * token, int pos);
Token * CreateToken(Token * tokens, int type, int value);

//---|для проверки корректности|---

int position;

void Check(Token * tokens);
void ErrorMessage(Token * tokens);
void Check_G(Token * tokens);
void Check_E(Token * tokens);
void Check_T(Token * tokens);
void Check_P(Token * tokens);
void Check_N(Token * tokens);

//---|для запуска программы|---

Node * stack_pointer;
int rax, rbx, rcx, rdx;

void Run(Token * tokens);
int Get_G(Token * tokens);
int Get_E(Token * tokens);
int Get_T(Token * tokens);
int Get_P(Token * tokens);
int Get_N(Token * tokens);

int main()
{

	char * str = FileToStr();
	Token * tokens = LexicalAnalysis(str);
	Check(tokens);
	Run(tokens);

	return 0;
}

//---|для стека
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
	if(SizeStack(stack) < 2)
	{
		assert(!"Not enough in stack");
	}
	else
	{
		Data a = PopStack(&stack);
		Data b = PopStack(&stack);
		stack = PushStack(stack, a * b);
		printf("%d * %d = %d to stack\n", a, b, a * b);
	}

	return stack;
}

Node * AddStack(Node * stack)
{
	if(SizeStack(stack) < 2)
	{
		assert(!"Not enough in stack");
	}
	else
	{
		Data a = PopStack(&stack);
		Data b = PopStack(&stack);
		stack = PushStack(stack, a + b);

		printf("%d + %d = %d to stack\n", a, b, a + b);
	}

	return stack;
}

Node * SubStack(Node * stack)
{
	if(SizeStack(stack) < 2)
	{
		assert(!"Not enough in stack");
	}
	else
	{
		Data a = PopStack(&stack);
		Data b = PopStack(&stack);
		stack = PushStack(stack, a - b);
		printf("%d - %d = %d to stack\n", a, b, a - b);
	}

	return stack;
}

Node * DivStack(Node * stack)
{
	if(SizeStack(stack) < 2)
	{
		assert(!"Not enough in stack");
	}
	else
	{
		Data a = PopStack(&stack);
		Data b = PopStack(&stack);
		stack = PushStack(stack, a % b);
		printf("modulo %d / %d = %d to stack\n", a, b, a % b);
		stack = PushStack(stack, a / b);
		printf("the whole part %d / %d = %d to stack\n", a, b, a / b);
	}

	return stack;
}

Data PopStack(Node ** stack)
{
	assert(&stack);

	if(!*stack)
		assert(!"Empty stack!");

	Node * cur = *stack;
	Data value = cur->Value;
	*stack = cur->Next;

	free(cur);

	return value;
}

int SizeStack(Node * stack)
{
	if(!stack)
		return 0;
	else
		return stack->Number;
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

//---|для лексического анализа|---
char * FileToStr()
{
	printf("---|Считывание текста программы|---\n");
	FILE * file = NULL;
	file = fopen("programm.txt", "r");
	assert(file);

	int n = SizeOfFile(file);
	assert(n);
	printf("Размер файла - %d символов\n", n);
	char * str = (char *) calloc(n+1, sizeof(char));
	assert(str);
	fscanf(file, "%[^EOF]", str);
	assert(str);
	printf("Считывание прошло нормально\n");
	printf("Размер программы - %lu символов\n", strlen(str));
	printf("---|Исходный текст программы|---\n");
	printf("%s", str);
	printf("---|Исходный текст программы|---\n\n");
	fclose(file);

	return str;
}

int SizeOfFile(FILE * file)
{
	assert(file);
	fseek(file, 0, SEEK_END);
	int n = 0;
	n = ftell(file);
	assert(n);
	fseek(file, 0, SEEK_SET);
	assert(!ftell(file));

	return n;
}

Token * LexicalAnalysis(char * str)
{
	printf("---|Лексический анализ текста программы|---\n");

	int position = 0;
	int size = strlen(str);
	int num;
	assert(size);

	Token * tokens = NULL;
	size_tokens_cur = 0;
	size_tokens_max = 0;
	num_str = 1;
	num_word = 1;

	while(position < size)
	{
	//---|NUMBERS|---
		if('0' <= str[position] && str[position] <= '9')
		{
			num = 0;
			while('0' <= str[position] && str[position] <= '9')
			{
				num = num * 10 + str[position] - '0';
				position++;
			}

			tokens = CreateToken(tokens, NUMBER, num);
			num_word++;
		}
	//---|BIN_OPERATORS|---
		else if(str[position] == '+')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, PLUS);
			num_word++;
			position++;
		}
		else if(str[position] == '-')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, MINUS);
			num_word++;
			position++;
		}
		else if(str[position] == '*')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, MULTIPLY);
			num_word++;
			position++;
		}
		else if(str[position] == '/')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, DIVIDE);
			num_word++;
			position++;
		}
		else if(str[position] == '^')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, DEGREE);
			num_word++;
			position++;
		}
		//---|REGISTERS|---
		else if(str[position] == 'r'&& str[position + 1] == 'a' && str[position + 2] == 'x'
				&& (str[position + 3] == ' ' || str[position + 3] == '\n' || str[position + 3] == ')'))
		{
			position += 3;
			tokens = CreateToken(tokens, REGISTER, RAX);
			num_word++;
		}
		else if(str[position] == 'r'&& str[position + 1] == 'b' && str[position + 2] == 'x'
				&& (str[position + 3] == ' ' || str[position + 3] == '\n' || str[position + 3] == ')'))
		{
			position += 3;
			tokens = CreateToken(tokens, REGISTER, RBX);
			num_word++;
		}
		else if(str[position] == 'r'&& str[position + 1] == 'c' && str[position + 2] == 'x'
				&& (str[position + 3] == ' ' || str[position + 3] == '\n' || str[position + 3] == ')'))
		{
			position += 3;
			tokens = CreateToken(tokens, REGISTER, RCX);
			num_word++;
		}
		else if(str[position] == 'r'&& str[position + 1] == 'd' && str[position + 2] == 'x'
				&& (str[position + 3] == ' ' || str[position + 3] == '\n' || str[position + 3] == ')'))
		{
			position += 3;
			tokens = CreateToken(tokens, REGISTER, RDX);
			num_word++;
		}
		//---|PUNCTUATION|---
		else if(str[position] == '(')
		{
			position++;
			tokens = CreateToken(tokens, PUNCTUATION, BRACKET_OPENING);
			num_word++;
		}
		else if(str[position] == ')')
		{
			position++;
			tokens = CreateToken(tokens, PUNCTUATION, BRACKET_CLOSE);
			num_word++;
		}
		else if(str[position] == '\n')
		{
			position++;
			tokens = CreateToken(tokens, PUNCTUATION, NEW_LINE);
			num_str++;
			num_word = 1;
		}
		else if(str[position] == '=')
		{
			position++;
			tokens = CreateToken(tokens, PUNCTUATION, EQUALITY);
			num_word++;
		}
		else if(str[position] == ' ')
		{
			position++;
		}
		//---|FUNCTIONS|---
		else if(str[position] == 'i' && str[position + 1] == 'n'
				&& (str[position + 2] == '\n' || str[position + 2] == ' '))
		{
			position += 2;
			tokens = CreateToken(tokens, FUNCTION, IN);
			num_word++;
		}
		else if(str[position] == 'o' && str[position + 1] == 'u' && str[position + 2] == 't'
				&& (str[position + 3] == '\n' || str[position + 3] == ' '))
		{
			position += 3;
			tokens = CreateToken(tokens, FUNCTION, OUT);
			num_word++;
		}
		else if(str[position] == 'p' && str[position + 1] == 'u' && str[position + 2] == 's' && str[position + 3] == 'h'
				&& (str[position + 4] == '\n' || str[position + 4] == ' '))
		{
			position += 4;
			tokens = CreateToken(tokens, FUNCTION, PUSH);
			num_word++;
		}
		else if(str[position] == 'p' && str[position + 1] == 'o' && str[position + 2] == 'p'
				&& (str[position + 3] == '\n' || str[position + 3] == ' '))
		{
			position += 4;
			tokens = CreateToken(tokens, FUNCTION, POP);
			num_word++;
		}
		else if(str[position] == 'a' && str[position + 1] == 'd' && str[position + 2] == 'd'
				&& (str[position + 3] == '\n' || str[position + 3] == ' '))
		{
			position += 3;
			tokens = CreateToken(tokens, FUNCTION, ADD);
			num_word++;
		}
		else if(str[position] == 's' && str[position + 1] == 'u' && str[position + 2] == 'b'
				&& (str[position + 3] == '\n' || str[position + 3] == ' '))
		{
			position += 3;
			tokens = CreateToken(tokens, FUNCTION, SUB);
			num_word++;
		}
		else if(str[position] == 'm' && str[position + 1] == 'u' && str[position + 2] == 'l'
				&& (str[position + 3] == '\n' || str[position + 3] == ' '))
		{
			position += 3;
			tokens = CreateToken(tokens, FUNCTION, MUL);
			num_word++;
		}
		else if(str[position] == 'd' && str[position + 1] == 'i' && str[position + 2] == 'v'
				&& (str[position + 3] == '\n' || str[position + 3] == ' '))
		{
			position += 3;
			tokens = CreateToken(tokens, FUNCTION, DIV);
			num_word++;
		}
		else
		{
			printf("Error: %d string, %d word\n", num_str, num_word);
			assert(!"wtf");
		}
	}

	tokens = CreateToken(tokens, END, 0);

	free(str);

	printf("---|Лексический анализ завершен|---\n");
	printf("---|Вывод в Dot|---\n\n");
	DotToken(tokens);
	printf("\n---|Вывод в Dot завершен|---\n");

	return tokens;
}

void DotToken(Token * tokens)
{
	assert(tokens);
	file_dot_token = NULL;

	file_dot_token = fopen("tokens.dot", "w");
	assert(file_dot_token);
	fprintf(file_dot_token, "digraph G {\n");
	dot_token_counter = 0;
	TokensToDot(tokens);
	fprintf(file_dot_token, "}");
	fclose(file_dot_token);

	system("dot -v -Tpng -o tokens tokens.dot");
	system("rm tokens.dot");
}

void TokensToDot(Token * tokens)
{
	assert(tokens);

	int pos = 0;

	while(pos < size_tokens_cur)
	{
		if((tokens[pos]).Type == NUMBER)
		{
			fprintf(file_dot_token, "\tN%d [label=\"", dot_token_counter);
			fprintf(file_dot_token, "%d", tokens[pos].Value);
			fprintf(file_dot_token, "\"]\n");
		}
		else
		{
			char * dot_semantics = GetSemantics(tokens, pos);
			fprintf(file_dot_token, "\tN%d [label=\"", dot_token_counter);
			fprintf(file_dot_token, "%s", dot_semantics);
			fprintf(file_dot_token, "\"]\n");
		}

		if (tokens[pos].Type != END)
		{
			fprintf(file_dot_token, "\tN%d->N%d\n", dot_token_counter, dot_token_counter+1);
			dot_token_counter++;
		}

		pos++;
	}
}

char * GetSemantics(Token * token, int pos)
{
	switch(token[pos].Type)
	{
		case BIN_OPERATOR:
			switch(token[pos].Value)
			{
				case PLUS:
					return "+";
				case MINUS:
					return "-";
				case DIVIDE:
					return "/";
				case MULTIPLY:
					return "*";
				case DEGREE:
					return "^";
			}
		case REGISTER:
			switch(token[pos].Value)
			{
				case RAX:
					return "rax";
				case RBX:
					return "rbx";
				case RCX:
					return "rcx";
				case RDX:
					return "rdx";
			}
		case PUNCTUATION:
			switch(token[pos].Value)
			{
				case BRACKET_OPENING:
					return "(";
				case BRACKET_CLOSE:
					return ")";
				case NEW_LINE:
					return "\\\\n";
				case EQUALITY:
					return "=";
			}
		case FUNCTION:
			switch(token[pos].Value)
			{
				case IN:
					return "in";
				case OUT:
					return "out";
				case PUSH:
					return "push";
				case POP:
					return "pop";
				case ADD:
					return "add";
				case SUB:
					return "sub";
				case MUL:
					return "mul";
				case DIV:
					return "div";
			}
		case END:
			return "END";
	}
}

Token * CreateToken(Token * tokens, int type, int value)
{
	if(size_tokens_cur == 0)
	{
		tokens = calloc(initial_quality, sizeof(Token));
		size_tokens_max = initial_quality;
	}

	if(size_tokens_cur == size_tokens_max)
	{
		tokens = realloc(tokens, size_tokens_max * 2 * sizeof(Token));
		size_tokens_max *= 2;
	}

	int pos = size_tokens_cur;

	tokens[pos].Type = type;
	tokens[pos].Value = value;
	tokens[pos].Num_Str = num_str;
	tokens[pos].Num_Word = num_word;

	size_tokens_cur++;

	return tokens;
}

//---|для проверки корректности|---

void ErrorMessage(Token * tokens)
{
	printf("Error: %d string, %d word\n", tokens[position].Num_Str, tokens[position].Num_Word);
	assert(!"Error");
}

int IsNewLine(Token * tokens)
{
	return tokens[position].Type == PUNCTUATION && tokens[position].Value == NEW_LINE;
}

void Check(Token * tokens)
{
	printf("\n---|Проверка корректности программы|---\n");

	position = 0;

	while(tokens[position].Type != END)
	{
		switch(tokens[position].Type)
		{
			case PUNCTUATION:
				switch (tokens[position].Value)
				{
					case NEW_LINE:
						break;
					default:
						ErrorMessage(tokens);
				}
				break;
			case REGISTER:
				position++;
				if(tokens[position].Type != PUNCTUATION || tokens[position].Value != EQUALITY)
					ErrorMessage(tokens);
				position++;
				Check_G(tokens);
				break;
			case FUNCTION:
				switch (tokens[position].Value)
				{
					case IN:
						position++;
						if(tokens[position].Type != REGISTER)
							ErrorMessage(tokens);
						break;
					case OUT:
						position++;
						if(tokens[position].Type != REGISTER)
							ErrorMessage(tokens);
						break;
					case PUSH:
						position++;
						if(tokens[position].Type == BIN_OPERATOR && tokens[position].Value == MINUS)
						{
							position++;
							if(!tokens[position].Type == NUMBER)
							ErrorMessage(tokens);
							position++;
						}
						else if(tokens[position].Type != REGISTER && tokens[position].Type != NUMBER)
							ErrorMessage(tokens);
						break;
					case POP:
						position++;
						if(tokens[position].Type != REGISTER)
							ErrorMessage(tokens);
						break;
					default:
						break;
				}
				position++;
			break;
		}

		if(!IsNewLine(tokens))
			ErrorMessage(tokens);
		position++;
	}

	printf("---|Программа написана корректно|---\n");
}

int IsPlusMinus(Token * tokens)
{
	return tokens[position].Type == BIN_OPERATOR && (tokens[position].Value == PLUS || tokens[position].Value == MINUS);
}

int IsMulDivDegr(Token * tokens)
{
	return tokens[position].Type == BIN_OPERATOR && (tokens[position].Value == MULTIPLY || tokens[position].Value == DIVIDE || tokens[position].Value == DEGREE);
}

void Check_G(Token * tokens)
{
	Check_E(tokens);
	if(!IsNewLine(tokens))
		ErrorMessage(tokens);
}

void Check_E(Token * tokens)
{
	Check_T(tokens);

	while(IsPlusMinus(tokens))
	{
		position++;
		Check_T(tokens);
	}
}

void Check_T(Token * tokens)
{
	Check_P(tokens);

	while(IsMulDivDegr(tokens))
	{
		if(tokens[position].Value == DEGREE)
		{
			position++;

			Check_N(tokens);
		}
		else
		{
			position++;
			Check_P(tokens);
		}
	}
}

void Check_P(Token * tokens)
{
	if(tokens[position].Type == PUNCTUATION && tokens[position].Value == BRACKET_OPENING)
	{
		position++;
		Check_E(tokens);
		if(!(tokens[position].Type == PUNCTUATION && tokens[position].Value == BRACKET_CLOSE))
			ErrorMessage(tokens);
		position++;
	}
	else if(tokens[position].Type == REGISTER)
		position++;
	else
		Check_N(tokens);
}

void Check_N(Token * tokens)
{
	if(tokens[position].Type == NUMBER)
		position++;
	else if(tokens[position].Type == BIN_OPERATOR && tokens[position].Value == MINUS)
	{
		position++;
		if(tokens[position].Type != NUMBER)
			ErrorMessage(tokens);
		position++;
	}
	else
		ErrorMessage(tokens);
}

//---|для запуска программы|---

void Run(Token * tokens)
{
	stack_pointer = CreateStack();
	rax = rbx = rcx = rdx = 0;

	printf("\n---|Выполнение программы|---\n");

	position = 0;

	while(tokens[position].Type != END)
		switch(tokens[position].Type)
		{
			case PUNCTUATION:
				if(tokens[position].Value == NEW_LINE)
					position++;
				break;
			case REGISTER:
				switch (tokens[position].Value)
				{
					case RAX:
						position += 2;
						printf("rax = ");
						rax = Get_G(tokens);
						printf(" = %d\n", rax);
						break;
					case RBX:
						position += 2;
						printf("rbx = ");
						rbx = Get_G(tokens);
						printf(" = %d\n", rbx);
						break;
					case RCX:
						position += 2;
						printf("rcx = ");
						rcx = Get_G(tokens);
						printf(" = %d\n", rcx);
						break;
					case RDX:
						position += 2;
						printf("rdx = ");
						rdx = Get_G(tokens);
						printf(" = %d\n", rdx);
						break;
				}

				break;
			case FUNCTION:
				switch (tokens[position].Value)
				{
					case IN:
						position++;

						switch (tokens[position].Value)
						{
							case RAX:
								printf("Enter rax:\n");
								scanf("%d", &rax);
								break;
							case RBX:
								printf("Enter rbx:\n");
								scanf("%d", &rbx);
								break;
							case RCX:
								printf("Enter rcx:\n");
								scanf("%d", &rcx);
								break;
							case RDX:
								printf("Enter rdx:\n");
								scanf("%d", &rdx);
								break;
						}

						position++;
						break;
					case OUT:
						position++;

						switch (tokens[position].Value)
						{
							case RAX:
								printf("Value rax:\n");
								printf("%d\n", rax);
								break;
							case RBX:
								printf("Value rbx:\n");
								printf("%d\n", rbx);
								break;
							case RCX:
								printf("Value rcx:\n");
								printf("%d\n", rcx);
								break;
							case RDX:
								printf("Value rdx:\n");
								printf("%d\n", rdx);
								break;
						}

						position++;
						break;
					case PUSH:
						position++;

						if(tokens[position].Type == NUMBER)
						{
							int num = tokens[position].Value;
							stack_pointer = PushStack(stack_pointer, num);
							printf("%d to stack\n", num);
						}
						else
							switch (tokens[position].Value)
							{
								case RAX:
									stack_pointer = PushStack(stack_pointer, rax);
									printf("rax (%d) to stack\n", rax);
									break;
								case RBX:
									stack_pointer = PushStack(stack_pointer, rbx);
									printf("rbx (%d) to stack\n", rbx);
									break;
								case RCX:
									stack_pointer = PushStack(stack_pointer, rcx);
									printf("rcx (%d) to stack\n", rcx);
									break;
								case RDX:
									stack_pointer = PushStack(stack_pointer, rdx);
									printf("rdx (%d) to stack\n", rdx);
									break;
							}

						position++;
						break;
					case POP:
						position++;

						switch (tokens[position].Value)
						{
							case RAX:
								rax = PopStack(&stack_pointer);
								printf("Top of stack (%d) to rax\n", rax);
								break;
							case RBX:
								rbx = PopStack(&stack_pointer);
								printf("Top of stack (%d) to rbx\n", rbx);
								break;
							case RCX:
								rcx = PopStack(&stack_pointer);
								printf("Top of stack (%d) to rcx\n", rcx);
								break;
							case RDX:
								rdx = PopStack(&stack_pointer);
								printf("Top of stack (%d) to rdx\n", rdx);
								break;
						}

						position++;
						break;
					case ADD:
						stack_pointer = AddStack(stack_pointer);
						break;
					case SUB:
						stack_pointer = SubStack(stack_pointer);
						break;
					case DIV:
						stack_pointer = DivStack(stack_pointer);
						break;
					case MUL:
						stack_pointer = MulStack(stack_pointer);
						break;
				}

			position++;
			break;
		}

	DeleteStack(stack_pointer);
	free(tokens);
	printf("---|Выполнение программы завершено успешно|---\n");
}

int Get_G(Token * tokens)
{
	int ret = Get_E(tokens);

	return ret;
}

int Get_E(Token * tokens)
{
	int ret = Get_T(tokens);

	while(IsPlusMinus(tokens))
	{
		if(tokens[position].Value == PLUS)
		{
			position++;
			printf("+");
			ret = ret + Get_T(tokens);
		}
		else
		{
			position++;
			printf("-");
			ret = ret - Get_T(tokens);
		}
	}

	return ret;
}

int Get_T(Token * tokens)
{
	int ret = Get_P(tokens);

	while(IsMulDivDegr(tokens))
	{
		int operator = tokens[position].Value;
		position++;

		if(operator == MULTIPLY)
		{
			printf("*");
			ret = ret * Get_P(tokens);
		}
		else if(operator == DIVIDE)
		{
			printf("/");
			ret = ret / Get_P(tokens);
		}
		else
		{
			printf("^");
			ret = ret ^ Get_P(tokens);
		}
	}

	return ret;
}

int Get_P(Token * tokens)
{
	int ret;

	if(tokens[position].Type == PUNCTUATION && tokens[position].Value == BRACKET_OPENING)
	{
		printf("(");
		position++;
		ret = Get_E(tokens);
		printf(")");
		position++;
	}
	else if(tokens[position].Type == REGISTER)
	{
		switch (tokens[position].Value)
		{
			case RAX:
				printf("rax(= %d)", rax);
				ret = rax;
				break;
			case RBX:
				printf("rbx(= %d)", rbx);
				ret = rbx;
				break;
			case RCX:
				printf("rcx(= %d)", rcx);
				ret = rcx;
				break;
			case RDX:
				printf("rdx(= %d)", rdx);
				ret = rdx;
				break;
		}

		position++;
	}
	else
	{
		ret = Get_N(tokens);
	}

	return ret;
}

int Get_N(Token * tokens)
{
	int ret;

	if(tokens[position].Type == NUMBER)
	{
		ret = tokens[position].Value;
		position++;
		printf("%d", ret);
	}
	else
	{
		position++;
		ret = (-1) * tokens[position].Value;
		position++;
		printf("%d", ret);
	}

	return ret;
}
