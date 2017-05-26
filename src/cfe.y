%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_bloc_var.h"
#include "list_fonction.h"
#include "pile_identificateur.h"
#include "generateur_string.h"

void yyerror(const char *s);
extern char *yytext;
extern int yylineno;		//numero de ligne
extern FILE *yyin;		//fichier en entrée
int bloc_level = 0;		//level du bloc qu'on traite
int nb_bloc = 0;		//nombre de bloc total
list_bloc *var_symb;		//list de bloc 
list_fonction *fonc_symb;	//list de fonction
list_var *list_param;		//list de parametre
int returned_type = 0;  	//type retourné par une fonction 0= void, 1 = int
char *type_courant;		//type courant util lors de déclaration de variable en liste de declaration 
pile_identificateur *tag_courant;		//le tag utilisé pour gérer les break
char *id_tag_courant;
pile_identificateur *identificateurs_non_resoluts;	//pile d'identificateur pour gérer les appels récursifs
pile_identificateur *condition_courante_switch;		//pile d'identificateur courant pour gérer les imbrication de switch (voir test switch.c)
pile_identificateur *tag_switch_next_cond;
char *var_tmp_a_declarer;	//liste de déclaration de variable temp
char *var_tmp_affectation;	//liste d'affectation de variable tmp
int flag;			//flag pour savoir si on est dans un switch
FILE *yyout;			//fichier de sortie

%}
%token IDENTIFICATEUR CONSTANTE VOID INT FOR WHILE IF ELSE SWITCH CASE DEFAULT
%token BREAK RETURN PLUS MOINS MUL DIV LSHIFT RSHIFT BAND BOR LAND LOR LT GT 
%token GEQ LEQ EQ NEQ NOT EXTERN
%union	{
	char*      pStr;

	struct lexp{	char *pstr;
			int nb;
			char *var_tmp;
	}lexp;

        struct code_var_tmp{     char *pstr;
				 char *var_tmp;
	}code_var_tmp;
}
%type <pStr> IDENTIFICATEUR type CONSTANTE liste_declarations liste_fonctions declaration liste_declarateurs declarateur fonction liste_parms parm liste_instructions instruction iteration selection saut affectation bloc appel binary_rel binary_comp programme
%type <lexp> liste_expressions
%type <code_var_tmp> expression condition variable
%left PLUS MOINS BOR BAND LSHIFT RSHIFT
%left MUL DIV
%left LAND LOR
%nonassoc THEN
%nonassoc ELSE
%left REL
%start programme
%%
programme	:	
		liste_declarations liste_fonctions {$$ = append($1,$2);fprintf(yyout,"%s",$$);}
;
liste_declarations	:	
		liste_declarations declaration {$$ = append($1,$2);}
	|	{$$ = "";}
;
liste_fonctions	:	
		liste_fonctions fonction {$$ = append($1,$2);}
|               fonction {$$ = $1;}
;
declaration	:	
		type liste_declarateurs ';' {$$ = append($1," "); $$ = append($$,$2);$$ = append($$,";\n");}
;
liste_declarateurs	:	
		liste_declarateurs ',' declarateur {$$ = append($1,","); $$ = append($$,gen_declaration_tab($3));
					if(ajouter_var(var_symb,bloc_level,getNomVarSuf($3,' '),type_courant) != 0 || (is_param(list_param,getNomVarSuf($3,' ')) == 0 && bloc_level == 1)){
						yyerror("ERROR: variable déjà déclarée");
					}
    					else{
						/*fprintf(stdout,"variable ajouté: TYPE= %s , NOM= %s\n",type_courant,$1);*/
						//fprintf(stdout,"var= %s.\n",getNomVarSuf($3,' '));
					}
		}
	|	declarateur {/*fprintf(stdout,"deb2: %s\n",$$);*/$$ = gen_declaration_tab($1);
					if(ajouter_var(var_symb,bloc_level,getNomVarSuf($1,' '),type_courant) != 0 || (is_param(list_param,getNomVarSuf($1,' ')) == 0 && bloc_level == 1)){
						yyerror("ERROR: variable déjà déclarée");
					}
    					else{
						/*fprintf(stdout,"variable ajouté: TYPE= %s , NOM= %s\n",type_courant,$1);*/
						//fprintf(stdout,"var= %s.\n",getNomVarSuf($1,' '));
					}}
