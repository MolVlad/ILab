#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

enum {OPERATOR, FUNCTION, NUMBER, VARIABLE, CONST};
enum {PLUS, MINUS, DIVIDE, MULTIPLY, SIN, COS, TG, ARCSIN, ARCTG, EXP, LOG, LN, DEGREE};

#define _PLUS(u, v) CreateNode(OPERATOR, PLUS, u, v)
#define _MINUS(u, v) CreateNode(OPERATOR, MINUS, u, v)
#define _MUL(u, v) CreateNode(OPERATOR, MULTIPLY, u, v)		// u * v
#define _DIVIDE(u, v) CreateNode(OPERATOR, DIVIDE, u, v)	// u / v

#define _DEGREE(x, y) CreateNode(FUNCTION, DEGREE, x, y)	// x^y
#define _SIN(x) CreateNode(FUNCTION, SIN, NULL, x)
#define _COS(x) CreateNode(FUNCTION, COS, NULL, x)
#define _TG(x) CreateNode(FUNCTION, TG, NULL, x)
#define _LN(x) CreateNode(FUNCTION, LN, NULL, x)		// ln (x)
#define _LOG(a, x) CreateNode(FUNCTION, LOG, a, x)	// log a (x)
#define _EXP(x) CreateNode(FUNCTION, EXP, NULL, x)
#define _ARCSIN(x) CreateNode(FUNCTION, ARCSIN, NULL, x)
#define _ARCTG(x) CreateNode(FUNCTION, ARCTG, NULL, x)

#define ZERO CreateNode(NUMBER, 0, NULL, NULL)
#define ONE CreateNode(NUMBER, 1, NULL, NULL)
#define _NUM(x) CreateNode(NUMBER, x, NULL, NULL)

#define _VAR CreateNode(VARIABLE, 0, NULL, NULL)

#define _CONST(a) CreateNode(CONST, a, NULL, NULL)

typedef struct Node
{
	int Value;
	int Type;
	struct Node * Left;
	struct Node * Right;
} Node;

int global_change;
FILE * file_dot, * file_latex;
int dot_counter;
char * str;
int position;

char ** CapCreate();
Node * Diff(const Node * root);
Node * Copy(const Node * root);
Node * CreateNode(int type, double value, Node * left, Node * right);

void BeginForLatex(Node * root);
void EndForLatex(Node * root, Node * res);
void NodeToLatex(const Node * root);

Node * CreateTree(int n);
void DeleteTree(Node * root);
Node * ScanTree();
char * FileToStr();
int FileIsOk(FILE * file);
int SizeOfFile(FILE * file);

Node * General(const char * formula);
Node * Get_E();
Node * Get_T();
Node * Get_P();
Node * Get_N();

Node * EasyMultiply(Node *root);
Node * EasyAddition(Node * root);
Node * EasyDivide(Node * root);
Node * Compute(Node * root);
void Optimization(Node * root);
int IsZero(Node * root);
int IsNumbers(Node * left, Node * right);
int IsOne(Node * root);

void PrintToDot(Node * root);
void NodeToDot(Node * root);

int main()
{
	srand(time(NULL));

	Node * root = ScanTree();

	BeginForLatex(root);
	Node * res = Diff(root);
	global_change = 1;
	Optimization(res);
	EndForLatex(root, res);
	PrintToDot(root);

	DeleteTree(res);
	DeleteTree(root);
	free(str);

	return 0;
}

char ** CapCreate()
{
	char ** a = calloc(20, sizeof(char *));
	int i;

	a[0] = "Очевидно, что\n";
	a[1] = "Заметим, что\n";
	a[2] = "Легко видеть, что\n";
	a[3] = "Нельзя не упомянуть тот факт, что\n";
	a[4] = "Далее следует\n";
	a[5] = "Как можно видеть\n";
	a[6] = "С помощью нехитрых преобразований получаем, что\n";
	a[7] = "Нетрудно догадаться, что\n";
	a[8] = "С помощью несложных логических размышлений получаем, что\n";
	a[9] = "При аналогичных размышлениях, получаем, что\n";
	a[10] = "По всем известной формуле получаем, что\n";
	a[11] = "Учитывая, что\n";
	a[12] = "Отсюда несложно получить, что\n";
	a[13] = "В частности:\n";
	a[14] = "Обнаружив следующую закономерность:\n";
	a[15] = "Следовательно,\n";
	a[16] = "Откуда получаем, что\n";
	a[17] = "И поэтому\n";
	a[18] = "Таким образом,\n";
	a[19] = "Не нужно быть гением, чтобы понять, что\n";

	return a;
}

