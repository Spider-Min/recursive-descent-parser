/*
 ============================================================================
 Name        : rdparser.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype {
	WHILE = 258,
	FOR = 259,
	IF = 260,
	ELSE = 261,
	PRINT = 262,
	SCAN = 263,
	RETURN = 264,
	INT = 265,
	ID = 266,
	STR = 267,
	VOID = 268,
	LOWER = 269,
	BIGER = 270,
	LEQU = 271,
	BEQU = 272,
	EQU = 273,
	UEQU = 274,
	ASSIGN = 275,
	AND = 276,
	OR = 277,
	STRING = 278,
	EOL = 279,
	NUMBER = 280,
};


extern int yylex();
extern int yylval;
extern char* yytext;
extern FILE* yyin;

int cmp_expr();
int expr();
int primary_expr();
int add_expr();
int mul_expr();
int expr_list();
void program();
void external_declaration();
void decl_or_stmt();
void declarator_list();
void intstr_list();
void initializer();
void declarator();
void parameter_list();
void parameter();
int type();
void statement();
void statement_list();
void expression_statement();
void id_list();
void assign();

int tok;


void advance()
{
	tok = yylex();
	if(tok == EOL)
	{
		printf("\n");
		advance();
	}
	else
		printf("tok: %s\t", yytext);
}

void program()
{
	advance();
	external_declaration();
	while(tok == INT || tok == STR || tok == VOID )
	{
		external_declaration();
	}
}

void external_declaration()
{
	int t = type();
	declarator();
	decl_or_stmt();
}


void decl_or_stmt()
{
	if(tok == '{')
	{
		advance();
		if(tok == '}')
			advance();
		else
		{
			statement_list();
			if(tok == '}')
				advance();
		}
		return;
	}
	else if(tok == ',')
	{
		advance();
		declarator_list();
		if(tok == ';')
			advance();
		return;
	}
	else if(tok == ';')
	{
		advance();
		return;
	}
}

void declarator_list()
{
	declarator();
	while(tok == ',')
	{
		advance();
		declarator();
	}
}

void intstr_list()
{
	initializer();
	while(tok == ',')
	{
		advance();
		initializer();
	}
}

void initializer()
{
	if(tok == NUMBER || tok == STRING)
		advance();
}


void declarator()
{
	if(tok == ID)
	{
		advance();
		if(tok == '(' || tok == '=' || tok == '[')
		{
			advance();
			//ID '(' ')' || ID '[' ']' || ID '[' ']' '=' '{' intstr_list '}'
			if(tok == ']' || tok == ')')
			{
				advance();
				if(tok == '=')
				{
					advance();
					if(tok == '{')
					{
						advance();
						intstr_list();
						if(tok == '}')
							advance();
					}
				}
			}
			//ID '(' parameter_list ')'
			else if(tok == INT || tok == STR || tok == VOID)
			{
				parameter();
				while(tok == ',')
				{
					advance();
					parameter();
				}
				if(tok == ')')
					advance();
			}
			//ID '=' expr || ID '[' expr ']' '=' '{' intstr_list '}'
			else
			{
				expr();
				if(tok == ']')
				{
					advance();
					if(tok == '=')
					{
						advance();
						if(tok == '{')
						{
							advance();
							intstr_list();
							if(tok == '}')
								advance();
						}
					}
				}
			}
		}
		else
			return;
	}
}


void parameter_list()
{
	parameter();
	while(tok == ',')
	{
		advance();
		parameter();
	}
}


void parameter()
{
	int t = type();
	if(tok == ID)
		advance();
	return;
}



int type()
{
	int returnType;
	if(tok == INT)
		returnType = INT;
	else if(tok == STR)
		returnType = STR;
	else if(tok == VOID)
		returnType = VOID;
	advance();
	return returnType;
}


void statement()
{
	if(tok == RETURN || tok == WHILE || tok == PRINT || tok == IF || tok == '{' || tok == SCAN) 
	{
		if(tok == RETURN)
		{
			advance();
			if(tok == ';')
				advance();
			else
			{
				expr();
				if(tok == ';')
					advance();
			}
		}
		else if(tok == PRINT)
		{
			advance();
			if(tok == ';')
				advance();
			else
			{
				expr_list();
				if(tok == ';')
					advance();
			}
		}
		else if(tok == WHILE)
		{
			advance();
			if(tok == '(')
			{
				advance();
				expr();
				if(tok == ')')
				{
					advance();
					statement();
				}
			}
		}
		else if(tok == IF)
		{
			advance();
			if(tok == '(')
			{
				advance();
				expr();
				if(tok == ')')
				{
					advance();
					statement();
					if(tok == ELSE)
					{
						advance();
						statement();
					}
				}
			}
		}
		else if(tok == '{')
		{
			advance();
			statement_list();
			if(tok == '}')
				advance();
		}
		else if(tok == SCAN)
		{
			id_list();
			if(tok == ';')
				advance();
		}
	}
	else if(tok == ';' || tok == '-' || tok == ID || tok == NUMBER || tok == STRING || tok == '(' )
	{
		if(tok == ';')
			advance();
		else 
		{
			expr();
			if(tok == ';')
				advance();
		}
	}
	else 
	{
		int t = type();
		declarator_list();
		if(tok == ';')
			advance();
	}
}


void statement_list()
{
	statement();
	while(tok == IF || tok == WHILE || tok == RETURN || tok == PRINT || tok == SCAN || tok == '{' || tok == INT || tok == STR || tok == VOID || tok == ';' || tok == '-' || tok == ID || tok == '(' || tok == NUMBER ) 
	{
		statement();
	}
}


void expression_statement()
{
	if(tok == ';')
		advance();
	else
	{
		expr();
		if(tok == ';')
			advance();
	}
}


int expr()
{
	int c = cmp_expr();
	return c;
}

//cmp_expr: add_expr
//		  | cmp_expr CMP add_expr
//		  ;

int cmp_expr()
{
	int a = add_expr();
	while(tok == LOWER || tok == BIGER || tok == LEQU || tok == BEQU || tok == EQU || tok == UEQU)
	{
		int oper = tok;
		advance();
		int a2 = add_expr();
		if(oper == LOWER )
			a = a < a2;
		else if(oper == BIGER)
			a = a > a2;
		else if(oper == LEQU)
			a = a <= a2;
		else if(oper == BEQU)
			a = a >= a2;
		else if(oper == EQU)
			a = a == a2;
		else
			a = a != a2;
	}
	return a;
}

//add_expr: mul_expr
//	| add_expr '+' mul_expr
//	| add_expr '-' mul_expr
//	;

int add_expr()
{
	int l = mul_expr();
	while(tok == '+' || tok == '-')
	{
		int oper = tok;
		advance();
		int r = mul_expr();
		if( oper == '+')
			l += r;
		else
			l -= r;
	}
	return l;
}

//mul_expr: primary_expr
//	| mul_expr '*' primary_expr
//	| mul_expr '/' primary_expr
//	| mul_expr '%' primary_expr
//  | '-' primary_expr

int mul_expr()
{
	if(tok == '-')
	{
		advance();
		int p = primary_expr();
		return -p;
	}
	int l = primary_expr();
	while(tok == '*' || tok == '/' || tok == '%')
	{
		int oper = tok;
		advance();
		int r = primary_expr();
		if( oper == '*')
			l *= r;
		else if( oper == '/')
			l /= r;
		else if( oper == '%')
			l %= r;
	}
	return l;
}


//primary_expr: NUMBER
// | ID '(' expr_list ')'
// | ID '(' ')'
// | '(' expr ')'
// | ID
// | STRING
// | ID ASSIGN expr
// | ID '=' expr
// | ID '[' expr ']'
// | ID '[' expr ']' '=' expr
// ;

int primary_expr()
{
	if(tok == ID)
	{
		advance();
		if(tok == '(')
		{
			advance();
			if(tok == ')')
				advance();
			else
			{
				expr_list();
				if(tok == ')')
					advance();
			}
		}
		else if(tok == '=')
		{
			advance();
			expr();
		}
		else if(tok == '[')
		{
			advance();
			expr();
			if(tok == ']')
			{
				advance();
				if(tok == '=')
				{
					advance();
					expr();
				}
			}
		}
		else
		{
			assign();
			expr();
		}
	}
	else if(tok == '(')
	{
		advance();
		expr();
		if(tok == ')')
			advance();
	}
	else if(tok == NUMBER)
		advance();
	else if(tok == STRING)
		advance();
	else if(tok == 'q')
		exit(0);
	return 1;
}

void assign()
{
	if(tok == ASSIGN)
	{
		advance();
	}
}

int expr_list()
{
	expr();
	while(tok == ',')
	{
		advance();
		expr();
	}
	return 1;
}


void id_list()
{
	if(tok == ID)
		advance();
	while(tok == ',')
	{
		advance();
		if(tok == ID)
			advance();
	}
}



typedef struct _ast ast;
typedef struct _ast *past;
struct _ast{
	int ivalue;
	char* nodeType;
	past left;
	past right;
};

past newAstNode()
{
	past node = malloc(sizeof(ast));
	if(node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	return node;
}

past newNum(int value)
{
	past var = newAstNode();
	var->nodeType = "intValue";
	var->ivalue = value;
	return var;
}
past newExpr(int oper, past left,past right)
{
	past var = newAstNode();
	var->nodeType = "expr";
	var->ivalue = oper;
	var->left = left;
	var->right = right;
	return var;
}


past astTerm()
{
	if(tok == INT)
	{
		past n = newNum(yylval);
		advance();
		return n;
	}
	else if(tok == '-')
	{
		advance();
		past k = astTerm();
		past n = newExpr('-', NULL, k);
		return n;
	}
	else if(tok == 'q')
		exit(0);
	return NULL;
}


past astFactor()
{
	past l = astTerm();
	while(tok == '*' || tok == '/')
	{
		int oper = tok;
		advance();
		past r = astTerm();
		l = newExpr(oper, l, r);
	}
	return l;
}

past astExpr()
{
	past l = astFactor();
	while(tok == '+' || tok == '-')
	{
		int oper = tok;
		advance();
		past r = astFactor();
		l = newExpr(oper, l, r);
	}
	return l;
}


void showAst(past node, int nest)
{
	if(node == NULL)
		return;

	int i = 0;
	for(i = 0; i < nest; i ++)
		printf("  ");
	if(strcmp(node->nodeType, "intValue") == 0)
		printf("%s %d\n", node->nodeType, node->ivalue);
	else if(strcmp(node->nodeType, "expr") == 0)
		printf("%s '%c'\n", node->nodeType, (char)node->ivalue);
	showAst(node->left, nest+1);
	showAst(node->right, nest+1);
}

int main(int argc, char **argv)
{
//	if(argc != 2)
//	{
//		printf("input file is needed.\n");
//		return 0;
//	}
//	FILE* f = fopen(argv[1]);
	yyin = fopen("test.c","r");
	program();
	
//	while(1)
//	{
//		printf("input expression, 'q' to exit>");
//		advance();
//		int r = expr();
		//printf("result: %d\n", r);

		//past rr = astExpr();
		//showAst(rr, 0);
//	}
	return 0;
}
