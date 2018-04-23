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
#include "check.h"
#include "check.c"

char * FileToStr();

int main()
{

	char * str = FileToStr();
	Token * tokens = LexicalAnalysis(str);
	Check(tokens);
//	Run(tokens);

	return 0;
}









