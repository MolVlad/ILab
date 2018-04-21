Node * sp = NULL;
int rax, rbx, rcx, rdx;

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
								break;
							case RBX:
								printf("scan rbx:\n");
								scanf("%d", &num);
								rbx = num;
								break;
							case RCX:
								printf("scan rcx:\n");
								scanf("%d", &num);
								rcx = num;
								break;
							case RDX:
								printf("scan rdx:\n");
								scanf("%d", &num);
								rdx = num;
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
									sp = PushStack(sp, rax);
									break;
								case RBX:
									printf("rbx (%d) to stack\n", rbx);
									sp = PushStack(sp, rbx);
									break;
								case RCX:
									printf("rcx (%d) to stack\n", rcx);
									sp = PushStack(sp, rcx);
									break;
								case RDX:
									printf("rdx (%d) to stack\n", rdx);
									sp = PushStack(sp, rdx);
									break;
								default:
									assert(!"error");
							}
						else if(object->Type == NUMBER)
						{
							printf("%d to stack\n", object->Value);
							sp = PushStack(sp, object->Value);
						}
						else
							assert(!"error");

						assert(IsNewStr(PopToken(&tokens)));
						break;
					case POP:
						object = PopToken(&tokens);
						assert(object->Type == REGISTER);
						assert(!IsEmptyStack(sp));

						switch(object->Value)
							{
								case RAX:
									rax = PopStack(&sp);
									printf("top of stack (%d) to rax\n", rax);
									break;
								case RBX:
									rbx = PopStack(&sp);
									printf("top of stack (%d) to rbx\n", rbx);
									break;
								case RCX:
									rcx = PopStack(&sp);
									printf("top of stack (%d) to rcx\n", rcx);
									break;
								case RDX:
									rdx = PopStack(&sp);
									printf("top of stack (%d) to rdx\n", rdx);
									break;
								default:
									assert(!"error");
							}

						assert(IsNewStr(PopToken(&tokens)));
						break;
					case ADD:
						sp = AddStack(sp);

						break;
					case SUB:
						sp = SubStack(sp);

						break;
					case MUL:
						sp = MulStack(sp);

						break;
					case DIV:
						sp = DivStack(sp);

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
	int ret = GetE(tokens);
	assert(IsNewStr(tokens));
	return ret;
}

int GetT(Token * tokens)
{
	
}
