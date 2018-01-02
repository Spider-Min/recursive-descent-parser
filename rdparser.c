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
	ADD_ASSIGN = 281,
	DEC_ASSIGN = 282,
	MUL_ASSIGN = 283,
	DIV_ASSIGN = 284,
	MOD_ASSIGN = 285
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
	char* nodeData;
	past left;
	past right;
};

past newList(int level, past list, past next);
past newExtDecl(char* type, past t, past ds);
past newType(int value);
past newDecl(past left, past right);
past newCompStmt(past sta_list);
past newDeclarator_list(past pl);
past newIfStmt(past e, past sta, past sta2);
past newIf_Else(past sta, past sta2);
past newWhileStmt(past e, past Sta);
past newReturnStmt(past e);
past newPrintStmt(past e);
past newScanStmt(past il);
past newExprStmt(past e);
past newID();
past newInitDecl(past id, past e);
past newFuncDecl(past id, past p);
past newArrayDecl(past id, past e);
past newArrayInitDecl(past id, past is);
past newInit(past id);
past newInitializer(int oper, char* data);
past newPrim_Expr(past id, past el);
past newPrim_Expr2(past id, past e);
past newPrim_Expr3(past id, past e, past e2);
past newArray(past id, past e);
past newPrim_Cmp_Expr(past e);
past newExpr(int oper, past left, past right);
past astProgram();
past astExternal();
past astDecl_Or_Stmt();
past astDeclarator_list();
past astDeclarator();
past astIntstr_list();
past astParameter_list();
past astParameter();
past astStatement_list();
past astStatement();
past astExpr_Stmt();
past astExpr();
past astCmp_Expr();
past astAdd_Expr();
past astMul_Expr();
past astPrimary_Expr();
past astExpr_list();
past astId_list();

