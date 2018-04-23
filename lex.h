enum {BIN_OPERATOR, NUMBER, REGISTER, PUNCTUATION, FUNCTION, END};

enum {PLUS, MINUS, DIVIDE, MULTIPLY, DEGREE};	// BIN_OPERATOR
enum {RAX, RBX, RCX, RDX};	// REGISTER
enum {IN, OUT, PUSH, POP, ADD, SUB, MUL, DIV};	// FUNCTION
enum {BRACKET_OPENING, BRACKET_CLOSE, NEW_LINE, EQUALITY};	//PUNCTUATION

typedef struct
{
	int Type;
	int Value;
	int Num_Str;
	int Num_Word;
} Token;

char * FileToStr();
Token * LexicalAnalysis(char * str);


