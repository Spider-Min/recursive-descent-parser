%{
enum yytokentype {
	WHILE=258,FOR,IF,ELSE,PRINT,SCAN,RETURN,INT,ID,STR,VOID,LOWER,BIGER,LEQU,BEQU,EQU,UEQU,ASSIGN,AND,OR,STRING,EOL,NUMBER,ADD_ASSIGN,SUB_ASSIGN,MUL_ASSIGN,DIV_ASSIGN,MOD_ASSIGN,ERROR
};

int yylval;

%}
DIGIT [0-9]
ID [A-Za-z][a-zA-Z_0-9]*
CHAR [a-zA-Z]
STRING .*?  

%%


"+" |
"-" |
"*" |
"/" |
"(" |
")" |
"[" |
"]" | 
"{" |
"}" |
"," |
";" |
"=" |
"q" { return yytext[0]; }

"<" { return LOWER; }
">" { return BIGER; }
"<=" { return LEQU; }
">=" { return BEQU; }
"==" { return EQU; }
"!=" { return UEQU; }

"+=" |
"-=" |
"*=" |
"/=" |
"%=" { return ASSIGN; }


[0-9]+ { yylval = atoi(yytext); return NUMBER; }
"int" { yylval = atoi(yytext); return INT; }
"str" { yylval = atoi(yytext); return STR; }
"void" { yylval = atoi(yytext); return VOID; }
"return" { yylval = atoi(yytext); return RETURN; }
"while" { yylval = atoi(yytext); return WHILE; }
"print" { yylval = atoi(yytext); return PRINT; }
"scan" { yylval = atoi(yytext); return SCAN; }
"if" { yylval = atoi(yytext); return IF; }
"else" { yylval = atoi(yytext); return ELSE; }

{ID} { return ID; }
\"[^"]*\" { yylval = atoi(yytext); return STRING; }

\n	{ return EOL;}

"//"{STRING}*\n |
[ \t] { /* ignore whitespace */ }
. 	{ printf("Mystery character %c\n", *yytext); }


%%


int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}
// {DIGIT}*"."{DIGIT}+ { yylval = atoi(yytext); return FLOAT; }

int yywrap()
{
	return 1;
}