;
declarateur	:	
		IDENTIFICATEUR {
					depiler(identificateurs_non_resoluts);

					$$ = $1;
				}
	|	declarateur '[' CONSTANTE ']' {$$ = append($1,"[");$$ = append($$,$3);$$ = append($$,"]");}
;
fonction	:	
		type IDENTIFICATEUR '(' liste_parms ')' '{' liste_declarations liste_instructions '}' {
					nb_bloc++;
					int res = add_fonction(fonc_symb,$1,$2,list_param,returned_type);
					depiler(identificateurs_non_resoluts);
					if(res == 1){
						yyerror("ERROR: fonction déjà déclarée");
					}
					else if(res == 2){
						fprintf(stderr,"warning: le type retourné est différent du type déclaré à la ligne %d.\n",yylineno);
					}
					else{
						//fprintf(stdout,"DEBUG===> fonction ajouté: TYPE= %s , NOM= %s\n",$1,$2);
						reset_list_var(list_param);
						returned_type = 0;
					}
					$$ = append($1," "); $$ = append($$,$2);$$ = append($$,"(");$$ = append($$,$4);$$ = append($$,")");$$ = append($$,"{\n");$$ = append($$,$7);$$ = append($$,var_tmp_a_declarer);$$ = append($$,$8);$$ = append($$,"}\n");var_tmp_a_declarer = "";
}
	|	EXTERN type IDENTIFICATEUR '(' liste_parms ')' ';' {
					int tmp = 0;
					if(strcmp($2,"int") == 0) {tmp = 1;}
					int res = add_fonction(fonc_symb,$2,$3,list_param,tmp);
					depiler(identificateurs_non_resoluts);
					if(res == 1){
						yyerror("ERROR: fonction déjà déclarée");
					}
					else if(res == 2){
						yyerror("ERROR: le type retourné est différent du type déclaré");
					} 
					else{
						//fprintf(stdout,"DEBUG===> fonction ajouté: TYPE= %s , NOM= %s\n",$2,$3);
						reset_list_var(list_param);
						returned_type = 0;
					}
					$$ = append("extern ",$2);$$ = append($$," ");$$ = append($$,$3);$$ = append($$,"(");$$ = append($$,$5);$$ = append($$,");\n");
}
;
type	:	
		VOID {
			$$ = "void";
			type_courant = "void";
		}
	|	INT {
			$$ = "int";
			type_courant = "int";
		}
;
liste_parms	:	
		liste_parms ',' parm {$$ = append($1,",");$$ = append($$,$3);}
	|	parm {$$ = $1;}
	|       {$$ = "";}
;
parm	:	
		INT IDENTIFICATEUR {	depiler(identificateurs_non_resoluts);
					type_courant = "int";
					//fprintf(stdout,"ttt\n");
					if(add_var_list_var(list_param, type_courant, $2) == 1){
						yyerror("ERROR: identificateur en double");
					}
					else{
						/*fprintf(stdout,"paramètre ajouté: TYPE= %s , NOM= %s\n",type_courant,$2);*/
					}
					$$ = append("int ",$2);
}
	|	VOID		{$$ = "void";}
;
liste_instructions :	
		liste_instructions instruction {$$ = append($1,$2);}
	|       {$$ = "";}
;
instruction	:	
		iteration	{$$ = $1;}
	|	selection	{$$ = $1;}
	|	saut		{$$ = $1;}
	|	affectation ';' {$$ = append($1,";\n");}
	|	bloc		{$$ = $1;}
	|	appel		{$$ = $1;}
;
iteration	:	
		FOR '(' affectation ';' condition ';' affectation ')' instruction {$$ = gen_for($3,$5.pstr,$7,$9);$$ = append($5.var_tmp,$$);var_tmp_affectation = "";}
	|	WHILE '(' condition ')' instruction {$$ = gen_while($3.pstr,$5);$$ = append($3.var_tmp,$$);var_tmp_affectation = "";}
;
selection	:	
		IF '(' condition ')' instruction %prec THEN {$$ = gen_if($3.pstr,$5,NULL);$$ = append($3.var_tmp,$$);var_tmp_affectation = "";}
	|	IF '(' condition ')' instruction ELSE instruction {$$ = gen_if($3.pstr,$5,$7);$$ = append($3.var_tmp,$$);var_tmp_affectation = "";}
	|	SWITCH '(' expression ')' instruction {$$ = gen_switch($5);depiler(tag_switch_next_cond);$$ = append($3.var_tmp,$$);var_tmp_affectation = "";}
	|	CASE CONSTANTE ':' instruction {$$ = gen_case($2,$4);}
	|	DEFAULT ':' instruction {$$ = gen_default($3);}
