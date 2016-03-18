/* CPlusGrammar.y
 * input file for yacc
 * Inspired by the original ANSI-C specification by Jeff Lee
 ** found www.quut.com/c/ANSI-C-grammar-y.html */

%{
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <float.h>
#include <errno.h>

// For keeping track of types and identifiers
#define NO_TYPE      -1
#define VOID_TYPE_NUM 0 // void
#define INT_TYPE_NUM  1 // Int
#define LONG_TYPE_NUM 2 // Long
#define SHRT_TYPE_NUM 3 // Short
#define USGN_TYPE_NUM 4 // Unsigned
#define FLT_TYPE_NUM  5 // Float
#define DBL_TYPE_NUM  6 // Double
#define CHAR_TYPE_NUM 7 // Char
#define BOOL_TYPE_NUM 8 // Bool
// Repeat for pointers (add 10 from regular type),
// just handle the special case of pointer pointer
// (e.g. char** is a pointer to a pointer to a char)
#define PPTR_TYPE_NUM 9 // pointer pointer
#define VPTR_TYPE_NUM 10 // void pointer
#define IPTR_TYPE_NUM 11 // int pointer
#define LPTR_TYPE_NUM 12 // long pointer
#define SPTR_TYPE_NUM 13 // short pointer
#define UPTR_TYPE_NUM 14 // ungigned pointer
#define FPTR_TYPE_NUM 15 // float pointer
#define DPTR_TYPE_NUM 16 // double pointer
#define CPTR_TYPE_NUM 17 // char pointer

const char *typeLUT[18] = {"void", "int", "long", "short", "unsigned", 
	"flaot", "double", "char", "bool", "ptr *", "void *", "int *", "long *", 
	"short *", "unsgnd *", "float *", "double *", "char *"};

// I could do this much better with a struct array but I'm lazy and this works
int typeArray[7901];
char symbolTable[7901][32];
int gTypeNum;

// For enabling/disabling output while parsing cin/cout statements
int parsingIOStatement = 0;

// Debugging
int yydebug=0;

extern FILE * yyin;
extern int yylex();
extern int yylineno;

void parseCinStatement();
void parseCoutStatement();
void yyerror(const char* s);
void insertIntoSymTab(char *str);
int symbolTableLookup(char *str);
void printTable();

%}

/* === Token Defintions === */
// Operators & Assignments
%token SIZEOF PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN

// Typedef Stuff
%token TYPEDEF_NAME ENUMERATION_CONSTANT
%token CIN COUT ENDL

// Types and Type Prefixes
%token TYPEDEF EXTERN STATIC CONST
%token BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID

// Stuff I have to keep
%token STRUCT UNION ENUM

// Flow control
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR CONTINUE BREAK RETURN

%union
{
        int intNum;
        double floatingPt;
        char *string;
}

// Things with types
%token <intNum> INT_CONSTANT
%token <floatingPt> F_CONSTANT
%token <string> STRING_LITERAL FUNC_NAME IDENTIFIER PREPROCSTMT

%start translation_unit
%%

primary_expression
	: IDENTIFIER
	| constant
	| string
	| '(' expression ')'
	;

constant
	: INT_CONSTANT
	| F_CONSTANT
	| ENUMERATION_CONSTANT	/* after it has been defined as such */
	;

enumeration_constant		/* before it has been defined as such */
	: IDENTIFIER
	;

string
	: STRING_LITERAL
	| FUNC_NAME
	;

output_statement
	: COUT									{ parsingIOStatement = 1; parseCoutStatement(); }
	;

input_statement
	: CIN 									{ parsingIOStatement = 1; parseCinStatement(); }
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	| '(' type_name ')' '{' initializer_list '}'
	| '(' type_name ')' '{' initializer_list ',' '}'
	;

argument_expression_list
	: assignment_expression 								
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression
	| relational_expression '>' shift_expression
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression	/* with constraints */
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	;

declaration_specifiers
	: storage_class_specifier declaration_specifiers
	| storage_class_specifier
	| type_specifier declaration_specifiers	
	| type_specifier 
	| type_qualifier declaration_specifiers	
	| type_qualifier
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator '=' initializer
	| declarator
	;

