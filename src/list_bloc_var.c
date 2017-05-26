#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_bloc_var.h"


//renvoie la taille d'une list_var
int taille_list_var(list_var *liste){

    int res = 0;
    if(liste->first == NULL){return res;}
    noeu_var *noeu_courant = liste->first;
    while(noeu_courant != NULL)
    {
        res++;
        noeu_courant = noeu_courant->suivant;
    }
    return res;
}

//ajoute une variable a la liste de type list_var
//renvoie 0 si tout ce passe bien et 1 si la variable existe déjà dans la liste
int add_var_list_var(list_var *liste,char *type, char *nom){

    noeu_var *noeu = malloc(sizeof(noeu_var));
    noeu->nom = nom;
    noeu->type = type;
    noeu->suivant = NULL;

    if(liste->first == NULL){liste->first = noeu; return 0;}
    noeu_var *noeu_courant = liste->first;
    while(noeu_courant->suivant != NULL)
    {
        if(strcmp(noeu_courant->nom, nom) == 0)
        {

            return 1;
        }
        noeu_courant = noeu_courant->suivant;
    }
    if(strcmp(noeu_courant->nom, nom) == 0)
    {

        return 1;
    }
    noeu_courant->suivant = noeu;
    return 0;
}

//ajoute un noeud à la liste var
void ajouter_noeu_var(list_var *liste,noeu_var *noeu)
{

    if(liste->first == NULL){liste->first = noeu;return;}
    noeu_var *noeu_courant = liste->first;
    while(noeu_courant->suivant != NULL)
    {

        noeu_courant = noeu_courant->suivant;
    }
    noeu_courant->suivant = noeu;

}

//vide la liste
void reset_list_var(list_var *liste)
{
    free(liste);
    liste = new_list_var();
}

//ajoute un noeud de type bloc à la liste de type list_bloc
void ajouter_noeu_bloc(list_bloc *liste)
{
    noeu_bloc *noeu_courant = liste->first;
    while(noeu_courant->suivant != NULL)
    {

        noeu_courant = noeu_courant->suivant;
    }
    noeu_courant->suivant = new_noeu_bloc();

}

//ajoute une var dans le bloc num bloc_level
//revoie 0 si tout va bien et 1 si elle est déjà déclarée
int ajouter_var(list_bloc *list_bloc,int bloc_level, char *nom, char *type)
{

    int i;
    noeu_bloc *noeu_courant_bloc = list_bloc->first;
    noeu_var *noeu_courant_var;
    noeu_var *var = new_noeu_var();
    var->nom = nom;
    var->type = type;
    var->suivant = NULL;
    //on cherche le bloc dans lequel on va ajouter la variable
    for(i = 0; i < bloc_level; i++)
    {
        noeu_courant_bloc = noeu_courant_bloc->suivant;
    }
    //puis on ajoute la variable
    if(noeu_courant_bloc->list_var_bloc->first == NULL){

            noeu_courant_bloc->list_var_bloc->first = var;
            return 0;
    }
    noeu_courant_var = noeu_courant_bloc->list_var_bloc->first;
    while(noeu_courant_var->suivant != NULL)
    {
        /*fprintf(stdout,"debug\n");*/
        if(strcmp(noeu_courant_var->nom, nom) == 0)
        {

            return 1;
        }
        noeu_courant_var = noeu_courant_var->suivant;
    }
    if(strcmp(noeu_courant_var->nom, nom) == 0)
    {

        return 1;
    }
    noeu_courant_var->suivant = var;
    return 0;
}

//verifie si une variable est déjà declarée
//retourne 0 si elle existe et 1 sinon
int is_declared(list_bloc *list_bloc,int bloc_level, char *nom){

    int i;
    noeu_bloc *noeu_courant_bloc = list_bloc->first;
    noeu_var *noeu_courant_var;
    //on recherche dans tout les blocs
    for(i = 0; i <= bloc_level; i++)
    {

        if(noeu_courant_bloc->list_var_bloc->first != NULL){

            noeu_courant_var = noeu_courant_bloc->list_var_bloc->first;
            while(noeu_courant_var->suivant != NULL)
            {
                /*fprintf(stdout,"debug\n");*/
                if(strcmp(getNomVar(nom,'['), getNomVar(noeu_courant_var->nom,'[')) == 0)
                {

                    return 0;
                }
                noeu_courant_var = noeu_courant_var->suivant;
            }
            if(strcmp(getNomVar(nom,'['), getNomVar(noeu_courant_var->nom,'[')) == 0)
            {

                return 0;
            }
        }
        noeu_courant_bloc = noeu_courant_bloc->suivant;
    }
    return 1;
}

