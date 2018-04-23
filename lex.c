#define Plus 1
#define Minus 0

FILE * file_dot_token;
int dot_token_counter;
int num_str, num_word, size_tokens_cur, size_tokens_max;
const int initial_quality = 5;

int SizeOfFile(FILE * file);
void TokensToDot(Token * token);
void DotToken(Token * tokens);
char * GetSemantics(Token * token, int pos);
Token * CreateToken(Token * tokens, int type, int value);

char * FileToStr()
{
	printf("---|Считывание текста программы|---\n");
	FILE * file = NULL;
	file = fopen("programm.txt", "r");
	assert(file);

	int n = SizeOfFile(file);
	assert(n);
	printf("Размер файла - %d символов\n", n);
	char * str = (char *) calloc(n, sizeof(char));
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
			int num = 0;
			while('0' <= str[position] && str[position] <= '9')
			{
				num = num * 10 + str[position] - '0';
				position++;
			}

			tokens = CreateToken(tokens, NUMBER, num);
			num_word++;
		}
		else if((str[position] == '-') && ('0' <= str[position + 1]) && (str[position + 1] <= '9'))
		{
			position++;
			int num = 0;
			while('0' <= str[position] && str[position] <= '9')
			{
				num = num * 10 + str[position] - '0';
				position++;
			}

			num *= -1;

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
		}
		else if(str[position] == ')')
		{
			position++;
			tokens = CreateToken(tokens, PUNCTUATION, BRACKET_CLOSE);
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


