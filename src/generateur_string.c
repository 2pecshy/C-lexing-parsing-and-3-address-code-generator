#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generateur_string.h"
#include "pile_identificateur.h"
#include "list_bloc_var.h"
#include "pile_int.h"

extern pile_identificateur *tag_courant;
extern char *id_tag_courant;
extern pile_identificateur *condition_courante_switch;
extern pile_identificateur *tag_switch_next_cond;
extern char *var_tmp_a_declarer;
extern char *var_tmp_affectation;
int num_tag = 0;
int num_var_tmp = 0;
//renvoie un pointeur de la concatenation de s1 et s2
char *append(char *s1, char *s2){

    int new_taille = strlen(s1)+strlen(s2)+1;
    char *res = malloc(sizeof(char)*new_taille);
    strcpy(res,s1);
    strcat(res,s2);
    return res;
}

//génère les tags utilisés pour les gotos
char *gen_tag(){

    char *res;
    char buffer[1000];
    sprintf(buffer,"%d",num_tag);
    num_tag++;
    res = malloc(sizeof(char)*strlen(buffer)+1);
    strcpy(res,buffer);
    res = append("L",res);
    return res;
}

char *gen_nom_var_tmp(){

    char *res;
    char buffer[1000];
    sprintf(buffer,"%d",num_var_tmp);
    num_var_tmp++;
    res = malloc(sizeof(char)*strlen(buffer)+1);
    strcpy(res,buffer);
    res = append("_T",res);
    return res;
}

char *gen_pointeur(char *nom){

    char *res = append("*",nom);
    return res;
}

//renvoie un pointeur de la géneration de la boucle for
char *gen_for(char *affectation1,char *condition, char *affectation2, char *instruction){

    char *tag_top = gen_tag();
    char *tag_bottom = gen_tag();
    char *res = malloc(sizeof(char)*2);
    char *instruction2 = malloc(sizeof(char)*strlen(instruction)+1);
    strncpy(instruction2,instruction,strlen(instruction)-1);
    instruction2 = append(instruction2,"\n");
    instruction2 = append(instruction2,affectation2);
    instruction2 = append(instruction2,";\n");
    if(instruction2[0] == '{'){instruction2 = append(instruction2,"}");}
    res = "";
    res = append(res,affectation1);
    res = append(res,";\n");
    res = append(res,tag_top);
    res = append(res,": ;\n");
    res = append(res," if (!(");
    res = append(res,condition);
    res = append(res,"))");
    res = append(res,"goto ");
    res = append(res,tag_bottom);
    res = append(res,";\n");
    res = append(res, instruction2);
    res = append(res,"goto ");
    res = append(res,tag_top);
    res = append(res,";\n");
    res = append(res,tag_bottom);
    res = append(res,": ;\n");
    if(id_tag_courant != NULL){
        res = append(res,id_tag_courant);
        res = append(res,": ;\n");
        if(tag_courant->first != NULL){
            id_tag_courant = tag_courant->first->id;
        }
        else{ id_tag_courant = NULL;}
    }
    return res;
}

//revoie un pointeur de la géneration de la boucle while
char *gen_while(char *condition,char *instruction){

    char *tag_top = gen_tag();
    char *tag_bottom = gen_tag();
    char *res = malloc(sizeof(char)*2);
    res = "";
    res = append(res,"goto ");
    res = append(res,tag_top);
    res = append(res,";\n");
    res = append(res, tag_bottom);
    res = append(res,": ;\n");
    res = append(res,instruction);
    res = append(res,tag_top);
    res = append(res,": ;\n");
    res = append(res,"if (");
    res = append(res, condition);
    res = append(res, ") ");
    res = append(res, "goto ");
    res = append(res, tag_bottom);
    res = append(res,";\n");
    if(id_tag_courant != NULL){
        res = append(res,id_tag_courant);
        res = append(res,": ;\n");
        if(tag_courant->first != NULL){
            id_tag_courant = tag_courant->first->id;
        }
        else{ id_tag_courant = NULL;}
    }
    return res;
}

//renvoie un pointeur de la géneration du if et else si else != NULL
char *gen_if(char *condition,char *instruction_then,char *instruction_else){

    char *tag_continuer = gen_tag();
    char *tag_else = gen_tag();
    char *res = malloc(sizeof(char)*2);
    res = "";
    res = append(res,"if (!(");
    res = append(res, condition);
    res = append(res, ")) ");
    res = append(res, "goto ");
    if(instruction_else == NULL){
        res = append(res, tag_continuer);
    }
    else{
        res = append(res, tag_else);
    }
    res = append(res,";\n");
    res = append(res, instruction_then);
    if(instruction_else != NULL){
        res = append(res, "goto ");
        res = append(res, tag_continuer);
        res = append(res,";\n");
        res = append(res,tag_else);
        res = append(res,": ;\n");
        res = append(res, instruction_else);
    }
    res = append(res,tag_continuer);
    res = append(res,": ;\n");
    return res;
}