Node * ScanTree()
{
	char * str = FileToStr();
	Node * root = General(str);
	free(str);

	assert(root);

	return root;
}

char * FileToStr()
{
	FILE * file;
	file = fopen("base.txt", "r");
	assert(file);
	assert(FileIsOk(file));

	int n = SizeOfFile(file);
	char * str = (char *) calloc(n+1, sizeof(char));
	fscanf(file, "%[^~]", str);
	fclose(file);

	return str;
}

int FileIsOk(FILE * file)
{
	fseek(file, -2, SEEK_END);
	char symbol;
	fscanf(file, "%c", &symbol);
	fseek(file, 0, SEEK_SET);

	return symbol == '~';
}

int SizeOfFile(FILE * file)
{
	fseek(file, -2, SEEK_END);
	int n = ftell(file);
	fseek(file, 0, SEEK_SET);

	return n;
}

Node * General(const char * formula)
{
	int n = strlen(formula);
	str = calloc(n+1, sizeof(char));
	strcpy(str, formula);
	position = 0;

	Node * ret = Get_E();

	assert(str[position] == '\0');

	return ret;
}

Node * Get_E()
{
	printf("beginE\n");
	Node * ret = Get_T();

	while (str[position] == '+' || str[position] == '-')
	{
		int operator = str[position];
		position++;

		Node * next_ret = Get_T();
		printf("нашел %c\n", operator);
		if (operator == '+')
			ret = _PLUS(ret, next_ret);
		else
			ret = _MINUS(ret, next_ret);
	}

	printf("endE\n");
	return ret;
}

Node * Get_T()
{
	printf("beginT\n");
	Node * ret = Get_P();

	while (str[position] == '*' || str[position] == '/' || str[position] == '^')
	{
		int operator = str[position];
		position++;

		Node * next_ret = Get_P();

		printf("нашел %c\n", operator);
		if (operator == '*')
			ret = _MUL(ret, next_ret);
		else if (operator == '/')
			ret = _DIVIDE(ret, next_ret);
		else if (operator == '^')
			ret = _DEGREE(ret, next_ret);
	}

	printf("endT\n");
	return ret;
}

Node * Get_P()
{
	Node * ret;
	printf("beginP\n");
	if (str[position] == '(')
	{
		printf("нашёл начало скобки\n");
		position++;
		ret = Get_E();
		assert(str[position] == ')');
		printf("нашёл конец скобки\n");
		position++;
	}
	else if (str[position] == 'x')
	{
		printf("нашёл х\n");
		position++;
		ret = _VAR;
	}
	else if (str[position] == 't' && str[position + 1] == 'g')
	{
		printf("нашел tg\n");
		assert(str[position + 2] == '(');
		position += 3;
		ret = _TG(Get_E());
		assert(str[position] == ')');
		position++;
	}
	else if (str[position] == 'l' && str[position + 1] == 'n')
	{
		printf("нашел ln\n");
		assert(str[position + 2] == '(');
		position += 3;
		ret = _LN(Get_E());
		assert(str[position] == ')');
		position++;
	}
	else if (str[position] == 'l' && str[position + 1] == 'o' && str[position + 2] == 'g')
	{
		printf("нашел log\n");
		assert(str[position + 3] == '(');
		position += 4;
		Node * base = Get_E();
		assert(str[position] == ')');
		position++;

		assert(str[position] == '(');
		position++;
		Node * arg = Get_E();
		assert(str[position] == ')');
		position++;

		ret = _LOG(base, arg);
	}
	else if (str[position] == '-')
	{
		position++;

		ret = _MUL(_NUM(-1), Get_E());
	}
	else if (str[position] == 'a' && str[position + 1] == 'r' && str[position + 2] == 'c')
	{
		position += 3;

		if (str[position] == 's' && str[position + 1] == 'i' && str[position + 2] == 'n')
		{
			printf("нашел arcsin\n");
			assert(str[position + 3] == '(');
			position += 4;
			ret = _ARCSIN(Get_E());
			assert(str[position] == ')');
			position++;
		}
		else if (str[position] == 't' && str[position + 1] == 'g')
		{
		printf("нашел arctg\n");
		assert(str[position + 2] == '(');
		position += 3;
		ret = _ARCTG(Get_E());
		assert(str[position] == ')');
		position++;
		}
		else
			assert(!"arcsin/arctg");
	}
	else if (str[position] == 'e' && str[position + 1] == 'x' && str[position + 2] == 'p')
	{
		printf("нашел exp\n");
		assert(str[position + 3] == '(');
		position += 4;
		ret = _EXP(Get_E());
		assert(str[position] == ')');
		position++;
	}
	else if (str[position] == 's' && str[position + 1] == 'i' && str[position + 2] == 'n')
	{
		printf("нашел син\n");
		assert(str[position + 3] == '(');
		position += 4;
		ret = _SIN(Get_E());
		assert(str[position] == ')');
		position++;
	}
	else if (str[position] == 'c' && str[position + 1] == 'o' && str[position + 2] == 's')
	{
		printf("нашел cos\n");
		assert(str[position + 3] == '(');
		position += 4;
		ret = _COS(Get_E());
		assert(str[position] == ')');
		position++;
	}
	else if ('a' <= str[position] && str[position] <= 'd')
	{
		printf("нашёл const\n");
		ret = _CONST(str[position]);
		position++;
	}
	else
		ret = Get_N();

	printf("endP\n");
	assert(ret);
	return ret;
}