;
saut	:	
		BREAK ';' {if(bloc_level <= 1){yyerror("ERROR: breack ne peut pas etre utilisé comme sa");}$$ = gen_break();}
	|	RETURN ';'{$$ = "return;\n";}
	|	RETURN expression ';' {returned_type = 1;$$ = append("return ",$2.pstr);$$ = append($$,";\n");$$ = append($2.var_tmp,$$);var_tmp_affectation = "";}
;
affectation	:	
		variable '=' expression {
					//fprintf(stdout,"var= %s\n",$1);
					//depiler(identificateurs_non_resoluts);
					if(is_declared(var_symb,bloc_level,$1.pstr) == 1){
						if(is_param(list_param,$1.pstr) == 1){
							//fprintf(stdout,"DEBUG====>b%d var non déclaré %s\n",bloc_level,$1);
							yyerror("ERROR: variable non déclarée");
						}					
					}
					/*debug("affectation", "affectation de variable");*/
					$$ = gen_var_tab($1.pstr,get_var(var_symb,bloc_level,$1.pstr));
					$$ = append($$,"=");$$ = append($$,$3.pstr);$$ = append($1.var_tmp,$$);$$ = append(var_tmp_affectation,$$);var_tmp_affectation = "";$$ = append($3.var_tmp,$$);
					}
;
bloc	:	
		'{' liste_declarations liste_instructions '}' {nb_bloc++;/* fprintf(stdout,"bloc level: %d\n",bloc_level);*/
							$$ = append("\n{\n",$2);$$ = append($$,$3);$$ = append($$,"}\n");}
;
appel	:	
		IDENTIFICATEUR '(' liste_expressions ')' ';' {depiler(identificateurs_non_resoluts);
								int res = check_type(fonc_symb,$1,$3.nb);
								int appel_rec = is_in_pile(identificateurs_non_resoluts,$1);
								if(appel_rec == 0){
									if(taille_list_var(list_param) != $3.nb){
										res = 3;
									}
								}
								if(res == 2 && appel_rec == 1){
									yyerror("ERROR: fonction non déclarée");
								}
								else if(res == 3){
									//fprintf(stdout,"nb param2 %d\n",$3);
									yyerror("ERROR: nombre de paramètre incorrect");
								}
								$$ = append($1,"(");$$ = append($$,$3.pstr);$$ = append($$,");\n");
								$$ = append(var_tmp_affectation,$$);
								var_tmp_affectation = "";
								$$ = append($3.var_tmp,$$);
								//fprintf(stdout,"deb: %s\n",var_tmp_affectation);
							}
;
variable	:	
		IDENTIFICATEUR	{$$.pstr = $1;depiler(identificateurs_non_resoluts);$$.var_tmp = "";}
	|	variable '[' expression ']' {$$.pstr = append($1.pstr,"[");$$.pstr = append($$.pstr,$3.pstr);$$.pstr = append($$.pstr,"]");$$.var_tmp = append($1.var_tmp,$3.var_tmp);}
