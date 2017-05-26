typedef struct noeu_fonction noeu_fonction;
typedef struct list_fonction list_fonction;

struct noeu_fonction{
	char *nom;
	char *type;
    int nb_param;
	noeu_fonction *suivant;
};

struct list_fonction{
	noeu_fonction *first;
};

int add_fonction(list_fonction *liste, char *type, char *nom, list_var *list_param, int type_de_retour);
list_fonction *new_list_fonction();
int check_type(list_fonction *liste,char *nom,int nb_param);