Node * Get_N()
{
	int number = 0;
	int count = 0;

	printf("beginN\n");
	while ('0' <= str[position] && str[position] <= '9')
	{
		number = number * 10 + str[position] - '0';
		position++;
		count++;
	}

	printf("нашел %d\n", number);
	assert(count);

	printf("endN\n");
	return _NUM(number);
}

void BeginForLatex(Node * root)
{
	file_latex = fopen("res.tex", "w");
	assert(file_latex);

	fprintf(file_latex, "\\documentclass[a4paper,12pt]{article}\n");
	fprintf(file_latex, "\\usepackage[T2A]{fontenc}\n");
	fprintf(file_latex, "\\usepackage[utf8]{inputenc}\n");
	fprintf(file_latex, "\\usepackage[english,russian]{babel}\n");
	fprintf(file_latex, "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools, }\n");
	fprintf(file_latex, "\\usepackage[normalem]{ulem}\n");

	fprintf(file_latex, "\\begin{document}\n");
	fprintf(file_latex, "\\section*{\\sout{Капитан очевидность}}");
	fprintf(file_latex, "\\section*{Производные}\n");
	fprintf(file_latex, "Возьмем производную вот такого зверя:\n\\\\[0.5cm]\n$");
	NodeToLatex(root);
	fprintf(file_latex, "$\n\\\\[0.5cm]\n");
}

void EndForLatex(Node * root, Node * res)
{
	fprintf(file_latex, "\\text{Окончательно получаем:}");
	fprintf(file_latex, "\n\\\\[0.5cm]\n");

	fprintf(file_latex, "$\\left(");
	NodeToLatex(root);
	fprintf(file_latex, "\\right)^\\prime = \n\\\\[0.5cm]\n");
	NodeToLatex(res);
	fprintf(file_latex, "$\n");
	fprintf(file_latex, "\n\\end{document}");

	fclose(file_latex);

	system("pdflatex res.tex");
	system("rm res.log res.aux");
	system("rm res.tex");
}

void PrintToDot(Node * root)
{
	file_dot = fopen("tree.dot", "w");
	assert(file_dot);
	fprintf(file_dot, "digraph G {\n");

	dot_counter = 0;
	NodeToDot(root);
	fprintf(file_dot, "}");
	fclose(file_dot);

	system("dot -v -Tpng -o tree tree.dot");
	system("rm tree.dot");
}

