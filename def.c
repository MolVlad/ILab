#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
#define _PLUS(u, v) CreateNode(OPERATOR, PLUS, Diff(u), Diff(v))
#define _MINUS(u, v) CreateNode(OPERATOR, MINUS, Diff(u), Diff(v))
#define _SQUARE(x) CreateNode(FUNCTION, DEGREE, Copy(x), _NUM(2))	// x^2
#define _MUL(u, v) CreateNode(OPERATOR, MULTIPLY, Copy(u), Copy(v))	// u * v
#define _DIVIDE(u, v) CreateNode(OPERATOR, DIVIDE, Copy(u), Copy(v))	// u / v



#define _SIN(x) CreateNode(FUNCTION, SIN, Copy(x), NULL);



#define MaxSizeStr 1000


typedef char * Data;
typedef struct Node {
	int Value;
	int Type; 
	struct Node * Left;
	struct Node * Right;
} Node;

//DECLARATION OF FUNCTIONS
//functions for differentiator
Node * Diff(const Node * root);
Node * Copy(const Node * root);
Node * CreateNode(int type, int value, Node * left, Node * right); 

//functions for save to file
void StrToFile(Data str);
void SaveTree(Node * root);
Data PrintToStr(Node * root);
void DeleteTree(Node * root);




int main()
{
	printf("Hello, world!");

	return 0;
}



//IMPLEMENTATION OF FUNCTIONS

Node * Diff(const Node * root)
{
	switch (root->Type)
	{
	case NUMBER:
		return ZERO;
	case VARIABLE:
		return ONE;
	case OPERATOR:
		switch (root->Value)
		{
		case PLUS:	// (u+v)' = u' + v'
			return _PLUS(Diff(root->Left), Diff(root->Right));
		case MINUS:	// (u-v)' = u' - v'
			return _MINUS(Diff(root->Left), Diff(root->Right));
		case MULTIPLY:	// (uv)' = u'v + uv'
		{
			Node * u1 = Diff(root->Left);	//u'
			Node * v = Copy(root->Right);	//v
			Node * u = Copy(root->Left);	//u
			Node * v1 = Diff(root->Right);	//v'

			return _PLUS(_MUL(u1, v), _MUL(u, v1));
		}	
		case DIVIDE:	// (u/v)' = (u'v - uv') / v^2
		{
			Node * u1 = Diff(root->Left);	//u'
			Node * v = Copy(root->Right);	//v
			Node * u = Copy(root->Left);	//u
			Node * v1 = Diff(root->Right);	//v'

			Node * numerator = _MINUS(_MUL(u1, v), _MUL(u, v1));	//числитель
			
			Node * denominator = _SQUARE(v);	//знаменатель

			return _DIVIDE(numerator, denominator);
		}}
	case FUNCTION:
		switch (root->Value)
		{
		
		}
	}
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

	return ret;
}

Node * CreateNode(int type, int value, Node * left, Node * right) 
{
	Node * ret = (Node *) calloc(1, sizeof(Node));

	ret->Type = type;
	ret->Value = value;
	ret->Left = left;
	ret->Right = right;

	return ret;
}

void StrToFile(Data str)	 
{
	FILE *file;
	file = fopen("base.txt", "w");
	assert(file);

	fprintf(file, "\\documentclass[a4paper,12pt]{article}\n");
	fprintf(file, "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools, }\n");
	fprintf(file, "\\begin{document}\n");

	fprintf(file, "\[");

	fprintf(file, "%s", str);

	fprintf(file, "\]");

	fprintf(file, "\\end{document}");

	fclose(file);
}

void SaveTree(Node * root)
{
	Data str = PrintToStr(root);
	assert(str);

	StrToFile(str);

	DeleteTree(root);

	free(str);
}

Data PrintToStr(Node * root)
{
	assert(root);	

	Data str = (Data) calloc(MaxSizeStr, sizeof(char));

	Data left_str;
	Data  right_str;

	switch (root->Type)
	{
	case NUMBER:
	{
		sprintf(str, "%d", root->Value);
		return str;
	}
	case VARIABLE:
	{
		sprintf(str, "x");
		return str;
	}	
	case OPERATOR:
		switch (root->Value)
		{
		case PLUS:
		{
			left_str = PrintToStr(root->Left);
			right_str = PrintToStr(root->Right);
		
			strcat(str, left_str);
			strcat(str, "+");
			strcat(str, right_str);

			free(left_str);
			free(right_str);
			return str;
		}
		case MINUS:
		{
			left_str = PrintToStr(root->Left);
			right_str = PrintToStr(root->Right);
		
			strcat(str, left_str);
			strcat(str, "-");
			strcat(str, right_str);

			free(left_str);
			free(right_str);
			return str;
		}	
		case MULTIPLY:
		{
			left_str = PrintToStr(root->Left);
			right_str = PrintToStr(root->Right);
			strcat(str, "(");
			strcat(str, left_str);
			strcat(str, ")(");
			strcat(str, right_str);
			strcat(str, ")");

			free(left_str);
			free(right_str);
			return str;		
		}	
		case DIVIDE:	
		{
			left_str = PrintToStr(root->Left);
			right_str = PrintToStr(root->Right);
			strcat(str, "(");
			strcat(str, left_str);
			strcat(str, ")\div(");
			strcat(str, right_str);
			strcat(str, ")");

			free(left_str);
			free(right_str);
			return str;		
		}	
		}
	case FUNCTION:
		switch (root->Value)
		{
		
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