storage_class_specifier
	: TYPEDEF	/* identifiers must be flagged as TYPEDEF_NAME */
	| STATIC
	;

type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| BOOL
	| struct_or_union_specifier
	| enum_specifier
	| TYPEDEF_NAME
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list ';'	/* for anonymous struct/union */
	| specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: ':' constant_expression
	| declarator ':' constant_expression
	| declarator
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator	/* identifiers must be flagged as ENUMERATION_CONSTANT */
	: enumeration_constant '=' constant_expression
	| enumeration_constant
	;

type_qualifier
	: CONST
	;

declarator
	: pointer direct_declarator
	| direct_declarator
	;

direct_declarator
	: IDENTIFIER 								{ insertIntoSymTab($1); }
	| '(' declarator ')'
	| direct_declarator '[' ']'
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_declarator '[' STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list '*' ']'
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_declarator '[' type_qualifier_list ']'
	| direct_declarator '[' assignment_expression ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' ')'
	| direct_declarator '(' identifier_list ')'
	;

pointer
	: '*' type_qualifier_list pointer 		{ gTypeNum = 9; }
	| '*' type_qualifier_list				{ gTypeNum += 10; }
	| '*' pointer 							{ gTypeNum = 9; }
	| '*' 									{ gTypeNum += 10; }
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list abstract_declarator
	| specifier_qualifier_list
	;

abstract_declarator
	: pointer direct_abstract_declarator
	| pointer
	| direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' '*' ']'
	| '[' STATIC type_qualifier_list assignment_expression ']'
	| '[' STATIC assignment_expression ']'
	| '[' type_qualifier_list STATIC assignment_expression ']'
	| '[' type_qualifier_list assignment_expression ']'
	| '[' type_qualifier_list ']'
	| '[' assignment_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' '*' ']'
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list ']'
	| direct_abstract_declarator '[' assignment_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	| assignment_expression
	;

initializer_list
	: designation initializer
	| initializer
	| initializer_list ',' designation initializer
	| initializer_list ',' initializer
	;

designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' constant_expression ']'
	| '.' IDENTIFIER
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	| output_statement
	| input_statement
	;

labeled_statement
	: CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' '}'
	| '{'  block_item_list '}'
	;

block_item_list
	: block_item
	| block_item_list block_item
	;

block_item
	: declaration
	| statement
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' compound_statement ELSE compound_statement
	| IF '(' expression ')' compound_statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' compound_statement
	| DO compound_statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' compound_statement
	| FOR '(' expression_statement expression_statement expression ')' compound_statement
	| FOR '(' declaration expression_statement ')' compound_statement
	| FOR '(' declaration expression_statement expression ')' compound_statement
	;

jump_statement
	: CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
	;

translation_unit
	: external_declaration 
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

%%
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "\n\n*** %s\nLine %d\n", s, yylineno);
	exit(1);
}

// Insert into the symbol table
void insertIntoSymTab(char *str)
{
	long long destination = 5381;
	int c = str[0], i = 0;

	// Solve for the destination
	while (c)
	{
		c = str[i++];
		destination = ((destination << 5) + destination) + c; /* destination * 33 + c */
	}
	while(destination < 0)
	{
		destination += 7901;
	}
	destination %= 7901;

	// If the entry exists, find a new empty spot
	if (symbolTable[destination][0])
	{
		fprintf(stderr, "COLLISSION\n");
		// I'm not going to do a quadratic hash, linear should be fine for my purposes
		while(symbolTable[destination++][0])
		{
			// If our string is in the table, we have a redefinition error
			if(strcmp(str, symbolTable[destination]) == 0)
			{
				char err[80];
				sprintf(err, "ERROR: redefinition of symbol \"%s\"", str);
				yyerror(err);
			}
		}

		// No redefinition, save our symbol
		typeArray[destination] = gTypeNum;
		strcpy(symbolTable[destination], str);
	}
	else
	{	// Record the type and copy the string into our table
		typeArray[destination] = gTypeNum;
		strcpy(symbolTable[destination], str);
	}
}

