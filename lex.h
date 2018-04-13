enum {BIN_OPERATOR, OPERATOR, NUMBER, REGISTER, PUNCTUATION, FUNCTION, CONST};

enum {PLUS, MINUS, DIVIDE, MULTIPLY};	// BIN_OPERATOR
enum {SIN, COS, TG, ARCSIN, ARCTG, EXP, LOG, LN, DEGREE};	//OPERATOR
enum {RAX, RBX, RCX, RDX};	// REGISTER
enum {IN, OUT, PUSH, POP, ADD, SUB, MUL, DIV};	// FUNCTION

typedef struct
{
	int Type;
	int Value;
	struct Token * Next;
} Token;

char * FileToStr();
Token * LexicalAnalysis(char * str);