void NodeToDot(Node * root)
{
	assert(root);

	int cur = dot_counter;
	fprintf(file_dot, "\tN%d [label=\"", cur);

	switch (root->Type)
	{
		case NUMBER:
			{
				fprintf(file_dot, "%d", root->Value);
				break;
			}
		case CONST:
			{
				fprintf(file_dot, "%c", root->Value);
				break;
			}
		case OPERATOR:
			{
				switch (root->Value)
				{
					case PLUS:
						{
							fprintf(file_dot, "+");
							break;
						}
					case MINUS:
						{
							fprintf(file_dot, "-");
							break;
						}
					case DIVIDE:
						{
							fprintf(file_dot, "/");
							break;
						}
					case MULTIPLY:
						{
							fprintf(file_dot, "*");
							break;
						}
				}
				break;
			}
		case VARIABLE:
			{
				fprintf(file_dot, "x");
				break;
			}
		case FUNCTION:
			{
				switch (root->Value)
				{
					case SIN:
						{
							fprintf(file_dot, "sin");
							break;
						}
					case COS:
						{
							fprintf(file_dot, "cos");
							break;
						}
					case TG:
						{
							fprintf(file_dot, "tg");
							break;
						}
					case ARCSIN:
						{
							fprintf(file_dot, "arcsin");
							break;
						}
					case ARCTG:
						{
							fprintf(file_dot, "arctg");
							break;
						}
					case EXP:
						{
							fprintf(file_dot, "exp");
							break;
						}
					case LOG:
						{
							fprintf(file_dot, "log");
							break;
						}
					case LN:
						{
							fprintf(file_dot, "ln");
							break;
						}
					case DEGREE:
						{
							fprintf(file_dot, "^");
							break;
						}
				}
				break;
			}
	}

	fprintf(file_dot, "\"]\n");

	if (root->Left)
	{
		fprintf(file_dot, "\tN%d->N%d\n", cur, ++dot_counter);
		NodeToDot(root->Left);
	}

	if (root->Right)
	{
		fprintf(file_dot, "\tN%d->N%d\n", cur, ++dot_counter);
		NodeToDot(root->Right);
	}
}

void Optimization(Node * root)
{
	while (global_change)
	{
		global_change = 0;

		root = Compute(root);
		root = EasyMultiply(root);
		root = EasyAddition(root);
		root = EasyDivide(root);

		assert(root);
	}
}

int IsNumbers(Node * left, Node * right)
{
	assert(left);
	assert(right);

	return (((left->Type) == NUMBER) && (right->Type) == NUMBER);
}

Node * EasyMultiply(Node *root)
{
	assert(root);

	Node * ret;
	int change = 0;

	if (!(root->Left && root->Right))
		return root;

	if ((root->Type == OPERATOR) && (root->Value == MULTIPLY))
	{
		if (IsZero(root->Left) || IsZero(root->Right))
		{
			ret = ZERO;
			change ++;
		}
		else if (IsOne(root->Left))
		{
			ret = Copy(root->Right);
			change ++;
		}
		else if (IsOne(root->Right))
		{
			ret = Copy(root->Left);
			change ++;
		}

		if (change)
		{
				global_change ++;

				int i = rand() % 20;
				char ** phrases = CapCreate();
				fprintf(file_latex, "%s\\\\[0.5cm]\n", phrases[i]);
				free(phrases);

				fprintf(file_latex, "$");
				NodeToLatex(root);
				fprintf(file_latex, " = ");
				NodeToLatex(ret);
				fprintf(file_latex, "$\n\\\\[0.5cm]\n");
				DeleteTree(root);

				return ret;
		}
	}

	if (root->Left)
		root->Left = EasyMultiply(root->Left);
	if (root->Right)
		root->Right = EasyMultiply(root->Right);

	return root;
}

int IsZero(Node * root)
{
	return ((root->Type == NUMBER) && (root->Value == 0));
}

int IsOne(Node * root)
{
	return ((root->Type == NUMBER) && (root->Value == 1));
}

Node * EasyAddition(Node * root)
{
	assert(root);

	Node * ret;
	int change = 0;

	if (!(root->Left && root->Right))
		return root;

	if (root->Type == OPERATOR)
	{
		switch (root->Value)
		{
			case PLUS:
				{
					if (IsZero(root->Left))
					{
						ret = Copy(root->Right);
						change ++;

						break;
					}
					else if (IsZero(root->Right))
					{
						ret = Copy(root->Left);
						change ++;

						break;
					}

					break;
				}
			case MINUS:
				{
					if (IsZero(root->Left))
					{
						ret = _MUL(_NUM(-1), Copy(root->Right));
						change ++;

						break;
					}
					else if (IsZero(root->Right))
					{
						ret = Copy(root->Left);
						change ++;

						break;
					}

					break;
				}
		}

		if (change)
		{
				global_change ++;

				int i = rand() % 20;
				char ** phrases = CapCreate();
				fprintf(file_latex, "%s\\\\[0.5cm]\n", phrases[i]);
				free(phrases);

				fprintf(file_latex, "$");
				NodeToLatex(root);
				fprintf(file_latex, " = ");
				NodeToLatex(ret);
				fprintf(file_latex, "$\n\\\\[0.5cm]\n");
				DeleteTree(root);

				return ret;
		}
	}

	if (root->Left)
		root->Left = EasyAddition(root->Left);
	if (root->Right)
		root->Right = EasyAddition(root->Right);

	return root;
}