// Look up the given string in our tabe, return it's location
// return -1 if not in the table
int symbolTableLookup(char *str)
{
	int location = 5381;
	int c = str[0], i = 0;

	// Solve for the location
	while (c)
	{
		c = str[i++];
		location = ((location << 5) + location) + c; /* location * 33 + c */
	}
	location %= 7901;

	// If there is a string at our position
	if(symbolTable[location][0])
	{
		while(symbolTable[location][0])
		{
			if(strcmp(symbolTable[location], str) == 0)
				return location;
			location++;
		}
	}

	return -1;
}

// When we call this funciton, "cin" is in yylval.string
void parseCinStatement()
{
	// Some variables I need
	int expectingRIGHTOPOrSemicolon = 1;
	int i = 0;
	int j = 0;

	// Queue to hold variables before they get put at the end of scanf
	int queue[80];
	int queueLoc = 0;

	char c;
	char temp[80];
	char formatString[200];

	// Nullify our arrays to avoid random shit being left over
	for (int i = 0; i < 200; ++i)
	{
		formatString[i] = 0;
	}

	strcpy(formatString, "scanf(\"");
	int formatStringLoc = 7;

	while(1)
	{
		if(expectingRIGHTOPOrSemicolon)
		{
			int i = yylex();
			// If it's a semicolon, end the while loop
			if(i == ';')
				break;

			// if it's not a LEFT_OP, throw a syntax error
			if(i != RIGHT_OP)
			{
				char *err;
				sprintf(err, "Syntax Error, expected \">>\" but found token \"%s\"", yylval.string);
				yyerror(err);
			}
			expectingRIGHTOPOrSemicolon = 0;
		}
		else
		{
			// We only know how to output STRING_LITERAL, IDENTIFIER, and endl
			switch (yylex())
			{
				case IDENTIFIER: {
					// We need to look up the type, and concatenate
					// Look up the type, insert the variable name into the queue,
					// and append the appropriate type specifier to formatString
					if((i = symbolTableLookup(yylval.string)) != -1)
					{
						// We found it
						switch (typeArray[i])
						{
							// Void and bool just get ignored
							case VOID_TYPE_NUM:
							case BOOL_TYPE_NUM:
							break;

							case INT_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'd';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case LONG_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'd';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case SHRT_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'd';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case USGN_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'u';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case FLT_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'f';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case DBL_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'f';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case CHAR_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'c';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							// This should never happen
							default:
							break;
						}
						queue[queueLoc] = 0;
					}
					else // Reference to undefined character
					{
						char err[80];
						sprintf(err, "Syntax error, reference to undefined variable \"%s\"", yylval.string);
						yyerror(err);
					}
				}	
				break;

				default: {
					char err[120];
					sprintf(err, "Syntax error\n expected identifier but instead found \"<<\"");
					yyerror(err);
				}
			}
			expectingRIGHTOPOrSemicolon = 1;
		}
	}
	formatString[formatStringLoc++] = '"';

	// empty the queue
	// The queue contains the location in the symbol table for each element
	queueLoc = 0;
	while(queue[queueLoc])
	{
		i = 0;
		formatString[formatStringLoc++] = ',';
		formatString[formatStringLoc++] = ' ';
		while(formatStringLoc && (c = symbolTable[queue[queueLoc]][i++]))
		{
			formatString[formatStringLoc++] = '&';
			formatString[formatStringLoc++] = c;
		}
		formatString[formatStringLoc] = '\0';
		queueLoc++;
	}

	formatString[formatStringLoc++] = ')';
	formatString[formatStringLoc++] = ';';
	formatString[formatStringLoc] = '\0';
	printf("%s\n", formatString);

	// Lex can be turned on again
	parsingIOStatement = 0;
	return;
}

