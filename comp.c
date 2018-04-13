#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"
#include "stack.c"
#include "lex.h"
#include "lex.c"
#include "tree.h"
#include "tree.c"
#include "run.h"
#include "run.c"

char * FileToStr();

int main()
{
	Node * sp = CreateStack();
	int rax, rbx, rcx, rdx;

	сhar * str = FileToStr();
	Token * tokens = LexicalAnalysis(str);
	Tree_Node * root = CreateTree(tokens);
	Run(root);

	return 0;
}