Node * EasyDivide(Node * root)
{
	assert(root);

	Node * ret;
	int change = 0;

	if (!(root->Left && root->Right))
		return root;

	if ((root->Type == OPERATOR) && (root->Value == DIVIDE))
	{
		if (IsZero(root->Left))
		{
			ret = ZERO;
			change ++;
		}

		if (change)
		{
				global_change ++;

				int i = rand() % 20;
				char ** phrases = CapCreate();
				fprintf(file_latex, "%s\\\\[0.5cm]\n", phrases[i]);
				free(phrases);

				fprintf(file_latex, "$");
				NodeToLatex(root);
				fprintf(file_latex, " = ");
				NodeToLatex(ret);
				fprintf(file_latex, "$\n\\\\[0.5cm]\n");
				DeleteTree(root);

				return ret;
		}
	}

	if (root->Left)
		root->Left = EasyDivide(root->Left);
	if (root->Right)
		root->Right = EasyDivide(root->Right);

	return root;

}

Node * Compute(Node * root)
{
	assert(root);

	switch (root->Type)
	{
		case CONST:
			{
				return root;
			}
		case NUMBER:
			{
				return root;
			}
		case OPERATOR:
			{
				assert(root->Left);
				assert(root->Right);

				root->Left = Compute(root->Left);
				root->Right = Compute(root->Right);

				assert(root->Left);
				assert(root->Right);

				if ((IsNumbers(root->Left, root->Right)) && (root->Value != DIVIDE))
				{
					Node * ret;

					global_change ++;

					int i = rand() % 20;
					char ** phrases = CapCreate();
					fprintf(file_latex, "%s\\\\[0.5cm]\n", phrases[i]);
					free(phrases);

					fprintf(file_latex, "$");
					NodeToLatex(root);
					fprintf(file_latex, " = ");


					switch (root->Value)
					{
						case PLUS:
							{
								ret = _NUM(root->Left->Value + root->Right->Value);

								break;
							}
						case MINUS:
							{
								ret = _NUM(root->Left->Value - root->Right->Value);

								break;
							}
						case MULTIPLY:
							{
								ret = _NUM(root->Left->Value * root->Right->Value);

								break;
							}
					}

					DeleteTree(root);
					NodeToLatex(ret);
					fprintf(file_latex, "$\n\\\\[0.5cm]\n");


					assert(ret);

					return ret;
				}
				else
				{
					return root;
				}

				break;
			}
		case FUNCTION:
			{
				return root;
				break;
			}
		case VARIABLE:
			{
				return root;
				break;
			}
	}
}

