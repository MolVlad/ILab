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

#define FUNCTION 5

#define SIN 6
#define COS 7
#define TG 8
#define CTG 9
#define ARCSIN 10
#define ARCCOS 11
#define ARCTG 12
#define ARCCTG 13
#define SH 14
#define CH 15
#define TH 16
#define CTH 17
#define EXP 18		// e^x
#define LOG 19		// log a (x)
#define LN 20		// ln(x)
#define DEGREE 21	// x^a
#define INDICATIVE 22	// a^x

#define NUMBER 23

#define VARIABLE 24

#define ZERO CreateNode(NUMBER, 0, NULL, NULL)
#define ONE CreateNode(NUMBER, 1, NULL, NULL)
#define _NUM(x) CreateNode(NUMBER, x, NULL, NULL)
#define _PLUS(u, v) CreateNode(OPERATOR, PLUS, u, v)
#define _MINUS(u, v) CreateNode(OPERATOR, MINUS, u, v)
#define _SQUARE(x) CreateNode(FUNCTION, DEGREE, Copy(x), _NUM(2))	// x^2
#define _MUL(u, v) CreateNode(OPERATOR, MULTIPLY, Copy(u), Copy(v))	// u * v
#define _DIVIDE(u, v) CreateNode(OPERATOR, DIVIDE, Copy(u), Copy(v))	// u / v
#define _VAR CreateNode(VARIABLE, 0, NULL, NULL)
#define _SIN(x) CreateNode(FUNCTION, SIN, Copy(x), NULL);


//STRUCT
typedef struct Node {
	double Value;
	int Type; 
	struct Node * Left;
	struct Node * Right;
} Node;


//GLOBAL
int global_change = 0;
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



	Node * a = CreateNode(OPERATOR, PLUS, _NUM(1), _VAR);
	Node * b = CreateNode(OPERATOR, MULTIPLY, _VAR, a);
	Node * c = CreateNode(OPERATOR, PLUS, _VAR, _NUM(2));
	Node * d = CreateNode(OPERATOR, MULTIPLY, _NUM(3), c);
	Node * e = CreateNode(OPERATOR, MINUS, _NUM(1), _VAR);
	Node * f = CreateNode(OPERATOR, DIVIDE, d, e);

	Node * root = CreateNode(OPERATOR, MINUS, b, f);

	Node * res = Diff(root);
	
	global_change = 1;
//	Optimization(res);
//	NodeToFile(res);
/*	
	DeleteTree(res);

	DeleteTree(root);
*/
	fprintf(file, "\\end{document}");

	fclose(file);

	return 0;
}



//IMPLEMENTATION OF FUNCTIONS

void Optimization(Node * root)
{
	while (global_change)
	{
		global_change = 0;
	
		//делаем дамп что запустилась оптимизация

		root = Compute(root);

		assert(root);	

		//делаем дамп что цикл оптимизации завершен
	}
}

int IsNumbers(Node * left, Node * right)
{
	assert(left);
	assert(right);

	int l_type = (int)left->Type;
	int r_type = (int)right->Type;

	return ((l_type == NUMBER) && (r_type == NUMBER));
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

	Node * ret;

	if (IsNumbers(root->Left, root->Right))
	{
		global_change ++; 	


		fprintf(file, "\\begin{math}");

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

	fprintf(file, "\\end{math} \n\\\\[0.5cm]\n");

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
			
			Node * denominator = _MUL(v, v);	//знаменатель

			ret = _DIVIDE(numerator, denominator);
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

	assert(ret);

	fprintf(file, "\\begin{math}");

	fprintf(file, "\\left(");

	NodeToFile(root);

	fprintf(file, "\\right)^\\prime = ");	

	NodeToFile(ret);

	fprintf(file, "\\end{math} \n\\\\[0.5cm]\n");

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

		int value = (int)root->Value;

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