char* initData()
{
	char* s = malloc(sizeof(char)*20);
	if(s == NULL)
	{
		printf("run out of memory\n");
		exit(0);
	}
	return s;
}
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
past newList(int value, past list, past next)
{
	past var = newAstNode();
	var->nodeType = "list";
	var->ivalue = value;
	var->left = list;
	var->right = next;
	return var;
}
//notetype == funcDecl / decl
past newExtDecl(char* type, past t, past ds)
{
	past var = newAstNode();
	var->nodeType = initData();
	strcpy(var->nodeType, type);
	var->ivalue = 0;
	var->left = t;
	var->right = ds;
	return var;
}
past newType(int value)
{
	past var = newAstNode();
	var->nodeType = "type";
	if(value == INT) var->nodeData = "int";
	else if(value == VOID) var->nodeData = "void";
	else if(value == STR) var->nodeData = "str";
	var->ivalue = value;
	var->left = NULL;
	var->right = NULL;
	return var;
}
past newDecl(past left, past right)
{
	past var = newAstNode();
	var->nodeType = "decl";
	var->ivalue = 1;
	var->left = left;
	var->right = right;
	return var;
}
past newCompStmt(past sta_list)
{
	past var = newAstNode();
	var->nodeType = "compStmt";
	var->ivalue = 0;
	var->left = sta_list;
	var->right = NULL;
	return var;
}
past newDecl_list(past pl)
{
	past var = newAstNode();
	var->nodeType = "decl_list";
	var->ivalue = 0;
	var->left = pl;
	var->right = NULL;
	return var;
}
past newIfStmt(past e, past sta, past sta2)
{
	past var = newAstNode();
	var->nodeType = "ifStmt";
	var->left = e;
	if(sta2 == NULL)
	{
		var->right = sta;
		var->ivalue = IF;
	}
	else
	{
		var->right = newIf_Else(sta, sta2);
		var->ivalue = ELSE;
	}
	return var;
}
past newIf_Else(past sta, past sta2)
{
	past var = newAstNode();
	var->nodeType = "if_else";
	var->ivalue = 0;
	var->left = sta;
	var->right = sta2;
	return var;
}
past newWhileStmt(past e, past sta)
{
	past var = newAstNode();
	var->nodeType = "while";
	var->ivalue = 0;
	var->left = e;
	var->right = sta;
	return var;
}
past newReturnStmt(past e)
{
	past var = newAstNode();
	var->nodeType = "jumpStmt";
	var->ivalue = 0;
	var->left = e;
	var->right = NULL;
	return var;
}
past newPrintStmt(past e)
{
	past var = newAstNode();
	var->nodeType = "printStmt";
	var->ivalue = 0;
	var->left = e;
	var->right = NULL;
	return var;
}
past newScanStmt(past il)
{
	past var = newAstNode();
	var->nodeType = "scanStmt";
	var->ivalue = 0;
	var->left = il;
	var->right = NULL;
	return var;
}
past newExpr_Stmt(past e)
{
	past var = newAstNode();
	var->nodeType = "exprStmt";
	var->ivalue = 0;
	var->left = e;
	var->right = NULL;
	return var;
}
past newParameter(past t, past id)
{
	past var = newAstNode();
	var->nodeType = "parameter";
	var->ivalue = 0;
	var->left = t;
	var->right = id;
	return var;
}
past newID()
{
	past var = newAstNode();
	var->nodeType = "varDecl";
	var->nodeData = initData();
	strcpy(var->nodeData, yytext);
	var->ivalue = 0;
	var->left = NULL;
	var->right = NULL;
	return var;
}
past newInitDecl(past id, past e)
{
	past var = newAstNode();
	var->nodeType = "varInitDecl";
	var->ivalue = 1;
	var->left = id;
	var->right = e;
	return var;
}
past newFuncDecl(past id, past p)
{
	past var = newAstNode();
	var->nodeType = "funcDecl";
	var->ivalue = 2;
	var->left = id;
	var->right = p;
	return var;
}
past newArrayDecl(past id, past e)
{
	past var = newAstNode();
	var->nodeType = "arrayDecl";
	var->ivalue = 0;
	var->left = id;
	var->right = e;
	return var;
}
past newArrayInitDecl(past id, past is)
{
	past var = newAstNode();
	var->nodeType = "arrayInitDecl";
	var->ivalue = 0;
	var->left = id;
	var->right = is;
	return var;
}
past newInit(past id)
{
	past var = newAstNode();
	var->nodeType = "Decl";
	var->ivalue = 0;
	var->left = id;
	var->right = NULL;
	return var;
}
past newInitializer(int oper, char* data)
{
	past var = newAstNode();
	var->ivalue = oper;
	if(oper == NUMBER)
		var->nodeType = "number";
	else
		var->nodeType = "STRING";
	var->nodeData = initData();
	strcpy(var->nodeData, data);
	var->ivalue = 0;
	var->left = NULL;
	var->right = NULL;
	return var;
}
past newPrim_Expr(past id, past el)
{
	past var = newAstNode();
	var->ivalue = 1;
	var->nodeType = "primary_expr";
	var->left = id;
	var->right = el;
	return var;
}
past newPrim_Expr2(past id, past e)
{
	past var = newAstNode();
	var->ivalue = 2;
	var->nodeType = "primary_expr";
	var->left = id;
	var->right = e;
	return var;
}
past newPrim_Expr3(past id, past e, past e2)
{
	past var = newAstNode();
	var->ivalue = 3;
	var->nodeType = "primary_expr";
	var->left = newArray(id, e);
	var->right = e2;
	return var;
}
past newArray(past id, past e)
{
	past var = newAstNode();
	var->nodeType = "array";
	var->ivalue = 0;
	var->left = id;
	var->right = e;
	return var;
}
past newPrim_Cmp_Expr(past e)
{
	past var = newAstNode();
	var->ivalue = 1;
	var->left = e;
	var->right = NULL;
	return var;
}
past newExpr(int oper, past left, past right)
{
	past var = newAstNode();
	var->ivalue = oper;
	var->nodeType = "expr";
	var->left = left;
	var->right = right;
	return var;
}




