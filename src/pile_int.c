#include <stdio.h>
#include <stdlib.h>
#include "pile_int.h"

pile_int *new_pile_int(){

    pile_int *pile = malloc(sizeof(pile_int));
    pile->first = NULL;
    return pile;
}

noeud_int *new_noeud_int(){

    noeud_int *noeud = malloc(sizeof(noeud_int));
    noeud->val = 0;
    noeud->suivant = NULL;
    return noeud;
}

void empiler_int(pile_int *pile,int val){

    noeud_int *noeud = new_noeud_int();
    noeud->val = val;
    if(pile->first == NULL){pile->first = noeud; return;}
    noeud->suivant = pile->first;
    pile->first = noeud;
}

void ajouter_en_fin_int(pile_int *pile,int val){

    noeud_int *noeud = new_noeud_int();
    noeud_int *noeud_courant = pile->first;
    noeud->val = val;
    if(pile->first == NULL){pile->first = noeud; return;}
    while(noeud_courant->suivant != NULL){

        noeud_courant = noeud_courant->suivant;
    }
    noeud_courant->suivant = noeud;
}

void depiler_int(pile_int *pile){

    //fprintf(stdout,"debug\n");
    noeud_int *tmp;
    if(pile->first == NULL){fprintf(stderr,"warning: depiler pile_int vide\n");return;}
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

int mul_of_pile_int(pile_int *pile){

    int res = 1;
    noeud_int *noeud_courant = pile->first;
    if(pile->first == NULL){return res;}
    noeud_courant = noeud_courant->suivant;
    while(noeud_courant != NULL){

        res = res * noeud_courant->val;
        noeud_courant = noeud_courant->suivant;
        //fprintf(stdout,"mul_of_pile_int: %d\n",res);
    }
    return res;
}

int mul_pile_int(pile_int *pile){

    int res = 1;
    noeud_int *noeud_courant = pile->first;
    while(noeud_courant != NULL){

        res = res * noeud_courant->val;
        noeud_courant = noeud_courant->suivant;
    }
    return res;
}
