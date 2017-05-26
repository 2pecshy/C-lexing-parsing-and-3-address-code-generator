%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
#include "cfe.tab.h"
#include "list_bloc_var.h"
#include "list_fonction.h"
#include "pile_identificateur.h"
#include "generateur_string.h"
void comment();
extern int bloc_level;
extern int nb_bloc;
extern list_bloc *var_symb;
extern char *type_courant;
extern pile_identificateur *identificateurs_non_resoluts;
extern pile_identificateur *tag_courant;
extern char *id_tag_courant;
extern pile_identificateur *tag_switch_next_cond;
extern int flag;



%}

D                       		[0-9]
L                       		[a-zA-Z_]
H                       		[a-fA-F0-9]
E                       		[Ee][+-]?{D}+
FS                      		(f|F|l|L)
IS                      		(u|U|l|L)*
WS  					[ \t\v\n\f]

%%

"/*"                                    { comment(); }
"//".*                                  {}
"break"					{ return(BREAK); }
"case"					{ return(CASE); }
"default"				{ return(DEFAULT); }
"else"					{ return(ELSE); }
"extern"				{ return(EXTERN); }
"for"					{ return(FOR); }
"if"					{ return(IF); }
"int"					{ return(INT); }
"return"				{ return(RETURN); }
"switch"				{ flag = 1;add_identificateur(tag_switch_next_cond,NULL); return(SWITCH); }
"void"					{ return(VOID); }
"while"					{ return(WHILE); }

{L}({L}|{D})*				{ yylval.pStr = malloc(strlen(yytext)+1);
					char *id = malloc(strlen(yytext)+1);
					strcpy(id,yytext);
					strcpy(yylval.pStr,yytext);
					add_identificateur(identificateurs_non_resoluts,id);
					return IDENTIFICATEUR;}

0[xX]{H}+{IS}?				{ yylval.pStr = malloc(strlen(yytext)+1);
					strcpy(yylval.pStr,yytext);
					return(CONSTANTE); }
0{D}+{IS}?				{ yylval.pStr = malloc(strlen(yytext)+1);
					strcpy(yylval.pStr,yytext);
					return(CONSTANTE); }
{D}+{IS}?				{ yylval.pStr = malloc(strlen(yytext)+1);
					strcpy(yylval.pStr,yytext);
					return(CONSTANTE); }

{D}+{E}{FS}?				{ yylval.pStr = malloc(strlen(yytext)+1);
					strcpy(yylval.pStr,yytext);
					return(CONSTANTE); }
{D}*"."{D}+({E})?{FS}?			{ yylval.pStr = malloc(strlen(yytext)+1);
					strcpy(yylval.pStr,yytext);
					return(CONSTANTE); }
{D}+"."{D}*({E})?{FS}?			{ yylval.pStr = malloc(strlen(yytext)+1);
					strcpy(yylval.pStr,yytext);
					return(CONSTANTE); }

"+"					{ return PLUS;}
"-"					{ return MOINS;}
"*"					{ return MUL;}
"/"					{ return DIV;}

";"					{ return ';'; }
"{"					{ bloc_level++;
					 ajouter_noeu_bloc(var_symb);
					add_identificateur(tag_courant,NULL);
					id_tag_courant = NULL;
					return '{'; }
"}"					{ bloc_level--;
					remove_list_bloc(var_symb);
					 return '}';}
","					{ return ','; }
":"					{ return ':'; }
"="					{ return '='; }
"("					{ return '('; }
")"					{ return ')'; }
"["					{ return '['; }
"]"					{ return ']'; }
"."					{ return '.'; }
"&"					{ return LAND; }
"!"					{ return NOT; }
"~"					{ return '~'; }
"%"					{ return '%'; }
"<"					{ return '<'; }
">"					{ return '>'; }
"<<"					{ return LSHIFT; }
">>"					{ return RSHIFT; }
"=="					{ return EQ; }
"!="					{ return NEQ; }
"<="					{ return LEQ; }
">="					{ return GEQ; }
"^"					{ return '^'; }
"|"					{ return LOR; }
"?"					{ return '?'; }

{WS}+					{}
.                       		{ return yytext[0]; }
%%
int yywrap()        /* called at end of input */
{
    return 1;           /* terminate now */
}

void comment()
{
    int c;

    while ((c = input()) != 0)
        if (c == '*')
        {
            while ((c = input()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}