Node * Diff(const Node * root)
{
	assert(root);

	Node * ret;

	switch (root->Type)
	{
		case NUMBER:
			{
				ret = ZERO;
				break;
			}
		case VARIABLE:
			{
				ret = ONE;
				break;
			}
		case CONST:
			{
				ret = ZERO;
				break;
			}
		case OPERATOR:
			{
				switch (root->Value)
				{
					case PLUS:	// (u+v)' = u' + v'
						{
							ret = _PLUS(Diff(root->Left), Diff(root->Right));
							break;
						}
					case MINUS:	// (u-v)' = u' - v'
						{
							ret = _MINUS(Diff(root->Left), Diff(root->Right));
							break;
						}
					case MULTIPLY:	// (uv)' = u'v + uv'
						{
							Node * u1 = Diff(root->Left);	// u'
							Node * v = Copy(root->Right);	// v
							Node * u = Copy(root->Left);	// u
							Node * v1 = Diff(root->Right);	// v'

							ret = _PLUS(_MUL(u1, v), _MUL(u, v1));
							break;
						}
					case DIVIDE:	// (u/v)' = (u'v - uv') / v^2
						{
							Node * u1 = Diff(root->Left);	// u'
							Node * v = Copy(root->Right);	// v
							Node * u = Copy(root->Left);	// u
							Node * v1 = Diff(root->Right);	// v'

							Node * numerator = _MINUS(_MUL(u1, v), _MUL(u, v1));	// числитель
							Node * denominator = _DEGREE(Copy(v), _NUM(2));	// знаменатель

							ret = _DIVIDE(numerator, denominator);
							break;
						}
				}

				break;
			}
		case FUNCTION:	// (f(x))' = f' * x'
			{
				switch (root->Value)
				{
					case DEGREE:	// y = f(x) ^ g(x); ln(y) = g(x) * ln(f(x)); y' = y * (g * ln(f))'
						{
							Node * y = Copy(root);

							Node * g = root->Right;
							Node * f = root->Left;

							Node * first_term = _MUL(Diff(g), _LN(Copy(f)));		// g' * ln(f)
							Node * second_term = _MUL(Copy(g), _DIVIDE(Diff(f), Copy(f)));	// g * f' / f

							Node * sum = _PLUS(first_term, second_term);

							ret = _MUL(y, sum);

							break;
						}
					case SIN:
						{
							Node * x = root->Right;

							ret = _MUL(Diff(x), _COS(Copy(x)));

							break;
						}
					case COS:
						{
							Node * x = root->Right;
							Node * base = _MUL(_NUM(-1), _SIN(Copy(x)));

							ret = _MUL(Diff(x), base);

							break;

						}
					case TG:
						{
							Node * x = root->Right;
							Node * base = _DEGREE(_COS(Copy(x)), _NUM(2));

							ret = _DIVIDE(Diff(x), base);

							break;
						}
					case LN:
						{
							Node * x = root->Right;

							ret = _DIVIDE(Diff(x), Copy(x));

							break;
						}
					case LOG:
						{
							Node * x = root->Right;
							Node * a = root->Left;

							assert ((a->Type == NUMBER) || (a->Type == CONST));

							Node * base = _MUL(Copy(x), _LN(Copy(a)));	// x * ln(a)

							ret = _DIVIDE(Diff(x), base);	// x' / (x * ln(a))

							break;

						}
					case ARCSIN:
						{
							Node * x = root->Right;
							Node * base = _MINUS(ONE, _DEGREE(Copy(x), _NUM(2)));	// 1 - x ^ 2
							Node * sqrt = _DEGREE(base, _DIVIDE(_NUM(1), _NUM(2)));		// sqrt(base)

							ret = _DIVIDE(Diff(x), base);	// x' / sqrt(1 - x ^ 2)

							break;

						}
					case ARCTG:
						{
							Node * x = root->Right;
							Node * base = _PLUS(_NUM(1), _DEGREE(Copy(x), _NUM(2)));	// 1 +x ^ 2

							ret = _DIVIDE(Diff(x), base);	// 1 / (1 + x ^ 2)

							break;

						}
					case EXP:	// (exp(x))' = exp(x) * x'
						{
							Node * x = root->Right;
							Node * exp = Copy(root);	// exp(x)

							ret = _MUL(Diff(x), exp);

							break;

						}
				}

				break;
			}
	}

	assert(ret);

	int i = rand() % 20;
	char ** phrases = CapCreate();
	fprintf(file_latex, "%s\\\\[0.5cm]\n", phrases[i]);
	free(phrases);

	fprintf(file_latex, "$\\left(");
	NodeToLatex(root);
	fprintf(file_latex, "\\right)^\\prime = ");
	NodeToLatex(ret);
	fprintf(file_latex, "$\n\\\\[0.5cm]\n");

	return ret;
}

Node * Copy(const Node * root)
{
	if (!root)
		return NULL;

	Node * ret = (Node *) calloc(1, sizeof(Node));

	ret->Left = Copy(root->Left);
	ret->Right = Copy(root->Right);
	ret->Value = root->Value;
	ret->Type = root->Type;

	assert(ret);

	return ret;
}

Node * CreateNode(int type, double value, Node * left, Node * right)
{
	Node * ret = (Node *) calloc(1, sizeof(Node));

	ret->Type = type;
	ret->Value = value;
	ret->Left = left;
	ret->Right = right;

	assert(ret);

	return ret;
}