//retourne la var déclarée, si c'est un tableau renvoie nom_tab[n][m]...[z]
char *get_var(list_bloc *list_bloc,int bloc_level, char *nom){

    int i;
    noeu_bloc *noeu_courant_bloc = list_bloc->first;
    noeu_var *noeu_courant_var;
    char *res = NULL;
    //on recher dans tout les blocs
    for(i = 0; i <= bloc_level; i++)
    {

        if(noeu_courant_bloc->list_var_bloc->first != NULL){

            noeu_courant_var = noeu_courant_bloc->list_var_bloc->first;
            while(noeu_courant_var->suivant != NULL)
            {
                /*fprintf(stdout,"debug\n");*/
                if(strcmp(getNomVar(nom,'['), getNomVar(noeu_courant_var->nom,'[')) == 0)
                {

                    res = noeu_courant_var->nom;
                }
                noeu_courant_var = noeu_courant_var->suivant;
            }
            if(strcmp(getNomVar(nom,'['), getNomVar(noeu_courant_var->nom,'[')) == 0)
            {

                res = noeu_courant_var->nom;
            }
        }
        noeu_courant_bloc = noeu_courant_bloc->suivant;
    }
    return res;
}

//supprime le dernier bloc de la liste
void remove_list_bloc(list_bloc *list)
{
    noeu_bloc *noeu_courant = list->first;
    if(noeu_courant->suivant == NULL){return;}
    while(noeu_courant->suivant->suivant != NULL){

       noeu_courant = noeu_courant->suivant;
    }
    /*fprintf(stdout,"free bloc var\n");*/
    free(noeu_courant->suivant->suivant); /* libération de memoire incomplète! */
    noeu_courant->suivant = NULL;
}

//renvoie 0 si nom est deja dans la liste et 1 sinon
int is_param(list_var *liste,char *nom){

    if(liste->first == NULL){return 1;}
    noeu_var *noeu_courant = liste->first;
    while(noeu_courant->suivant != NULL)
    {
        if(strcmp(noeu_courant->nom, nom) == 0)
        {

            return 0;
        }
        noeu_courant = noeu_courant->suivant;
    }
    if(strcmp(noeu_courant->nom, nom) == 0)
    {

        return 0;
    }
    return 1;
}

//renvoie l'id d'un tableau (ex: tab[20] --> tab)
char *getNomVar(char *nom,char symb){

    int taille = strlen(nom)+1;
    char *res = malloc(sizeof(char)*taille);
    int i;
    for(i = 0;i< taille;i++){

        if(nom[i] == symb){

            res[i] = '\0';//ajout du symbole de fin de string
            //fprintf(stdout,"var = %s\n",res);
            break;
        }
        res[i] = nom[i];
    }
    return res;
}

//renvoie le pointeur de la string après le symbole symb si symb n'existe pas renvoie nom
char *getNomVarSuf(char *nom,char symb){

    int taille = strlen(nom)+1;
    char *res = malloc(sizeof(char)*taille);
    int i,flag;
    flag = 0;
    for(i = 0;i< taille;i++){

        if(nom[i] == symb){

            flag = 1;
            //fprintf(stdout,"var = %s\n",res);
        }
        else if(flag == 1){

            res[i] = nom[i];
        }
    }
    if(flag == 0){

        return nom;
    }
    return res;
}

noeu_var *new_noeu_var()
{

    noeu_var *res = malloc(sizeof (noeu_var));
    res->nom = NULL;
    res->type = NULL;
    res->suivant = NULL;
    return res;
}

list_var *new_list_var(){

    list_var *res = malloc(sizeof (list_var));
    res->first = NULL;
    return res;
}

noeu_bloc *new_noeu_bloc()
{

    noeu_bloc *res = malloc(sizeof (noeu_bloc));
    res->list_var_bloc = new_list_var();
    res->suivant = NULL;
    return res;
}

list_bloc *new_list_bloc(){

    list_bloc *res = malloc(sizeof (list_bloc));
    res->first = new_noeu_bloc();
    return res;
}

//test
/*int main(){

    int bloc_level = 0;
    list_bloc *l1 = new_list_bloc();
    char *nom = "toto";
    char *type = "int";
    char *nom2 = "toto";
    char *type2 = "int";
    char *nom3 = "titi";
    char *type3 = "int";
    if(ajouter_var(l1,bloc_level,nom,type) != 0){
        fprintf(stdout,"la var existe deja\n");
    }
    else{
        fprintf(stdout,"variable ajouté\n");
    }

    if(ajouter_var(l1,bloc_level,nom2,type2) != 0){
        fprintf(stdout,"la var existe deja\n");
    }
    else{
        fprintf(stdout,"variable ajouté\n");
    }
    ajouter_noeu_bloc(l1);
    bloc_level++;
    if(ajouter_var(l1,bloc_level,nom2,type2) != 0){
        fprintf(stdout,"la var existe deja\n");
    }
    else{
        fprintf(stdout,"variable ajouté\n");
    }
    if(ajouter_var(l1,bloc_level,nom3,type3) != 0){
        fprintf(stdout,"la var existe deja\n");
    }
    else{
        fprintf(stdout,"variable ajouté\n");
    }

    return 0;
}*/