// When we call this funciton, "cout" is in yylval.string
void parseCoutStatement()
{
	// Some variables I need
	int expectingLEFTOPOrSemicolon = 1;
	int i = 0;
	int j = 0;
	
	// Queue for which variable goes where in the final printf statement
	int queue[80];
	int queueLoc = 0;

	char c;
	char temp[80];
	char formatString[200];

	// Nullify our arrays to avoid random shit being left over
	for (int i = 0; i < 80; ++i)
	{
		queue[i] = 0;
	}
	for (int i = 0; i < 200; ++i)
	{
		formatString[i] = 0;
	}

	strcat(formatString, "printf(\"");
	int formatStringLoc = 8;

	while(1)
	{
		if(expectingLEFTOPOrSemicolon)
		{
			int i = yylex();
			// If it's a semicolon, end the while loop
			if(i == ';')
				break;

			// if it's not a LEFT_OP, throw a syntax error
			if(i != LEFT_OP)
			{
				char *err;
				sprintf(err, "Syntax Error, expected \"<<\" but found token \"%s\"", yylval.string);
				yyerror(err);
			}
			expectingLEFTOPOrSemicolon = 0;
		}
		else
		{
			// We only know how to output STRING_LITERAL, IDENTIFIER, and endl
			switch (yylex())
			{
				case STRING_LITERAL: {
					// Copy the string to formatString one
					// character at a time, so I can catch if the programmer wants 
					// to output a '%'
					// i is location in yylval.string, j is location in temp
					// i starts at one to ignore the first '\"'
					i = 1;
					j = strlen(yylval.string);
					// j-1 because we need to ignore the last '\"'
					while((c = yylval.string[i++]) && (i < (j-1)))
					{
						if(c == '%')
						{
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = '%';
						}
						else
						{
							formatString[formatStringLoc++] = c;
						}
					}
					formatString[formatStringLoc] = '\0';
				}
				break;

				case ENDL: {
					// Just add an end of line character by character
					formatString[formatStringLoc++] = '\\';
					formatString[formatStringLoc++] = 'n';
					formatString[formatStringLoc] = '\0';
				}
				break;

				case IDENTIFIER: {
					// We need to look up the type, and concatenate
					// Look up the type, insert the variable name into the queue,
					// and append the appropriate type specifier to formatString
					if((i = symbolTableLookup(yylval.string)) != -1)
					{
						// We found it
						switch (typeArray[i])
						{
							// Void and bool just get ignored
							case VOID_TYPE_NUM:
							case BOOL_TYPE_NUM:
							break;

							case INT_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'd';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case LONG_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'd';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case SHRT_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'd';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case USGN_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'u';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case FLT_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'f';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case DBL_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'f';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							case CHAR_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'c';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							// All pointer types just output as the real hex address
							case PPTR_TYPE_NUM:
							case VPTR_TYPE_NUM:
							case IPTR_TYPE_NUM:
							case LPTR_TYPE_NUM:
							case SPTR_TYPE_NUM:
							case UPTR_TYPE_NUM:
							case FPTR_TYPE_NUM:
							case DPTR_TYPE_NUM:
							case CPTR_TYPE_NUM:
							formatString[formatStringLoc++] = '%';
							formatString[formatStringLoc++] = 'h';
							formatString[formatStringLoc] = '\0';
							queue[queueLoc++] = i;
							break;

							// This should never happen
							default:
							break;
						}
						queue[queueLoc] = 0;
					}
					else // Reference to undefined character
					{
						char err[80];
						sprintf(err, "Syntax error, reference to undefined variable \"%s\"", yylval.string);
						yyerror(err);
					}
				}	
				break;

				case INT_CONSTANT: {
					// Convert the integer to a string, and copy it's characters
					sprintf(temp, "%d", yylval.intNum);
					i = 0;
					while((c = temp[i++]) != 0)
					{
						formatString[formatStringLoc++] = c;
					}
					formatString[formatStringLoc] = '\0';
				}
				break;

				case F_CONSTANT: {
					// Convert the integer to a string, and copy it's characters
					sprintf(temp, "%f", yylval.floatingPt);
					i = 0;
					int passDot = 0;
					while((c = temp[i++]) != 0)
					{
						formatString[formatStringLoc++] = c;

						// I know I'm wasting a lot of time to switch this on
						// but compilers aren't supposed to be fast right?
						if (c == '.')
							passDot = 1;

						if (c == '0' && passDot)
						{
							if (temp[i-1] == '.')
							{
								formatString[formatStringLoc++] = c;
								break;
							}
							else
								break;
						}
					}
					formatString[formatStringLoc] = '\0';
				}
				break;

				default: {
					char err[120];
					sprintf(err, "Syntax error\n expected one of [\"string literal\", \"endl\", \"identifier\", \"constant\"], but instead found \"<<\"");
					yyerror(err);
				}
			}
			expectingLEFTOPOrSemicolon = 1;
		}
	}
	formatString[formatStringLoc++] = '"';

	// empty the queue
	// The queue contains the location in the symbol table for each element
	queueLoc = 0;
	while(queue[queueLoc])
	{
		i = 0;
		formatString[formatStringLoc++] = ',';
		formatString[formatStringLoc++] = ' ';
		while(formatStringLoc && (c = symbolTable[queue[queueLoc]][i++]))
		{
			formatString[formatStringLoc++] = c;
		}
		formatString[formatStringLoc] = '\0';
		queueLoc++;
	}

	formatString[formatStringLoc++] = ')';
	formatString[formatStringLoc++] = ';';
	formatString[formatStringLoc] = '\0';
	printf("%s\n", formatString);

	// Lex can be turned on again
	parsingIOStatement = 0;
	return;
}

