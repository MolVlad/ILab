enum {BIN_OPERATOR = 0, OPERATOR = 1, NUMBER, REGISTER, PUNCTUATION, FUNCTION, CONST};

enum {PLUS = 0, MINUS, DIVIDE, MULTIPLY, DEGREE};	// BIN_OPERATOR
enum {SIN, COS, TG, LN};	//OPERATOR
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