int BracketsNeed(Node * root)
{
	if (root->Type == OPERATOR)
		if ((root->Value == PLUS) || (root->Value == MINUS))
			return 1;

	if ((root->Type == NUMBER) && (root->Value < 0))
		return 1;

	return 0;
}

void NodeToLatex(const Node * root)
{
	assert(root);

	switch (root->Type)
	{
		case NUMBER:
			{
				fprintf(file_latex, "%d", root->Value);
				break;
			}
		case VARIABLE:
			{
				fprintf(file_latex, "x");
				break;
			}
		case CONST:
			{
				fprintf(file_latex, "%c", root->Value);
				break;
			}
		case OPERATOR:
			{
				assert(root->Left);
				assert(root->Right);

				switch (root->Value)
				{
					case PLUS:
						{
							NodeToLatex(root->Left);
							fprintf(file_latex, "+");
							NodeToLatex(root->Right);

							break;
						}
					case MINUS:
						{
							NodeToLatex(root->Left);
							fprintf(file_latex, "-");
							NodeToLatex(root->Right);

							break;
						}
					case MULTIPLY:
						{
							if (BracketsNeed(root->Left))
							{
								fprintf(file_latex, "\\left(");
								NodeToLatex(root->Left);
								fprintf(file_latex, "\\right)");
							}
							else
								NodeToLatex(root->Left);

							fprintf(file_latex, "\\cdot ");

							if (BracketsNeed(root->Right))
							{
								fprintf(file_latex, "\\left(");
								NodeToLatex(root->Right);
								fprintf(file_latex, "\\right)");
							}
							else
								NodeToLatex(root->Right);

							break;
						}
					case DIVIDE:
						{
							fprintf(file_latex, "\\frac{");
							NodeToLatex(root->Left);
							fprintf(file_latex, "}{");
							NodeToLatex(root->Right);
							fprintf(file_latex, "}");

							break;
						}
				}

				break;
			}
		case FUNCTION:
			{
				switch (root->Value)
				{
					case DEGREE:
						{
							if (root->Left->Type != VARIABLE)
							{
								fprintf(file_latex, "\\left(");
								NodeToLatex(root->Left);
								fprintf(file_latex, "\\right)");
							}
							else
								NodeToLatex(root->Left);

							fprintf(file_latex, "^{");
							NodeToLatex(root->Right);
							fprintf(file_latex, "}");

							break;
						}
					case SIN:
						{
							fprintf(file_latex, "\\sin\\left(");
							NodeToLatex(root->Right);
							fprintf(file_latex, "\\right)");

							break;
						}
					case COS:
						{
							fprintf(file_latex, "\\cos\\left(");
							NodeToLatex(root->Right);
							fprintf(file_latex, "\\right)");

							break;
						}
					case TG:
						{
							fprintf(file_latex, "\\tan\\left(");
							NodeToLatex(root->Right);
							fprintf(file_latex, "\\right)");

							break;
						}
					case ARCSIN:
						{
							fprintf(file_latex, "\\arcsin\\left(");
							NodeToLatex(root->Right);
							fprintf(file_latex, "\\right)");

							break;
						}
					case ARCTG:
						{
							fprintf(file_latex, "\\arctan\\left(");
							NodeToLatex(root->Right);
							fprintf(file_latex, "\\right)");

							break;
						}
					case EXP:
						{
							fprintf(file_latex, "\\exp\\left(");
							NodeToLatex(root->Right);
							fprintf(file_latex, "\\right)");

							break;
						}
					case LOG:
						{
							fprintf(file_latex, "\\log_{");
							NodeToLatex(root->Left);
							fprintf(file_latex, "}\\left(");
							NodeToLatex(root->Right);
							fprintf(file_latex, "\\right)");

							break;
						}
					case LN:
						{
							fprintf(file_latex, "\\ln\\left(");
							NodeToLatex(root->Right);
							fprintf(file_latex, "\\right)");

							break;
						}
				}

				break;
			}
	}
}

void DeleteTree(Node * root)
{
	assert(root);

	if (root->Left)
		DeleteTree(root->Left);
	if (root->Right)
		DeleteTree(root->Right);
	free(root);
}