;
expression	:	
		'(' expression ')' {$$.pstr = $2.pstr;if(flag == 1){add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;}$$.var_tmp = $2.var_tmp;}
	|	expression PLUS expression {$$.pstr = append($1.pstr,"+");/*%prec OP*/
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = gen_code_3_adresse($1.pstr,"+",$3.pstr);
							if(flag == 1){
								add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;
							}
							$$.var_tmp = append($1.var_tmp,$3.var_tmp);
							$$.var_tmp = append($$.var_tmp,var_tmp_affectation);
							//fprintf(stdout,"deb: %s\n",$$.var_tmp);
							var_tmp_affectation = "";
							}
	|	expression MOINS expression{$$.pstr = append($1.pstr,"-");/*%prec OP*/
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = gen_code_3_adresse($1.pstr,"-",$3.pstr);
							if(flag == 1){
								add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;
							}
							$$.var_tmp = append($1.var_tmp,$3.var_tmp);
							$$.var_tmp = append($$.var_tmp,var_tmp_affectation);
							//fprintf(stdout,"deb: %s\n",$$.var_tmp);
							var_tmp_affectation = "";
							}
	|	expression MUL expression{$$.pstr = append($1.pstr,"*");/*%prec OP*/
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = gen_code_3_adresse($1.pstr,"*",$3.pstr);
							if(flag == 1){
								add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;
							}
							$$.var_tmp = append($1.var_tmp,$3.var_tmp);
							$$.var_tmp = append($$.var_tmp,var_tmp_affectation);
							//fprintf(stdout,"deb: %s\n",$$.var_tmp);
							var_tmp_affectation = "";
							}
	|	expression DIV expression{$$.pstr = append($1.pstr,"/");/*%prec OP*/
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = gen_code_3_adresse($1.pstr,"/",$3.pstr);
							if(flag == 1){
								add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;
							}
							$$.var_tmp = append($1.var_tmp,$3.var_tmp);
							$$.var_tmp = append($$.var_tmp,var_tmp_affectation);
							//fprintf(stdout,"deb: %s\n",$$.var_tmp);
							var_tmp_affectation = "";
							}
	|	expression LSHIFT expression{$$.pstr = append($1.pstr,"<<");/*%prec OP*/
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = gen_code_3_adresse($1.pstr,"<<",$3.pstr);
							if(flag == 1){
								add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;
							}
							$$.var_tmp = append($1.var_tmp,$3.var_tmp);
							$$.var_tmp = append($$.var_tmp,var_tmp_affectation);
							//fprintf(stdout,"deb: %s\n",$$.var_tmp);
							var_tmp_affectation = "";
							}
	|	expression RSHIFT expression{$$.pstr = append($1.pstr,">>");/*%prec OP*/
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = gen_code_3_adresse($1.pstr,">>",$3.pstr);
							if(flag == 1){
								add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;
							}
							$$.var_tmp = append($1.var_tmp,$3.var_tmp);
							$$.var_tmp = append($$.var_tmp,var_tmp_affectation);
							//fprintf(stdout,"deb: %s\n",$$.var_tmp);
							var_tmp_affectation = "";
							}
	|	expression BAND expression{$$.pstr = append($1.pstr,"&");/*%prec OP*/
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = gen_code_3_adresse($1.pstr,"&",$3.pstr);
							if(flag == 1){
								add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;
							}
							$$.var_tmp = append($1.var_tmp,$3.var_tmp);
							$$.var_tmp = append($$.var_tmp,var_tmp_affectation);
							//fprintf(stdout,"deb: %s\n",$$.var_tmp);
							var_tmp_affectation = "";
							}
	|	expression BOR expression{$$.pstr = append($1.pstr,"|");/*%prec OP*/
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = gen_code_3_adresse($1.pstr,"|",$3.pstr);
							if(flag == 1){
								add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;
							}
							$$.var_tmp = append($1.var_tmp,$3.var_tmp);
							$$.var_tmp = append($$.var_tmp,var_tmp_affectation);
							//fprintf(stdout,"deb: %s\n",$$.var_tmp);
							var_tmp_affectation = "";
							}
	|	MOINS expression{$$.pstr = append("-",$2.pstr);if(flag == 1){add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;}$$.var_tmp = $2.var_tmp;}
	|	CONSTANTE	{$$.pstr = $1;if(flag == 1){add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;}$$.var_tmp = "";}
	|	variable	{	//fprintf(stdout,"var= %s\n",$1);
					if(is_declared(var_symb,bloc_level,$1.pstr) == 1){
						if(is_param(list_param,$1.pstr) == 1){
							//fprintf(stdout,"DEBUG====>b%d var non déclaré %s\n",bloc_level,$1);
							yyerror("ERROR: variable non déclarée");
						}					
					}
					$$.pstr = gen_var_tab($1.pstr,get_var(var_symb,bloc_level,$1.pstr));
					$$.var_tmp = $1.var_tmp;
					$$.var_tmp = append($$.var_tmp,var_tmp_affectation);var_tmp_affectation = "";
					//fprintf(stdout,"deb: %s\n",$$.var_tmp);
					//$$ = $1;
					if(flag == 1){add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;}
				}
	|	IDENTIFICATEUR '(' liste_expressions ')'{	depiler(identificateurs_non_resoluts);
								int res = check_type(fonc_symb,$1,$3.nb);
								int appel_rec = is_in_pile(identificateurs_non_resoluts,$1);
								if(appel_rec == 0){
									if(taille_list_var(list_param) != $3.nb){
										res = 3;
									}
								}
								if(res == 1){
									fprintf(stderr,"warning: affectation avec une fonction de type void à la ligne %d.\n",yylineno);
								}
								else if(res == 2 && appel_rec == 1){
									//fprintf(stdout,"nb param %d\n",$3);
									yyerror("ERROR: fonction non déclarée");
								}
								else if(res == 3 ){
									//fprintf(stdout,"nb param2 %d\n",$3);
									yyerror("ERROR: nombre de paramètre incorrect");
								}
							$$.pstr = append($1,"(");
							$$.pstr = append($$.pstr,$3.pstr);
							$$.pstr = append($$.pstr,")");
							if(flag == 1){add_identificateur(condition_courante_switch,append($$.pstr,""));flag = 0;}
							$$.var_tmp = $3.var_tmp;					
}
;
liste_expressions	:	
		liste_expressions ',' expression {$$.nb = $1.nb + 1;$$.pstr = append($1.pstr,",");$$.pstr = append($$.pstr,$3.pstr);$$.var_tmp = append($1.var_tmp,$3.var_tmp);}
	|	expression {$$.nb = 1;$$.pstr = $1.pstr;$$.var_tmp = $1.var_tmp;}
	|	{$$.nb = 0;$$.pstr = "";$$.var_tmp = "";}
