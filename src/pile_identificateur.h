typedef struct pile_identificateur pile_identificateur;
typedef struct noeud_identificateur noeud_identificateur;

struct pile_identificateur{
	 noeud_identificateur *first;
};

struct noeud_identificateur{
	char *id;
	noeud_identificateur *suivant;
};

pile_identificateur *new_pile_identificateur();
noeud_identificateur *new_noeud_identificateur();
void empiler(pile_identificateur *pile,noeud_identificateur *noeud);
void add_identificateur(pile_identificateur *pile, char *id);
void depiler(pile_identificateur *pile);
int is_in_pile(pile_identificateur *pile, char *id);
void ajouter_en_fin_id(pile_identificateur *pile,char *nom);
