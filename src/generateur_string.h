#include "pile_int.h"
char *append(char *s1, char *s2);
char *gen_tag();
char *gen_nom_var_tmp();
char *gen_pointeur(char *nom);
char *gen_for(char *affectation1,char *condition, char *affectation2, char *instruction2);
char *gen_while(char *condition,char *instruction);
char *gen_if(char *condition,char *instruction_then,char *instruction_else);
char *gen_break();
char *gen_switch(char *instruction);
char *gen_case(char *constante,char *instruction);
char *gen_default(char *instruction);
char *gen_var_tmp(char *expression1,char *binary_op,char *expression2);
pile_int *get_arg_tab(char *nom);
char *gen_var_tab(char *nom,char *nom_declarateur);
char *gen_declaration_tab(char *nom);
char *gen_code_3_adresse(char *expression1,char *binary_op,char *expression2);
