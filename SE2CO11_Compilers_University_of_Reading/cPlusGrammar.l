/* CPlusGrammar.l
 * input file for flex
 * Inspired by the file by Jeff Lee
 ** found http://www.quut.com/c/ANSI-C-grammar-l.html/ */

/* Regular Expression Definitions */
BIN		([01])
OCTAL	[0-7]
NZERO	[1-9]
DECIM	[0-9]
HEX		[a-fA-F0-9]
LETTR	[a-zA-Z_]
ALNUM	[a-zA-Z_0-9]
HEXPR	(0[xX])
BINPR	(0[bB])
E		([Ee][+-]?{DECIM}+)
P		([Pp][+-]?{DECIM}+)
FS		(f|F|l|L)
IS		(((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP		(u|U|L)
SP		(u8|u|U|L)
ES		(\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WHSPC	[ \t\n\v\f]

%{
#include <stdio.h>
#include "cPlusParser.tab.h"
#include <math.h>

extern YYSTYPE yylval; 						/* For backwards compatibility with lex/yacc */
extern void yyerror(const char *);			/* prints grammar violation message */
extern int sym_type(const char *);			/* returns type from symbol table */

// Type numbers
#define NO_TYPE       -1
#define VOID_TYPE_NUM 0 // void
#define INT_TYPE_NUM  1 // int
#define LONG_TYPE_NUM 2 // long
#define SHRT_TYPE_NUM 3 // short
#define USGN_TYPE_NUM 4 // unsigned
#define FLT_TYPE_NUM  5 // float
#define DBL_TYPE_NUM  6 // double
#define CHAR_TYPE_NUM 7 // char
#define BOOL_TYPE_NUM 8 // bool
#define PTR_TYPE_NUM  9 // generalized pointer
extern int gTypeNum; // So I can access them in the parser

extern int parsingIOStatement;

// Funcitons I need
static void blockComment(void);
static void preproc(void);
static int bintoint(char* str);
static int hextoint(char* str);
%}

/* Options */
%option yylineno

/* Definitions */
%%
"/*"									{ blockComment(); }
"//".*									{ /* consume //-comment */ }
"#"										{ preproc(); /* I just write preprocessor statements directly to output */ }

"bool"									{ gTypeNum = BOOL_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(BOOL); }
"break"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(BREAK); }
"case"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(CASE); }
"char"									{ gTypeNum = CHAR_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(CHAR); }
"cin"									{ yylval.string=strdup(yytext); return(CIN); }
"const"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(CONST); }
"continue"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(CONTINUE); }
"cout"									{ yylval.string=strdup(yytext); return(COUT); }
"default"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(DEFAULT); }
"do"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(DO); }
"double"								{ gTypeNum = DBL_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(DOUBLE); }
"else"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(ELSE); }
"endl"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(ENDL); }
"enum"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(ENUM); }
"extern"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(EXTERN); }
"float"									{ gTypeNum = FLT_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(FLOAT); }
"for"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(FOR); }
"if"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(IF); }
"int"									{ gTypeNum = INT_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(INT); }
"long"									{ gTypeNum = LONG_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(LONG); }
"return"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(RETURN); }
"short"									{ gTypeNum = SHRT_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(SHORT); }
"signed"								{ gTypeNum = INT_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(SIGNED); }
"sizeof"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(SIZEOF); }
"static"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(STATIC); }
"struct"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(STRUCT); }
"switch"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(SWITCH); }
"typedef"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(TYPEDEF); }
"union"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(UNION); }
"unsigned"								{ gTypeNum = USGN_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(UNSIGNED); }
"void"									{ gTypeNum = VOID_TYPE_NUM; if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(VOID); }
"while"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(WHILE); }
"__func__"								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return(FUNC_NAME); }

{LETTR}{ALNUM}*							{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return IDENTIFIER; }

{BINPR}{BIN}+{IS}?						{ if(!parsingIOStatement) printf("%s ", yytext); yylval.intNum=bintoint(yytext); return INT_CONSTANT; }
{HEXPR}{HEX}+{IS}?						{ if(!parsingIOStatement) printf("%s ", yytext); yylval.intNum=hextoint(yytext); return INT_CONSTANT; }
[+-]?{NZERO}{DECIM}*{IS}?				{ if(!parsingIOStatement) printf("%s ", yytext); yylval.intNum=atoi(yytext); return INT_CONSTANT; }
"0"{OCTAL}*{IS}?						{ if(!parsingIOStatement) printf("%s ", yytext); yylval.intNum=atoi(yytext); return INT_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"				{ if(!parsingIOStatement) printf("%s ", yytext); yylval.intNum=atoi(yytext); return INT_CONSTANT; }

{DECIM}+{E}{FS}?						{ if(!parsingIOStatement) printf("%s ", yytext); yylval.floatingPt=atof(yytext); return F_CONSTANT; }
{DECIM}*"."{DECIM}+{E}?{FS}?			{ if(!parsingIOStatement) printf("%s ", yytext); yylval.floatingPt=atof(yytext); return F_CONSTANT; }
{DECIM}+"."{E}?{FS}?					{ if(!parsingIOStatement) printf("%s ", yytext); yylval.floatingPt=atof(yytext); return F_CONSTANT; }
{HEXPR}{HEX}+{P}{FS}?					{ if(!parsingIOStatement) printf("%s ", yytext); yylval.floatingPt=atof(yytext); return F_CONSTANT; }
{HEXPR}{HEX}*"."{HEX}+{P}{FS}?			{ if(!parsingIOStatement) printf("%s ", yytext); yylval.floatingPt=atof(yytext); return F_CONSTANT; }
{HEXPR}{HEX}+"."{P}{FS}?				{ if(!parsingIOStatement) printf("%s ", yytext); yylval.floatingPt=atof(yytext); return F_CONSTANT; }

({SP}?\"([^"\\\n]|{ES})*\"{WHSPC}*)+	{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return STRING_LITERAL; }

