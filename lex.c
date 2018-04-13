int SizeOfFile(FILE * file);
Token * CreateToken(Token * tokens, int type, int value);
void DotToken(Token * tokens);

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
	printf("%s\n\n", str);
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
	Token * tokens = NULL;

	while(position <= size)
	{
		//---|BIN_OPERATORS|---
		if(str[position] == '+')
		{
			CreateToken(tokens, BIN_OPERATOR, PLUS);
			position++;
		}
		if(str[position] == '-')
		{
			CreateToken(tokens, BIN_OPERATOR, MINUS);
			position++;
		}
		if(str[position] == '*')
		{
			CreateToken(tokens, BIN_OPERATOR, MULTIPLY);
			position++;
		}
		if(str[position] == '/')
		{
			CreateToken(tokens, BIN_OPERATOR, DIVIDE);
			position++;
		}
		//---|OPERATORS|---
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		if()
		{
			
			CreateToken(tokens, OPERATOR, );
		}
		//---|REGISTERS|---
		if()
		{
			
			CreateToken(tokens, REGISTER, );
		}
		if()
		{
			
			CreateToken(tokens, REGISTER, );
		}
		if()
		{
			
			CreateToken(tokens, REGISTER, );
		}
		if()
		{
			
			CreateToken(tokens, REGISTER, );
		}
		//---|FUNCTIONS|---
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
		}	
	}

	printf("---|Лексический анализ завершен|---\n");
	printf("---|Вывод в Dot|---\n\n");
	DotToken(tokens);
	printf("\n---|Вывод в Dot завершен успешно|---\n");

	return Token;
}

void DotToken(Token * tokens)
{
	

}

Token * CreateToken(Token * tokens, int type, int value)
{
	Token * new_token = calloc(1, sizeof(Token));

	if(!tokens)
	{
		new_token->Next = tokens;
		new_token->Type - type;
		new_token->Value = value;
	}
	else
		new_token = CreateToken(tokens->Next, type, value);

	return new_token;
}





