#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//DEFINES

#define OPERATOR 0

#define PLUS 1
#define MINUS 2
#define DIVIDE 3
#define MULTIPLY 4

#define _PLUS(u, v) CreateNode(OPERATOR, PLUS, u, v)
#define _MINUS(u, v) CreateNode(OPERATOR, MINUS, u, v)
#define _MUL(u, v) CreateNode(OPERATOR, MULTIPLY, u, v)			//u * v
#define _DIVIDE(u, v) CreateNode(OPERATOR, DIVIDE, u, v)		//u / v



#define FUNCTION 7

#define SIN 8
#define COS 9
#define TG 10
#define CTG 11
#define ARCSIN 12
#define ARCCOS 13
#define ARCTG 14
#define ARCCTG 15
#define SH 16
#define CH 17
#define TH 18
#define CTH 19
#define EXP 20		// e^x
#define LOG 21		// log a (x)
#define LN 22		// ln(x)
#define INDICATIVE 23	// a^x
#define DEGREE 24	//x^a
#define ROOT 25

#define _DEGREE(a, x) CreateNode(FUNCTION, DEGREE, _NUM(a), x)		//x^a
#define _ROOT(a, x) CreateNode(FUNCTION, ROOT, _NUM(a), x)		//root of degree a of x
#define _SIN(x) CreateNode(FUNCTION, SIN, NULL, x)
#define _COS(x) CreateNode(FUNCTION, COS, NULL, x)
#define _TG(x) CreateNode(FUNCTION, TG, NULL, x)
#define _CTG(x) CreateNode(FUNCTION, CTG, NULL, x)
#define _LN(x) CreateNode(FUNCTION, LN, NULL, x)			//ln (x)
#define _LOG(a, x) CreateNode(FUNCTIONS, LOG, _NUM(a), x)		//log a (x)
#define _INDICATIVE(x, a) CreateNode(FUNCTION, INDICATIVE, _NUM(a), x)	//a^x
#define _EXP(x) CreateNode(FUNCTION, EXP, NULL, x)
#define _SH(x) CreateNode(FUNCTION, SH, NULL, x)
#define _CH(x) CreateNode(FUNCTION, CH, NULL, x)
#define _TH(x) CreateNode(FUNCTION, TH, NULL, x)
#define _CTH(x) CreateNode(FUNCTION, CTH, NULL, x)
#define _ARCSIN(x) CreateNode(FUNCTION, ARCSIN, NULL, x)
#define _ARCCOS(x) CreateNode(FUNCTION, ARCCOS, NULL, x)
#define _ARCTG(x) CreateNode(FUNCTION, ARCTG, NULL, x)
#define _ARCCTG(x) CreateNode(FUNCTION, ARCCTG, NULL, x)



#define NUMBER 24

#define ZERO CreateNode(NUMBER, 0, NULL, NULL)
#define ONE CreateNode(NUMBER, 1, NULL, NULL)
#define _NUM(x) CreateNode(NUMBER, x, NULL, NULL)



#define VARIABLE 25

#define _VAR CreateNode(VARIABLE, 0, NULL, NULL)



//STRUCT
typedef struct Node {
	double Value;
	int Type; 
	struct Node * Left;
	struct Node * Right;
} Node;


//GLOBAL
int global_change = 1;
FILE *file;


//DECLARATION OF FUNCTIONS

//functions for differentiator
Node * Diff(const Node * root);
Node * Copy(const Node * root);
Node * CreateNode(int type, double value, Node * left, Node * right); 

//functions for print to file
void NodeToFile(const Node * root);
void DeleteTree(Node * root);

//functions for optimization
Node * Compute(Node * root);
void Optimization(Node * root);



int main()
{
	file = fopen("res.tex", "w");
	assert(file);

	fprintf(file, "\\documentclass[a4paper,12pt]{article}\n");
	fprintf(file, "\\usepackage[T2A]{fontenc}\n");
	fprintf(file, "\\usepackage[utf8]{inputenc}\n");
	fprintf(file, "\\usepackage[english,russian]{babel}\n"); 
	fprintf(file, "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools, }\n");
	fprintf(file, "\\usepackage[normalem]{ulem}\n");


	fprintf(file, "\\begin{document}\n");
	fprintf(file, "\\section*{\\sout{Капитан очевидность}}"); 
	fprintf(file, "\\section*{Производные}");

	Node * a = _PLUS(_NUM(1), _VAR);
	Node * b = _MUL(_VAR, a);
	Node * c = _PLUS(_VAR, _NUM(2));
	Node * d = _MUL(_NUM(3), c);
	Node * e = _MINUS(_NUM(1), _VAR);
	Node * f = _DIVIDE(d, e);

	Node * root = CreateNode(OPERATOR, MINUS, b, f);

	Node * sqrt = _ROOT(2, _VAR);
	Node * sq = _DEGREE(3, _VAR);

	Node * g = _PLUS(sq, sqrt);

	root = _PLUS(g, root);

	Node * res = Diff(root);

#if defined(optimization)
	Optimization(res);
	NodeToFile(res);
#endif

#if defined(delete)	
	DeleteTree(res);

	DeleteTree(root);
#endif

	fprintf(file, "\\end{document}");

	fclose(file);

	system("pdflatex res.tex");
	system("rm res.log res.aux");

#if defined(realise)
	system("rm res.tex");
#endif
	return 0;
}



