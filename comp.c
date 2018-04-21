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

int main()
{
	sp = CreateStack();
	rax = rbx = rcx = rdx = 0;

	char * str = FileToStr();
	Token * tokens = LexicalAnalysis(str);
	Run(tokens);

	return 0;
}