//renvoie un pointeur de la géneration du break
//génère un goto vers la fin du bloc d'un while, for ou switch
char *gen_break(){

    char *res = malloc(sizeof(char)*2);
    res = "";
    if(id_tag_courant == NULL){

        tag_courant->first->id = gen_tag();
        id_tag_courant = tag_courant->first->id;
    }
    res = append(res,"goto ");
    res = append(res,id_tag_courant);
    res = append(res,";\n");
    return res;
}

//renvoie un pointeur de la géneration du switch
char *gen_switch(char *instruction){

    char *res = malloc(sizeof(char)*2);
    res = "";
    depiler(condition_courante_switch);
    res = append(res,instruction);
    res = append(res,id_tag_courant);

    res = append(res,": ;\n");
    if(tag_courant->first != NULL){
        //gestion des imbrications de bloc while for et switch
        depiler(tag_courant);
        if(tag_courant->first != NULL){
            id_tag_courant = tag_courant->first->id;
            //fprintf(stdout,"%s\n",id_tag_courant);
        }
    }
    else{ id_tag_courant = NULL;}
    return res;
}

//renvoie un pointeur de la géneration du case
char *gen_case(char *constante,char *instruction){

    char *res = malloc(sizeof(char)*2);
    char *cond_case;
    char *tag_cond = gen_tag();
    cond_case = append(condition_courante_switch->first->id, "!=" );
    cond_case = append(cond_case,constante);
    res = "";
    if(tag_switch_next_cond->first->id != NULL){
        res = append(res, "goto ");
        res = append(res, tag_cond);
        res = append(res,";\n");
        res = append(res,tag_switch_next_cond->first->id);
        res = append(res,": ;\n");
    }
    tag_switch_next_cond->first->id = gen_tag();
    res = append(res,"if (");
    res = append(res, cond_case);
    res = append(res, ") ");
    res = append(res, "goto ");
    res = append(res, tag_switch_next_cond->first->id);
    res = append(res,";\n");
    res = append(res, tag_cond);
    res = append(res,": ;\n");
    res = append(res, instruction);
    return res;
}

//renvoie un pointeur de la géneration du default
char *gen_default(char *instruction){
    char *res;
    res = "";
    if(tag_switch_next_cond->first->id != NULL){
        res = append(res,tag_switch_next_cond->first->id);
        res = append(res,": ;\n");
    }
    res = append(res,instruction);
    return res;

}

//renvoie un pointeur de la géneration d'une variable tmp
//ajoute à la liste de déclaration le nom de la var tmp à déclarer
char *gen_var_tmp(char *expression1,char *binary_op,char *expression2){

    char *res;
    char *declaration = "int ";
    char *nom_var = gen_nom_var_tmp();
    declaration = append(declaration,nom_var);
    declaration = append(declaration,";\n");
    var_tmp_a_declarer = append(var_tmp_a_declarer,declaration);
    //fprintf(stdout,"tmp var= %s\n",var_tmp_a_declarer);
    //fprintf(stdout,"tmp var1= %s\n",declaration);
    res = append(nom_var,"=");
    res = append(res,expression1);
    res = append(res,binary_op);
    res = append(res,expression2);
    res = append(res,";\n");
    return res;
}

//renvoie un pointeur de la géneration d'un pointeur tmp
//ajoute à la liste de déclaration le nom du pointeur tmp à déclarer
char *gen_pointeur_tab_tmp(char *expression1,char *expression2){

    char *res;
    char *declaration = "int ";
    char *nom_var = gen_nom_var_tmp();
    declaration = append(declaration,gen_pointeur(nom_var));
    declaration = append(declaration,";\n");
    var_tmp_a_declarer = append(var_tmp_a_declarer,declaration);
    //fprintf(stdout,"tmp pointeur= %s\n",var_tmp_a_declarer);
    res = append(nom_var,"=");
    res = append(res,expression1);
    res = append(res,"+");
    res = append(res,expression2);
    res = append(res,";\n");
    return res;
}

//renvoie la liste des dimentions d'une adresse d'un tableau (ex: tab[x][2][z] -> (list char) ("x","2","z") )
pile_identificateur *get_arg_tab2(char *nom){

    int taille = strlen(nom)+1;
    pile_identificateur *res = new_pile_identificateur();
    int i,ib;
    char buffer[100];
    char *value;
    int flag;
    flag = 0;
    ib = 0;
    for(i = 0;i< taille;i++){

        if(nom[i] == '['){

            flag = 1;
            ib = 0;

        }
        else if(nom[i] == ']'){

            flag = 0;
            buffer[ib] = '\0';
            value = malloc(sizeof(char)*strlen(buffer+1));
            strcpy(value,buffer);
            ajouter_en_fin_id(res,value);

        }
        else if (flag == 1){
            buffer[ib] = nom[i];
            ib++;
        }
    }
    return res;
}

