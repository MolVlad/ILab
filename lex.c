FILE * file_dot_token;
int dot_token_counter;
char * dot_semantics;

int SizeOfFile(FILE * file);
Token * CreateToken(Token * tokens, int type, int value);
void TokenToDot(Token * token);
void DotToken(Token * tokens);
char * GetSemantics(Token * token);

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
//	fclose(file);

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
	Token * tokens = NULL;

	while(position < size)
	{
		if(str[position] == '\n' || str[position] == ' ')
			position++;
		else if(str[position] == '(' || str[position] == ')')
			position++;
//---|BIN_OPERATORS|---
		else if(str[position] == '+')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, PLUS);
			position++;
		}
		else if(str[position] == '-')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, MINUS);
			position++;
		}
		else if(str[position] == '*')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, MULTIPLY);
			position++;
		}
		else if(str[position] == '/')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, DIVIDE);
			position++;
		}
		else if(str[position] == '^')
		{
			tokens = CreateToken(tokens, BIN_OPERATOR, DEGREE);
			position++;
		}
//---|OPERATORS|---
		else if(str[position] == 's' && str[position + 1] == 'i' && str[position + 2] == 'n')
		{
			position += 3;
			assert(str[position] == '(');
			tokens = CreateToken(tokens, OPERATOR, SIN);
		}
		else if(str[position] == 'c' && str[position + 1] == 'o' && str[position + 2] == 's')
		{
			position += 3;
			assert(str[position] == '(');
			tokens = CreateToken(tokens, OPERATOR, COS);
		}
		else if(str[position] == 't' && str[position + 1] == 'g')
		{
			position += 2;
			assert(str[position] == '(');
			tokens = CreateToken(tokens, OPERATOR, TG);
		}
		else if(str[position] == 'l' && str[position + 1] == 'n')
		{
			position += 2;
			assert(str[position] == '(');
			tokens = CreateToken(tokens, OPERATOR, LN);
		}
//---|REGISTERS|---
		else if(str[position] == 'r'&& str[position + 1] == 'a' && str[position + 2] == 'x'
				&& (str[position + 3] == ' ' || str[position + 3] == '\n' || str[position] == ')'))
		{
			position += 3;
			tokens = CreateToken(tokens, REGISTER, RAX);
		}
		else if(str[position] == 'r'&& str[position + 1] == 'b' && str[position + 2] == 'x'
				&& (str[position + 3] == ' ' || str[position + 3] == '\n' || str[position] == ')'))
		{
			position += 3;
			tokens = CreateToken(tokens, REGISTER, RBX);
		}
		else if(str[position] == 'r'&& str[position + 1] == 'c' && str[position + 2] == 'x'
				&& (str[position + 3] == ' ' || str[position + 3] == '\n' || str[position] == ')'))
		{
			position += 3;
			tokens = CreateToken(tokens, REGISTER, RCX);
		}
		else if(str[position] == 'r'&& str[position + 1] == 'd' && str[position + 2] == 'x'
				&& (str[position + 3] == ' ' || str[position + 3] == '\n' || str[position] == ')'))
		{
			position += 3;
			tokens = CreateToken(tokens, REGISTER, RDX);
		}
		else
			assert(!"wtf");
/*//---|FUNCTIONS|---
		if()
		{
			
			CreateToken(tokens, FUNCTION, );
		}
		if()
		{
			
			CreateToken(tokens, FUNCTION, );
		}
		if()
		{
			
			CreateToken(tokens, FUNCTION, );
		}
		if()
		{
			
			CreateToken(tokens, FUNCTION, );
		}
		if()
		{
			
			CreateToken(tokens, FUNCTION, );
		}
		if()
		{
			
			CreateToken(tokens, FUNCTION, );
		}
		if()
		{
			
			CreateToken(tokens, FUNCTION, );
		}
		if()
		{
			
			CreateToken(tokens, FUNCTION, );
		}*/
	}

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
	TokenToDot(tokens);
	fprintf(file_dot_token, "}");
	fclose(file_dot_token);

	system("dot -v -Tpng -o tokens tokens.dot");
	system("rm tokens.dot");
}

void TokenToDot(Token * token)
{
	assert(token);

	dot_semantics = GetSemantics(token);
	fprintf(file_dot_token, "\tN%d [label=\"", dot_token_counter);
	fprintf(file_dot_token, "%s", dot_semantics);
	fprintf(file_dot_token, "\"]\n");
	if (token->Next)
	{
		fprintf(file_dot_token, "\tN%d->N%d\n", dot_token_counter, dot_token_counter+1);
		dot_token_counter++;
		TokenToDot(token->Next);
	}
}

char * GetSemantics(Token * token)
{
	switch(token->Type)
	{
		case BIN_OPERATOR:
			switch(token->Value)
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
				default:
					assert(!"wtf");
			}
			break;
		case OPERATOR:
			switch(token->Value)
			{
				case SIN:
					return "sin";
				case COS:
					return "cos";
				case TG:
					return "TG";
				case LN:
					return "LN";
				default:
					assert(!"wtf");
			}
			break;
		case REGISTER:
			switch(token->Value)
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
			break;
		default:
			assert(!"wtf");
	}
}

Token * CreateToken(Token * tokens, int type, int value)
{
	if(!tokens)
	{
		tokens = calloc(1, sizeof(Token));
		tokens->Next = NULL;
		tokens->Type - type;
		tokens->Value = value;

		return tokens;
	}
	else
		tokens->Next = CreateToken(tokens->Next, type, value);

		return tokens;
}





