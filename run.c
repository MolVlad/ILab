int GetG(Token * tokens);
int IsEquality(Token * token);
int IsEnd(Token * tokens);

int IsEquality(Token * token)
{
	return token->Type == PUNCTUATION && token->Value == EQUALITY;
}

int IsEnd(Token * tokens)
{
	return tokens == NULL;
}

int IsNewStr(Token * tokens)
{
	return tokens->Type == PUNCTUATION && tokens->Value == NEW_LINE;
}

void Run(Token * tokens)
{
	Token * cur = PopToken(&tokens);

	Token * object;

	while(!IsEnd(cur))
	{
		switch(cur->Type)
		{
			case PUNCTUATION:
				break;
			case REGISTER:
				assert(IsEquality(PopToken(&tokens)));
				switch (cur->Value)
				{
					case RAX:
						printf("rax = ");
						rax = GetG(tokens);
						printf(" = %d\n", rax);
						break;
					case RBX:
						printf("rbx = ");
						rbx = GetG(tokens);
						printf(" = %d\n", rbx);
						break;
					case RCX:
						printf("rcx = ");
						rcx = GetG(tokens);
						printf(" = %d\n", rcx);
						break;
					case RDX:
						printf("rdx = ");
						rdx = GetG(tokens);
						printf(" = %d\n", rdx);
						break;
				}

				assert(IsNewStr(PopToken(&tokens)));
				break;
			case FUNCTION:
				switch (cur->Value)
				{
					case IN:
						object = PopToken(&tokens);
						assert(object->Type == REGISTER);
						int num;

						switch(object->Value)
						{
							case RAX:
								printf("scan rax:\n");
								scanf("%d", &num);
								rax = num;
								printf("rax = %d\n", rax);
								break;
							case RBX:
								printf("scan rbx:\n");
								scanf("%d", &num);
								rbx = num;
								printf("rbx = %d\n", rbx);
								break;
							case RCX:
								printf("scan rcx:\n");
								scanf("%d", &num);
								rcx = num;
								printf("rcx = %d\n", rcx);
								break;
							case RDX:
								printf("scan rdx:\n");
								scanf("%d", &num);
								rdx = num;
								printf("rdx = %d\n", rdx);
								break;
							default:
								assert(!"error");
						}

						assert(IsNewStr(PopToken(&tokens)));
						break;
					case OUT:
						object = PopToken(&tokens);
						assert(object->Type == REGISTER);

						switch(object->Value)
						{
							case RAX:
								printf("value of rax:\n%d\n", rax);
								break;
							case RBX:
								printf("value of rbx:\n%d\n", rbx);
								break;
							case RCX:
								printf("value of rcx:\n%d\n", rcx);
								break;
							case RDX:
								printf("value of rdx:\n%d\n", rdx);
								break;
							default:
								assert(!"error");
						}

						assert(IsNewStr(PopToken(&tokens)));
						break;
					case PUSH:
						object = PopToken(&tokens);
						if(object->Type == REGISTER)
							switch(object->Value)
							{
								case RAX:
									printf("rax (%d) to stack\n", rax);
									PushStack(sp, rax);
									break;
								case RBX:
									printf("rbx (%d) to stack\n", rbx);
									PushStack(sp, rbx);
									break;
								case RCX:
									printf("rcx (%d) to stack\n", rcx);
									PushStack(sp, rcx);
									break;
								case RDX:
									printf("rdx (%d) to stack\n", rdx);
									PushStack(sp, rdx);
									break;
								default:
									assert(!"error");
							}
						else if(object->Type == NUMBER)
						{
							printf("%d to stack\n", object->Value);
							PushStack(sp, object->Value);
						}
						else
							assert(!"error");

						assert(IsNewStr(PopToken(&tokens)));
						break;
					default:
						assert(!"error");
				}
			break;
		}

		cur = PopToken(&tokens);
	}
}

int GetG(Token * tokens)
{
	return 1;
}