//IMPLEMENTATION OF FUNCTIONS

void Optimization(Node * root)
{
	while (global_change)
	{
		global_change = 0;
	
		root = Compute(root);

		assert(root);	
	}
}

int IsNumbers(Node * left, Node * right)
{
	assert(left);
	assert(right);

	int l_type = (int)(left->Type);
	int r_type = (int)(right->Type);

	return ((l_type == NUMBER) && (r_type == NUMBER));
}

Node * Compute(Node * root)
{
	printf("lol");

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

	Node * ret;

	if (IsNumbers(root->Left, root->Right))
	{
		global_change ++; 	


		fprintf(file, "$");

		NodeToFile(root);

		fprintf(file, " = ");	

		int value = (int)root->Value;

		switch (value)
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
	}

	NodeToFile(ret);

	fprintf(file, "$\n\\\\[0.5cm]\n");

	assert(ret);

	return ret;
	}
	case FUNCTION:
	{
	
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
			int value = (int)root->Value;

			switch (value)
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
			int value = (int)root->Value;

			switch (value)
			{
				case DEGREE:	// (x ^ a)' = a * x ^ (a - 1)
				{
					int a = root->Left->Value;
					Node * x = root->Right;

					Node * base = _DEGREE(_NUM(a - 1), Copy(x));	// base = x ^ (a - 1)

					Node * f1 = _MUL(_NUM(a), base);	// f' = a * base

					ret = _MUL(f1, Diff(x));	// (f(x))' = f' * x'
					break;
				}
				case ROOT:	// (root of degree a of x)' = (root of degree a of x) ^ (1 - a) \ a
				{
					int a = root->Left->Value;
					Node * x = root->Right;

					Node * base = _DEGREE(_NUM(1 - a), Copy(x));	// base = x ^ (1 - a)

					base = _ROOT(_NUM(a), base);	// base = (root of degree a of x) ^ (1 - a) 

					Node * f1 = _DIVIDE(base, _NUM(a));	// f' = base \ a

					ret = _MUL(f1, Diff(x));	// (f(x))' = f' * x'
					break;
				}
				case SIN:
				case COS:
				case TG:
				case CTG:
				case LN:
				case LOG:
				case ARCSIN:
				case ARCCOS:
				case ARCTG:
				case ARCCTG:
				case SH:
				case CH:
				case TH:
				case CTH:
				case EXP:
				case INDICATIVE:
			}
		}
	}

	assert(ret);

	fprintf(file, "$\\left(");
	NodeToFile(root);
	fprintf(file, "\\right)^\\prime = ");	
	NodeToFile(ret);
	fprintf(file, "$\n\\\\[0.5cm]\n");

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

void NodeToFile(const Node * root)
{
	assert(root);	

	switch (root->Type)
	{
		case NUMBER:
		{
			fprintf(file, "%.0f", root->Value);
			break;
		}
		case VARIABLE:
		{
			fprintf(file, "x");
			break;
		}	
		case OPERATOR:
		{
			assert(root->Left);
			assert(root->Right);

			int value = (int)(root->Value);

			switch (value)
			{
				case PLUS:
				{
					NodeToFile(root->Left);

					fprintf(file, "+");

					NodeToFile(root->Right);	

					break;
				}
				case MINUS:
				{
					NodeToFile(root->Left);

					fprintf(file, "-");

					NodeToFile(root->Right);	

					break;
				}	
				case MULTIPLY:
				{
	
					fprintf(file, "(");

					NodeToFile(root->Left);

					fprintf(file, ")\\cdot(");

					NodeToFile(root->Right);	
	
					fprintf(file, ")");

					break;		
				}	
				case DIVIDE:	
				{
					fprintf(file, "\\frac{");
			
					NodeToFile(root->Left);

					fprintf(file, "}{");

					NodeToFile(root->Right);	
	
					fprintf(file, "}");	

					break;		
				}	
			}
		}
		case FUNCTION:
		{
			int value = (int)root->Value;

			switch (value)
			{
		
			}
		}
	}
}

void DeleteTree(Node * root)
{
	if (root->Left)
		DeleteTree(root->Right);
	if (root->Left)
		DeleteTree(root->Right);
	free(root);
}
