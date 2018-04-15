enum {BIN_OPERATOR, OPERATOR, NUMBER, REGISTER, PUNCTUATION, FUNCTION, CONST};

enum {PLUS, MINUS, DIVIDE, MULTIPLY, DEGREE};	// BIN_OPERATOR
enum {SIN, COS, TG, LN};	//OPERATOR
enum {RAX, RBX, RCX, RDX};	// REGISTER
enum {IN, OUT, PUSH, POP, ADD, SUB, MUL, DIV};	// FUNCTION
enum {BRACKET_OPENING, BRACKET_CLOSE, NEW_LINE, EQUALITY};	//PUNCTUATION

typedef struct
{
	int Type;
	int Value;
	struct Token * Next;
} Token;

char * FileToStr();
Token * LexicalAnalysis(char * str);
