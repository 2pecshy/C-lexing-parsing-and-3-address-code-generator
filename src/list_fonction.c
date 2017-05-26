#include <stdio.h>
#include <stdlib.h>
#include "list_bloc_var.h"
#include "list_fonction.h"


//ajoute une fonction à la liste des fonctions
//renvoie 0 si tout va bien, 1 si elle est déjà declarée, et 2 si le type retourné n'est pas comme celui déclaré
int add_fonction(list_fonction *liste, char *type, char *nom, list_var *list_param, int type_de_retour){

    int type_fonc = 0;
    int res = 0;
    if(strcmp(type,"int") == 0){type_fonc = 1;}
    /*fprintf(stdout,"type = %d, %d\n",type_fonc,type_de_retour);*/
    if(type_fonc != type_de_retour){

        res = 2;
    }
    noeu_fonction *noeu = malloc(sizeof(noeu_fonction));
    noeu->nom = nom;
    noeu->type = type;
    noeu->nb_param = taille_list_var(list_param);
    noeu->suivant = NULL;

    if(liste->first == NULL){

        liste->first = noeu;
        return res;
    }

    noeu_fonction *noeu_courant = liste->first;
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
    return res;

}

list_fonction *new_list_fonction(){

    list_fonction *res = malloc(sizeof (list_fonction));
    res->first = NULL;
    return res;
}

//verifie le nombre de paramètre donné lors d'un appel ansi que le type de retour
//revoie 3 si le nombre de paramètre est incorrecte 1 si le type est void, et 2 si la fonction n'existe pas
int check_type(list_fonction *liste,char *nom,int nb_param){

    noeu_fonction *noeu_courant = liste->first;
    if(noeu_courant == NULL){return 2;}
    while(noeu_courant->suivant != NULL)
    {
        if(strcmp(noeu_courant->nom, nom) == 0)
        {
            if(noeu_courant->nb_param != nb_param){
                return 3;
            }
            if(strcmp(noeu_courant->type, "int") == 0){
                return 0;
            }
            return 1;
        }
        noeu_courant = noeu_courant->suivant;
    }
    if(strcmp(noeu_courant->nom, nom) == 0)
    {
        if(noeu_courant->nb_param != nb_param){
            return 3;
        }
        if(strcmp(noeu_courant->type, "int") == 0){
            return 0;
        }
        return 1;
    }
    return 2;
}