past astProgram()
{
	advance();
	past ext = astExternal();
	past list = newList(0, NULL, ext);
	int level = 0;
	while(tok == INT || tok == VOID || tok == STR)
	{
		level++;
		ext = astExternal();
		list = newList(level, list, ext);
	}

	return list;
}
past astExternal()
{
	int oper = tok;
	advance();
	past t = newType(oper);
	past d = astDeclarator();
	past ds = astDecl_Or_Stmt();
	
	if(strcmp(d->nodeType, "funcDecl") == 0)
	{
		t = newFuncDecl(t, d);
		t = newExtDecl("funcDecl", t, ds);
		return t;
	}
	else
	{
		t = newDecl(t, d);
		t = newExtDecl("decl", t, ds);
		return t;
	}
}
past astDecl_Or_Stmt()
{
	if(tok == '{')
	{
		advance();
		if(tok == '}')
		{
			advance();
			past s = newCompStmt(NULL);
			return s;
		}
		else
		{
			past s = astStatement_list();
			if(tok == '}')
			{
				advance();
				s = newCompStmt(s);
				return s;
			}
		}
	}
	else if(tok == ',')
	{
		advance();
		past dl = astDeclarator_list();
		if(tok == ';')
		{
			advance();
			dl = newDecl_list(dl);
			return dl;
		}
	}
	else if(tok == ';')
	{
		advance();
		return NULL;
	}
	else
	{
		printf("something wrong!");
		exit(0);
	}
}
past astDeclarator_list()
{
	past result = newList(0, NULL, astDeclarator());
	int level = 0;
	while(tok == ',')
	{
		advance();
		level++;
		result = newList(level, result, astDeclarator());
	}
	
	return result;
}
past astDeclarator()
{
	past id = newID();
	advance();
	if(tok == '=')
	{
		advance();
		past e = astExpr();
		id = newInitDecl(id, e);
		return id;
	}
	else if(tok == '(')
	{
		advance();
		if(tok == ')')
		{
			advance();
			id = newFuncDecl(id, NULL);
			return id;
		}
		else
		{
			past l = astParameter();
			if(tok == ')')
			{
				advance();
				id = newFuncDecl(id, l);
				return id;
			}
		}
	}
	else if(tok == '[')
	{
		advance();
		if(tok == ']')
		{
			advance();
			if(tok == '=')
			{
				advance();
				if(tok == '{')
				{
					advance();
					past i = astIntstr_list();
					if(tok == '}')
					{
						advance();
						id = newArrayDecl(id, NULL);
						id = newArrayInitDecl(id, i);
						return id;
					}
				}
			}
			else
			{
				id = newArrayDecl(id, NULL);
				return id;
			}
		}
		else
		{
			past e = astExpr();
			if(tok == ']')
			{
				advance();
				if(tok == '=')
				{
					advance();
					if(tok == '{')
					{
						advance();
						past i = astIntstr_list();
						if(tok == '}')
						{
							advance();
							id = newArrayDecl(id, e);
							id = newArrayInitDecl(id, i);
							return id;
						}
					}
				}
				else
				{
					id = newArrayDecl(id, e);
					return id;
				}
			}
		}
	}
	else
	{
		id = newInit(id);
		return id;
	}
}
past astIntstr_list()
{
	int oper = tok;
	past result = newList(0, NULL, newInitializer(oper, yytext));
	int level = 0;
	advance();
	while(tok == ',')
	{
		advance();
		oper = tok;
		level++;
		result = newList(level, result, newInitializer(oper, yytext));
		advance();
	}
	return result;
}

