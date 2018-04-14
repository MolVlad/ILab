#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stack.h"
#include "stack.c"
#include "lex.h"
#include "lex.c"
#include "run.h"
#include "run.c"

char * FileToStr();

Node * sp;
int rax, rbx, rcx, rdx;

int main()
{
	sp = CreateStack();
	rax = rbx = rcx = rdx = 0;

	char * str = FileToStr();
	printf("ASDGHJHJEGIOERG\n\n\n\n");
	Token * tokens = LexicalAnalysis(str);
//	Run(tokens);

	return 0;
}