;
condition	:	
		NOT '(' condition ')' {$$.pstr = append("!(",$3.pstr);$$.pstr = append($$.pstr,")");$$.var_tmp = $3.var_tmp;}
	|	condition binary_rel condition %prec REL{$$.pstr = append($1.pstr,$2);$$.pstr = append($$.pstr,$3.pstr); $$.var_tmp = append($1.var_tmp,$3.var_tmp);}
	|	'(' condition ')'{$$.pstr = append("(",$2.pstr);$$.pstr = append($$.pstr,")");$$.var_tmp = $2.var_tmp;}
	|	expression binary_comp expression{ $$.pstr = append($1.pstr,$2); $$.pstr = append($$.pstr,$3.pstr);$$.var_tmp = append($1.var_tmp,$3.var_tmp);}
;
binary_rel	:	
		LAND {$$ = "&";}
	|	LOR {$$ = "|";}
;
binary_comp	:	
		'<' {$$ = "<";}
	|	'>' {$$ = ">";}
	|	GEQ {$$ = ">=";}
	|	LEQ {$$ = "<=";}
	|	EQ {$$ = "==";}
	|	NEQ {$$ = "!=";}
;
%%

void yyerror(const char *s)
{
  fflush(stdout);
  fprintf(stderr, "*** %s à la ligne %d\n",s,yylineno);
  exit(1);
}

void init_compilateur(){

  /*++++++++++++++++++++++++++INIT++++++++++++++++++++++++++*/
  var_symb = new_list_bloc();
  list_param = new_list_var();
  fonc_symb = new_list_fonction();
  identificateurs_non_resoluts = new_pile_identificateur();
  condition_courante_switch = new_pile_identificateur();
  tag_switch_next_cond = new_pile_identificateur();
  var_tmp_a_declarer = "";
  var_tmp_affectation = "";
  flag = 0;
  tag_courant = new_pile_identificateur();
  id_tag_courant = NULL;
}

int main (int argc, char *argv[]){

  char *nom_yyout;	//nom du fichier de sortie
  init_compilateur();	//init du compilateur

  if (argc == 1){
    yyparse();
  }
  else if (argc == 2) {
    nom_yyout = malloc(sizeof(char)*strlen(argv[1])+1);
    strncpy(nom_yyout,argv[1],strlen(argv[1])-2);//génération du nom du fichier de sortie
    nom_yyout = append(nom_yyout,"_3d.c");
    yyin = fopen(argv[1], "r");
    if(yyin == NULL){

      fprintf(stderr,"%s: Aucun fichier ou dossier de ce type.\n",argv[1]);
      return 1;
    }
    yyout = fopen(nom_yyout,"w");
    yyparse();
    fclose(yyout);
  }

  else if (argc == 3) {

     yyout = fopen(argv[2],"w");
     yyin = fopen(argv[1], "r");
     if(yyin == NULL){

       fprintf(stderr,"%s: Aucun fichier ou dossier de ce type.\n",argv[1]);
       return 1;
     }
     yyparse();
     fclose(yyout);
  }
  return 0;
}

