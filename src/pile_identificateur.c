#include <stdio.h>
#include <stdlib.h>
#include "pile_identificateur.h"

pile_identificateur *new_pile_identificateur(){

    pile_identificateur *pile = malloc(sizeof(pile_identificateur));
    pile->first = NULL;
    return pile;
}

noeud_identificateur *new_noeud_identificateur(){

    noeud_identificateur *noeud = malloc(sizeof(noeud_identificateur));
    noeud->id = NULL;
    noeud->suivant = NULL;
    return noeud;
}

//empile un noeud en tête de pile
void empiler(pile_identificateur *pile,noeud_identificateur *noeud){

    //fprintf(stdout,"empiler %s\n",noeud->id);
    if(pile->first == NULL){pile->first = noeud; return;}
    noeud->suivant = pile->first;
    pile->first = noeud;
}

void ajouter_en_fin_id(pile_identificateur *pile,char *nom){

    noeud_identificateur *noeud = new_noeud_identificateur();
    noeud_identificateur *noeud_courant = pile->first;
    noeud->id = nom;
    if(pile->first == NULL){pile->first = noeud; return;}
    while(noeud_courant->suivant != NULL){

        noeud_courant = noeud_courant->suivant;
    }
    noeud_courant->suivant = noeud;
}

//ajoute un identificateur dans la pile
void add_identificateur(pile_identificateur *pile, char *id){

    noeud_identificateur *noeud = new_noeud_identificateur();
    noeud->id = id;
    empiler(pile,noeud);
}

//dépile la tête de pile
void depiler(pile_identificateur *pile){

    //fprintf(stdout,"debug\n");
    noeud_identificateur *tmp;
    if(pile->first == NULL){fprintf(stderr,"warning: depiler pile_ident vide\n");return;}
    if(pile->first->suivant == NULL){

        //fprintf(stdout,"depiler %s\n",pile->first->id);
        free(pile->first);
        pile->first = NULL;
    }
    else{
        tmp = pile->first;
        pile->first = pile->first->suivant;
        //fprintf(stdout,"depiler %s\n",tmp->id);
        free(tmp);
    }
}

//renvoie 0 si l'id est dans la pile et 1 sinon
int is_in_pile(pile_identificateur *pile, char *id){

    noeud_identificateur *noeud_courant = pile->first;
    if(noeud_courant == NULL){return 1;}
    while(noeud_courant->suivant != NULL)
    {
        if(strcmp(noeud_courant->id, id) == 0)
        {

            return 0;
        }
        noeud_courant = noeud_courant->suivant;
    }
    if(strcmp(noeud_courant->id, id) == 0)
    {

        return 0;
    }
    return 1;
}
