void ErrorMessage(Token * tokens)
{
	printf("Error: %d string, %d word\n", tokens[pos_check].Num_Str, tokens[pos_check].Num_Word);
	assert(!"Error");
}

int IsNewLine(Token * tokens)
{
	return tokens[pos_check].Type == PUNCTUATION && tokens[pos_check].Value == NEW_LINE;
}

void Check(Token * tokens)
{
	printf("\n---|Проверка корректности программы|---\n");

	pos_check = 0;

	while(tokens[pos_check].Type != END)
	{
		switch(tokens[pos_check].Type)
		{
			case PUNCTUATION:
				switch (tokens[pos_check].Value)
				{
					case NEW_LINE:
						break;
					default:
						ErrorMessage(tokens);
				}
				break;
			case REGISTER:
				pos_check++;
				if(tokens[pos_check].Type != PUNCTUATION || tokens[pos_check].Value != EQUALITY)
					ErrorMessage(tokens);
				pos_check++;
				Check_GetG(tokens);
				pos_check++;
				if(!IsNewLine(tokens))
					ErrorMessage(tokens);
				break;
			case FUNCTION:
				switch (tokens[pos_check].Value)
				{
					case IN:
						pos_check++;
						if(tokens[pos_check].Type != REGISTER)
							ErrorMessage(tokens);
						break;
					case OUT:
						pos_check++;
						if(tokens[pos_check].Type != REGISTER)
							ErrorMessage(tokens);
						break;
					case PUSH:
						pos_check++;
						if(tokens[pos_check].Type != REGISTER && tokens[pos_check].Type != NUMBER)
							ErrorMessage(tokens);
						break;
					case POP:
						pos_check++;
						if(tokens[pos_check].Type != REGISTER)
							ErrorMessage(tokens);
						break;
					default:
						break;
				}
				pos_check++;
			break;
		}

		if(!IsNewLine(tokens))
			ErrorMessage(tokens);
		pos_check++;
	}

	printf("---|Программа написана корректно|---\n");
}

int Check_GetG(Token * tokens)
{
//	int ret = GetE(tokens);
	return 1;
}
/*
int GetE(Token * tokens)
{
	int ret = GetT(tokens);

	while(IsPlusMinus(tokens))
	{
		Token * operator = PopToken(&tokens);
		if(operator->Value == PLUS)
			ret = ret + GetT(tokens);
		else
			ret = ret - GetT(tokens);
	}

	return ret;
}

int GetT(Token * tokens)
{
	int ret = GetP(tokens);

	while(IsMulDivDegr(tokens))
	{
		Token * operator = PopToken(&tokens);
		if(operator->Value == MULTIPLY)
			ret = ret * GetP(tokens);
		else if(operator->Value == DIVIDE)
			ret = ret / GetP(tokens);
		else
			ret = ret / GetN(tokens);
	}

	return ret;

}

int GetP(Token * tokens)
{
	int ret;

	if(tokens->Type == PUNCTUATION && tokens->Value == BRACKET_OPENING)
	{
		PopToken(tokens);
		ret = GetE(tokens);
		Token * next = PopToken(&tokens);
		assert(next->Type == PUNCTUATION && next->Value == BRACKET_CLOSE);
	}
	else if(tokens->Type == REGISTER)
	{
		switch (tokens->Value)
		{
			case RAX:
				ret = rax;
				break;
			case RBX:
				ret = rbx;
				break;
			case RCX:
				ret = rcx;
				break;
			case RDX:
				ret = rdx;
			default:
				assert(!"error");
		}
	}
	else
		ret = GetN(tokens);

	return ret;
}

int GetN(Token * tokens)
{
	tokens = PopToken(tokens);
	return 0;
	int number = 0;
	int count = 0;

	while (tokens->Type == NUMBER)
	{
		number = number * 10 + tokens->Value;
		tokens = PopToken(&tokens);
		count++;
	}

	assert(count);
	return number;
}
*/