//renvoie la liste des dimentions d'une déclaration d'un tableau (ex: tab[2][3][4] -> (list int) (2,3,4) )
pile_int *get_arg_tab(char *nom){

    int taille = strlen(nom)+1;
    pile_int *res = new_pile_int();
    int i,ib;
    char buffer[100];
    int value;
    int flag;
    flag = 0;
    ib = 0;
    for(i = 0;i< taille;i++){

        if(nom[i] == '['){

            flag = 1;
            ib = 0;

        }
        else if(nom[i] == ']'){

            flag = 0;
            buffer[ib] = '\0';
            value = atoi(buffer);
            ajouter_en_fin_int(res,value);

        }
        else if (flag == 1){
            buffer[ib] = nom[i];
            ib++;
        }
    }
    return res;
}

//renvoie un pointeur vers le nom du pointeur tmp de l'adresse d'un tableau [x][y][z] (ex: tab[x][y][z] -> *_tmp )
char *gen_var_tab(char *nom,char *nom_declarateur){

    char *res;
    int coef;
    pile_identificateur *arg_nom = new_pile_identificateur();
    pile_int *arg_nom_declarateur = new_pile_int();
    pile_identificateur *var_tmp = new_pile_identificateur();
    char *var_a_affecter;
    char *tmp;
    char buffer[100];
    if(strcmp(getNomVar(nom,'['),nom) == 0){

        return nom;
    }
    arg_nom = get_arg_tab2(nom);
    //fprintf(stdout,"nom decla: %s\n",nom_declarateur);
    arg_nom_declarateur = get_arg_tab(nom_declarateur);
    while(arg_nom->first->suivant != NULL){

        coef = mul_of_pile_int(arg_nom_declarateur);
        depiler_int(arg_nom_declarateur);
        sprintf(buffer,"%d",coef);
        tmp = gen_var_tmp(arg_nom->first->id,"*",buffer);
        //fprintf(stdout,"gen_var_tab: %s, id: %s\n",tmp,getNomVar(tmp,'='));
        add_identificateur(var_tmp,getNomVar(tmp,'='));
        var_tmp_affectation = append(var_tmp_affectation,tmp);
        depiler(arg_nom);
    }
    if(var_tmp->first == NULL){

        tmp = gen_pointeur_tab_tmp(getNomVar(nom,'['),arg_nom->first->id);
        var_tmp_affectation = append(var_tmp_affectation,tmp);
        res = getNomVar(tmp,'=');
        res = append("*",res);
        return res;
    }
    var_a_affecter = arg_nom->first->id;
    while(var_tmp->first != NULL){

        tmp = gen_var_tmp(var_tmp->first->id,"+",var_a_affecter);
        depiler(var_tmp);
        var_a_affecter = getNomVar(tmp,'=');
        var_tmp_affectation = append(var_tmp_affectation,tmp);
    }
    tmp = gen_pointeur_tab_tmp(getNomVar(nom,'['),var_a_affecter);
    var_tmp_affectation = append(var_tmp_affectation,tmp);
    res = getNomVar(tmp,'=');
    res = append("*",res);
    return res;

}

//renvoie le pointeur de la déclaration d'un tableau (ex: int tab[4][3] -> int tab [12] )
char *gen_declaration_tab(char *nom){

    char *res;
    //fprintf(stdout,"deb decla tab: %s. | nom: %s.\n",getNomVar(nom,'['),nom);
    if(strcmp(getNomVar(nom,'['),nom) == 0){

        return nom;
    }
    char buffer[100];
    int taille;
    pile_int *arg_nom = get_arg_tab(nom);
    taille = mul_pile_int(arg_nom);
    sprintf(buffer,"%d",taille);
    res = append(getNomVar(nom,'['),"[");
    res = append(res,buffer);
    //fprintf(stdout,"deb: %s\n",buffer);
    res = append(res,"]");
    return res;
}

//renvoie le pointeur du nom de la variable final d'une expression à n adresses vers 3 adresses
char *gen_code_3_adresse(char *expression1,char *binary_op,char *expression2){

    char *res;
    res = gen_var_tmp(expression1,binary_op,expression2);
    var_tmp_affectation = append(var_tmp_affectation,res);
    res = getNomVar(res,'=');
    return res;
}

/*
int main(){

    char aff1[] = "i = 0;";
    char cond[] = "i < 100";
    char aff2[] = "i = i + 1;";
    char inst[] = "{\n x = x + i;}";
    char inst2[] = "{\n y = y + 1;}";
    char *res = gen_for(aff1,cond,aff2,inst);
    res = append(res,gen_while(cond,inst));
    res = append(res,gen_if(cond,inst,inst2));
    res = append(res,gen_if(cond,inst,NULL));
    fprintf(stdout,"%s",res);
    return 0;
}*/