past astParameter_list()
{
	past result = newList(0, NULL, astParameter());
	int level = 0;
	while(tok == ',')
	{
		advance();
		level++;
		result = newList(level, result, astParameter());
	}

	return result;
}
past astParameter()
{
	int oper = tok;
	advance();
	past t = newType(oper);
	past i = newID();
	advance();
	t = newParameter(t, i);
	return t;
}
past astStatement_list()
{
	past sta = astStatement();
	past list = newList(0, NULL, sta);
	int level = 0;

	while(tok != '}')
	{
		sta = astStatement();
		level++;
		list = newList(level, list, sta);
	}
	return list;
}
past astStatement()
{
	if(tok == INT || tok == VOID || tok == STR)
	{
		past type = newType(tok);
		advance();
		past dl = astDeclarator_list();
		if(tok == ';')
		{
			advance();
			dl = newDecl(type, dl);
			return dl;
		}
	}
	else if(tok == '{')
	{
		advance();
		past sta = astStatement_list();
		if(tok == '}')
		{
			advance();
			sta = newCompStmt(sta);
			return sta;
		}
	}
	else if(tok == IF)
	{
		advance();
		if(tok == '(')
		{
			advance();
			past e = astExpr();
			if(tok == ')')
			{
				advance();
				past sta = astStatement();
				if(tok == ELSE)
				{
					advance();
					past sta_els = astStatement();
					sta = newIfStmt(e, sta, sta_els);
				}
				else
				{
					sta = newIfStmt(e, sta, NULL);
				}
				return sta;
			}
		}
	}
	else if(tok == WHILE)
	{
		advance();
		if(tok == '(')
		{
			advance();
			past e = astExpr();
			if(tok == ')')
			{
				advance();
				past sta = astStatement();
				sta = newWhileStmt(e, sta);
				return sta;
			}
		}
	}
	else if(tok == RETURN)
	{
		advance();
		if(tok == ';')
		{
			advance();
			past e = newReturnStmt(NULL);
			return e;
		}
		else
		{
			past e = astExpr();
			if(tok == ';')
			{
				advance();
				e = newReturnStmt(e);
				return e;
			}
		}
	}
	else if(tok == PRINT)
	{
		advance();
		if(tok == ';')
		{
			advance();
			past p = newPrintStmt(NULL);
			return p;
		}
		else
		{
			past p = astExpr_list();
			if(tok == ';')
			{
				advance();
				p = newReturnStmt(p);
				return p;
			}
		}
	}
	else if(tok == SCAN)
	{
		advance();
		past i = astId_list();
		if(tok == ';')
		{
			advance();
			i = newScanStmt(i);
			return i;
		}
	}
	else
	{
		past e = astExpr_Stmt();
		e = newExpr_Stmt(e);
		return e;
	}
}
past astExpr_Stmt()
{
	past result;
	if(tok == ';')
	{
		advance();
		return NULL;
	}
	else
	{
		past e = astExpr();
		if(tok == ';')
		{
			advance();
			result = newExpr_Stmt(e);
			return result;
		}
	}
}
past astExpr()
{
	past c = astCmp_Expr();
	c = newExpr(0, c, NULL);
	return c;
}
past astCmp_Expr()
{
	past l = astAdd_Expr();
	while(tok == LOWER || tok == BIGER || tok == LEQU || tok == BEQU || tok == EQU || tok == UEQU)
	{
		int oper = tok ;
		advance();
		past r = astAdd_Expr();
		l = newExpr(oper, l ,r);
	}
	return l;
}
past astAdd_Expr()
{
	past l = astMul_Expr();
	while(tok == '+' || tok == '-')
	{
		int oper = tok ;
		advance();
		past r = astMul_Expr();
		l = newExpr(oper, l, r);
	}
	return l;
}
past astMul_Expr()
{
	if(tok == '-')
	{
		advance();
		past l = astPrimary_Expr();
		while(tok == '*' || tok == '/' || tok == '%')
		{
			int oper = tok ;
			advance();
			past r = astPrimary_Expr();
			l = newExpr(oper, l, r);
		}
		l = newExpr('-', NULL,	l);
		return l;
	}
	else
	{
		past l = astPrimary_Expr();
		while(tok == '*' || tok == '/' || tok == '%')
		{
			int oper = tok ;
			advance();
			past r = astPrimary_Expr();
			l = newExpr(oper, l, r);
		}
		return l;
	}
}
past astPrimary_Expr()
{
	if(tok == ID)
	{
		past id = newID();
		advance();
		if(tok == '(')
		{
			advance();
			if(tok == ')')
			{
				advance();
				id = newPrim_Expr(id, NULL);
				return id;
			}
			else
			{
				past el = astExpr_list();
				if(tok == ')')
				{
					advance();
					id = newPrim_Expr(id, el);
					return id;
				}
			}
		}
		else if(tok == '=')
		{
			advance();
			past e = astExpr();
			id = newPrim_Expr2(id, e);
			return id;
		}
		else if(tok == '[')
		{
			advance();
			past e = astExpr();
			if(tok == ']')
			{
				advance();
				if(tok == '=')
				{
					advance();
					past e2 = astExpr();
					id = newPrim_Expr3(id, e, e2);
					return id;
				}
				else
				{
					id = newPrim_Expr3(id, e, NULL);
					return id;
				}
			}
		}
		else if(tok == ADD_ASSIGN || tok == DEC_ASSIGN || tok == MUL_ASSIGN || tok == DIV_ASSIGN || tok == MOD_ASSIGN)
		{
			int oper = tok ;
			advance();
			past e = astExpr();
			id = newExpr(oper, id, e);
			return id;
		}
		else
			return id;
	}
	else if( tok == '(')
	{
		advance();
		past e = astExpr();
		e = newPrim_Cmp_Expr(e);
		if(tok == ')')
		{
			advance();
			return e;
		}
	}
	else if(tok == NUMBER || tok == STRING)
	{
		past i = newInitializer(tok, yytext);
		advance();
		return i;
	}
	else return NULL;
}
past astExpr_list()
{
	past result = newList(0, NULL, astExpr());
	int level = 0;
	while(tok == ',')
	{
		advance();
		level++;
		result = newList(level, result, astExpr());
	}
	return result;
}
past astId_list()
{
	past result = newList(0, NULL, newID());
	int level = 0;
	advance();
	while(tok == ',')
	{
		advance();
		level++;
		result = newList(level, result, newID());
		advance();
	}
	return result;
}


			
			


void showAst(past node, int nest)
{
	if(node == NULL)
		return;


	if(strcmp(node->nodeType, "list") == 0)
	{
		if(node->left == NULL)
		{
			int i = 0;
			for(i = 0; i < nest; i ++)
				printf("  ");
			printf("%s           %d\n",node->nodeType, node->ivalue+1);
			showAst(node->right, nest+1);
		}
		else
		{
			node->left->ivalue = node->ivalue;
			showAst(node->left, nest);
			showAst(node->right, nest+1);
		}
	}
	else
	{
		int i = 0;
		for(i = 0; i < nest; i ++)
			printf("  ");
		if(node->nodeData != NULL)
			printf("%s    %s     %d\n",node->nodeType, node->nodeData, node->ivalue);
		else
			printf("%s           %d\n",node->nodeType, node->ivalue);
		showAst(node->left, nest+1);
		showAst(node->right, nest+1);
	}
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
//	program();
	
	past p = astProgram();
	printf("\n\nThe final Syntax Tree is:\n\n");
	showAst(p, 0);
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