void printTable()
{
	int i = 0;

	fprintf(stderr, "\n\nSymbol Table\n");
	for(; i < 7901; i++)
	{
		// No symbol is empty string a.k.a. '\0' a.k.k. the integer 0, so this works
		if(symbolTable[i][0])
		{
			fprintf(stderr, "%-6d", i);
			fprintf(stderr, "%-10s", typeLUT[typeArray[i]]);
			fprintf(stderr, "%s\n", symbolTable[i]);

		}
	}
}

void zerotable()
{
	int i = 7901;
	int j;
	for(; i > 0; i--)
	{
		typeArray[i] = NO_TYPE;
		for(j = 0; j < 32; j++)
			symbolTable[i][j] = 0;
	}
}

// return stdout file*
int setSTDOUTtoFile(const char* outname)
{
	// Set stdout to the user's specified file
	int dflt, file;
	fflush(stdout);
	dflt = dup(1);

	if ((file = open(outname, O_CREAT|O_WRONLY, 0777)) == -1)
	{
		perror("Error opening file:");
	}
	dup2(file, 1);
	close(file);

	return dflt;
}

void restoreSTDOUT(int dflt)
{
	// Restore Std out
	fflush(stdout);
	dup2(dflt, 1);
	close(dflt);
}

int main(int argc, char const *argv[])
{
	int i = 0;
	const char *outname = NULL;

	int pipestdout[2];
	pipe(pipestdout);

	zerotable();

	if (argc >= 2)
	{
		while(++i < argc && argv[i][0] == '-')
		{
			switch(argv[i][1]) {
				case 'd': {
					yydebug = 1;
				}
				break;

				case 'o': {
					outname = argv[++i];
				}
				break;

				default: {
					printf("Unrecognised parameter\n");
				}
				break;
			}
		}
	}
	else
	{
		printf("NOT ENOGH ARGUMENTS\n");
		printf("PLEASE USE\n");
		printf("$ compiler [-d] [-o outputFileName] input.cp\n");
		exit(0);
	}

	if(outname && (access(argv[i], F_OK) != -1))
	{
		// Set yyin
		yyin = fopen(argv[i], "r");

		i = setSTDOUTtoFile(outname);
		yyparse();
		restoreSTDOUT(i);
	}
	else if(access(argv[i], F_OK) != -1)
	{
		// Set yyin
		yyin = fopen(argv[i], "r");

		i = setSTDOUTtoFile("output.c");
		yyparse();
		restoreSTDOUT(i);
	}
	else
	{
		printf("BAD OR NO INPUT FILE\n");
		exit(0);
	}

	if (yydebug)
			printTable();
	return 0;
}