">>="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return RIGHT_ASSIGN; }
"<<="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return LEFT_ASSIGN; }
"+="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return ADD_ASSIGN; }
"-="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return SUB_ASSIGN; }
"*="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return MUL_ASSIGN; }
"/="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return DIV_ASSIGN; }
"%="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return MOD_ASSIGN; }
"&="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return AND_ASSIGN; }
"^="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return XOR_ASSIGN; }
"|="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return OR_ASSIGN; }
">>"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return RIGHT_OP; }
"<<"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return LEFT_OP; }
"++"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return INC_OP; }
"--"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return DEC_OP; }
"->"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return PTR_OP; }
"&&"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return AND_OP; }
"||"									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return OR_OP; }
"<="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return LE_OP; }
">="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return GE_OP; }
"=="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return EQ_OP; }
"!="									{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return NE_OP; }
";"										{ if(!parsingIOStatement) printf("%s\n", yytext); yylval.string=strdup(yytext); return ';'; }
("{"|"<%")								{ if(!parsingIOStatement) printf("%s\n", yytext); yylval.string=strdup(yytext); return '{'; }
("}"|"%>")								{ if(!parsingIOStatement) printf("%s\n", yytext); yylval.string=strdup(yytext); return '}'; }
","										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return ','; }
":"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return ':'; }
"="										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '='; }
"("										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '('; }
")"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return ')'; }
("["|"<:")								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '['; }
("]"|":>")								{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return ']'; }
"."										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '.'; }
"&"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '&'; }
"!"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '!'; }
"~"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '~'; }
"-"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '-'; }
"+"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '+'; }
"*"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '*'; }
"/"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '/'; }
"%"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '%'; }
"<"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '<'; }
">"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '>'; }
"^"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '^'; }
"|"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '|'; }
"?"										{ if(!parsingIOStatement) printf("%s ", yytext); yylval.string=strdup(yytext); return '?'; }

{WHSPC}+								{ /* whitespace separates tokens */ }
.										{ /* discard bad characters */ }

%%

int yywrap(void) {		/* called at the end of parsing */
	return 1;           /* terminate now */
}

static void preproc(void)
{
	int c;
	char buffer[100];
	int i = 1;
	buffer[0] = '#';

	// Copy characters until we find '\n'
	while((c = input()) != 0)
	{
		if(c == '\n')
		{
			buffer[i] = '\0';
			printf("%s\n", buffer);
			return;
		}
		buffer[i] = c;
		i++;
	}
	yyerror("Bad Preprocessor Statement");
}

static void blockComment(void)
{
	int c;

	// Just eat characters until we find "*/"
	while ((c = input()) != 0)
	{
		if (c == '*')
		{
			while ((c = input()) == '*')
				;

			if (c == '/')
				return;

			if (c == 0)
				break;
		}
	}
	yyerror("unterminated comment");
}

static int bintoint(char* str)
{
	// We know that b is in str[1], so start parsing strt a[2]
	int len = strlen(str);
	int i = len;
	int ret = 0;
	while(i >= 2)
	{
		if (str[i] == '1')
		{
			ret += (1 << (len-i-1));
		}
		i--;
	}
	return ret;
}

static int hextoint(char* str)
{
	int len = strlen(str);
	int i = len;
	int ret = 0;
	while(i >= 2)
	{
		switch (str[i])
		{
			case '1':
			ret += 1 * pow(16, len-i-1);
			break;

			case '2':
			ret += 2 * pow(16, len-i-1);
			break;

			case '3':
			ret += 3 * pow(16, len-i-1);
			break;

			case '4':
			ret += 4 * pow(16, len-i-1);
			break;

			case '5':
			ret += 5 * pow(16, len-i-1);
			break;

			case '6':
			ret += 6 * pow(16, len-i-1);
			break;

			case '7':
			ret += 7 * pow(16, len-i-1);
			break;

			case '8':
			ret += 8 * pow(16, len-i-1);
			break;

			case '9':
			ret += 9 * pow(16, len-i-1);
			break;

			case 'a':
			case 'A':
			ret += 10 * pow(16, len-i-1);
			break;

			case 'b':
			case 'B':
			ret += 11 * pow(16, len-i-1);
			break;

			case 'c':
			case 'C':
			ret += 12 * pow(16, len-i-1);
			break;

			case 'd':
			case 'D':
			ret += 13 * pow(16, len-i-1);
			break;

			case 'e':
			case 'E':
			ret += 14 * pow(16, len-i-1);
			break;

			case 'f':
			case 'F':
			ret += 15 * pow(16, len-i-1);
			break;


			case '0':
			default:
			ret += 0;
			break;
		}
		i--;
	}
	return ret;
}
