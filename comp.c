#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"
#include "stack.c"

enum {BIN_OPERATOR, OPERATOR, NUMBER, REGISTER, PUNCTUATION, FUNCTION, CONST};

enum {PLUS, MINUS, DIVIDE, MULTIPLY};	// BIN_OPERATOR
enum {SIN, COS, TG, ARCSIN, ARCTG, EXP, LOG, LN, DEGREE};	//OPERATOR
enum {RAX, RBX, RCX, RDX};	// REGISTER
enum {IN, OUT, PUSH, POP, ADD, SUB, MUL, DIV};	// FUNCTION

int main()
{
/*	Node * stack = CreateStack();

	stack = PushStack(stack, 3);
	stack = PushStack(stack, 2);
	stack = PushStack(stack, 5);
	stack = PushStack(stack, 4);

	AddStack(stack);
	DotStack(stack);
	PrintStack(stack);
//	DeleteStack(stack);
*/
	return 0;
}










