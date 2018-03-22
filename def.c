#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define OPERATOR 0

#define PLUS 1
#define MINUS 2
#define DIVIDE 3
#define MULTIPLY 4

#define _PLUS(u, v) CreateNode(OPERATOR, PLUS, u, v)
#define _MINUS(u, v) CreateNode(OPERATOR, MINUS, u, v)
#define _MUL(u, v) CreateNode(OPERATOR, MULTIPLY, u, v)		//u * v
#define _DIVIDE(u, v) CreateNode(OPERATOR, DIVIDE, u, v)	//u / v

#define FUNCTION 5

#define SIN 6
#define COS 7
#define TG 8
#define ARCSIN 9
#define ARCTG 10
#define EXP 11		// e^x
#define LOG 12		// log a (x)
#define LN 13		// ln(x)
#define DEGREE 14	//x^a or a^x

#define _DEGREE(x, y) CreateNode(FUNCTION, DEGREE, x, y)	//x^y
#define _SIN(x) CreateNode(FUNCTION, SIN, NULL, x)
#define _COS(x) CreateNode(FUNCTION, COS, NULL, x)
#define _TG(x) CreateNode(FUNCTION, TG, NULL, x)
#define _LN(x) CreateNode(FUNCTION, LN, NULL, x)		//ln (x)
#define _LOG(a, x) CreateNode(FUNCTIONS, LOG, _NUM(a), x)	//log a (x)
#define _EXP(x) CreateNode(FUNCTION, EXP, NULL, x)
#define _ARCSIN(x) CreateNode(FUNCTION, ARCSIN, NULL, x)
#define _ARCTG(x) CreateNode(FUNCTION, ARCTG, NULL, x)

#define NUMBER 24

#define ZERO CreateNode(NUMBER, 0, NULL, NULL)
#define ONE CreateNode(NUMBER, 1, NULL, NULL)
#define _NUM(x) CreateNode(NUMBER, x, NULL, NULL)

#define VARIABLE 25

#define _VAR CreateNode(VARIABLE, 0, NULL, NULL)

typedef struct Node {
	int Value;
	int Type;
	struct Node * Left;
	struct Node * Right;
} Node;

int global_change = 1;
FILE *file_dot, *file_latex;
int dot_counter;

Node * Diff(const Node * root);
Node * Copy(const Node * root);
Node * CreateNode(int type, double value, Node * left, Node * right);

void BeginForLatex();
void EndForLatex(Node * root);
void NodeToLatex(const Node * root);

Node * CreateTree();
void DeleteTree(Node * root);

Node * EasyMultiply(Node *root);
Node * EasyAddition(Node * root);
Node *  EasyDivide(Node * root);
Node * Compute(Node * root);
void Optimization(Node * root);

void PrintToDot(Node * root);
void NodeToDot(Node * root);

int main()
{
	Node * root = CreateTree();

	BeginForLatex();

	Node * res = Diff(root);

	Optimization(res);

	EndForLatex(res);

	PrintToDot(root);

	DeleteTree(res);
	DeleteTree(root);

	return 0;
}

Node * CreateTree()
{
	Node * a = _PLUS(_NUM(1), _VAR);
	Node * b = _MUL(_VAR, a);
	Node * c = _PLUS(_VAR, _NUM(2));
	Node * d = _MUL(_NUM(3), c);
	Node * e = _MINUS(_NUM(1), _VAR);
	Node * f = _DIVIDE(d, e);
	Node * root = _MINUS(b, f);

	return root;
}

void BeginForLatex()
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
	fprintf(file_latex, "\\section*{Производные}");
}

void EndForLatex(Node * root)
{
	fprintf(file_latex, "\\text{Окончательно получаем:}");
	fprintf(file_latex, "\n\\\\[0.5cm]\n");
	fprintf(file_latex, "$");
	NodeToLatex(root);
	fprintf(file_latex, "$");
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
							fprintf(file_dot, "arccos");
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
	
}

Node * EasyAddition(Node * root)
{

}

Node *  EasyDivide(Node * root)
{
	
}

Node * Compute(Node * root)
{
	assert(root);

	switch (root->Type)
	{
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

				if (IsNumbers(root->Left, root->Right))
				{
					Node * ret;

					global_change ++;

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
						case DIVIDE:
							{
								assert(root->Right->Value);

								ret = _NUM(root->Left->Value / root->Right->Value);
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
			}
		case FUNCTION:
			{
				return root;
			}
		case VARIABLE:
			{
				return root;
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
					Node * u1 = Diff(root->Left);	//u'
					Node * v = Copy(root->Right);	//v
					Node * u = Copy(root->Left);	//u
					Node * v1 = Diff(root->Right);	//v'

					ret = _PLUS(_MUL(u1, v), _MUL(u, v1));
					break;
				}
				case DIVIDE:	// (u/v)' = (u'v - uv') / v^2
				{
					Node * u1 = Diff(root->Left);	//u'
					Node * v = Copy(root->Right);	//v
					Node * u = Copy(root->Left);	//u
					Node * v1 = Diff(root->Right);	//v'

					Node * numerator = _MINUS(_MUL(u1, v), _MUL(u, v1));	//числитель

					Node * denominator = _MUL(Copy(v), Copy(v));	//знаменатель

					ret = _DIVIDE(numerator, denominator);
					break;
				}
			}
		}
		case FUNCTION:	// (f(x))' =  f' * x'
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
				}
				case SIN:
					{

					}
				case COS:
					{

					}
				case TG:
					{

					}
				case LN:
					{

					}
				case LOG:
					{

					}
				case ARCSIN:
					{

					}
				case ARCTG:
					{

					}
				case EXP:
					{

					}
			}
		}
	}

	assert(ret);

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

					fprintf(file_latex, "(");

					NodeToLatex(root->Left);

					fprintf(file_latex, ")\\cdot(");

					NodeToLatex(root->Right);

					fprintf(file_latex, ")");

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
		}
		case FUNCTION:
		{
			switch (root->Value)
			{
				case DEGREE:
					{
						fprintf(file_latex, "(");

						NodeToLatex(root->Left);

						fprintf(file_latex, ")^{");

						NodeToLatex(root->Right);

						fprintf(file_latex, "}");

						break;
					}
				case SIN:
					{
						fprintf(file_latex, "\\sin(");

						NodeToLatex(root->Right);

						fprintf(file_latex, ")");

						break;
					}
				case COS:
					{
						fprintf(file_latex, "\\cos(");

						NodeToLatex(root->Right);

						fprintf(file_latex, ")");

						break;
					}
				case TG:
					{
						fprintf(file_latex, "\\sin(");

						NodeToLatex(root->Right);

						fprintf(file_latex, ")");

						break;
					}
				case ARCSIN:
					{
						fprintf(file_latex, "\\arcsin(");

						NodeToLatex(root->Right);

						fprintf(file_latex, ")");

						break;
					}
				case ARCTG:
					{
						fprintf(file_latex, "\\arctan(");

						NodeToLatex(root->Right);

						fprintf(file_latex, ")");

						break;
					}
				case EXP:
					{
						fprintf(file_latex, "\\exp(");

						NodeToLatex(root->Right);

						fprintf(file_latex, ")");

						break;
					}
				case LOG:
					{
						fprintf(file_latex, "\\log_{");

						NodeToLatex(root->Left);

						fprintf(file_latex, "\\}()");

						NodeToLatex(root->Right);

						fprintf(file_latex, ")");

						break;
					}
				case LN:
					{
						fprintf(file_latex, "\\ln(");

						NodeToLatex(root->Right);

						fprintf(file_latex, ")");

						break;
					}
			}
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
	root = NULL;
}
