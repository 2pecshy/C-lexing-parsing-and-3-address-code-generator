#ifndef PILE_INT_H
#define PILE_INT_H
typedef struct pile_int pile_int;
typedef struct noeud_int noeud_int;

struct pile_int{
	 noeud_int *first;
};

struct noeud_int{
	int val;
	noeud_int *suivant;
};

pile_int *new_pile_int();
noeud_int *new_noeud_int();
void empiler_int(pile_int *pile,int val);
void depiler_int(pile_int *pile);
void ajouter_en_fin_int(pile_int *pile,int val);
int mul_of_pile_int(pile_int *pile);
int mul_pile_int(pile_int *pile);
#endif
