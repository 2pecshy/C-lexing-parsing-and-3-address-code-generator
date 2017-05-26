typedef struct noeu_var noeu_var;
typedef struct list_var list_var;
typedef struct noeu_bloc noeu_bloc;
typedef struct list_bloc list_bloc;

struct noeu_var{
	char *nom;
	char *type;
	noeu_var *suivant;
};

struct list_var{
	noeu_var *first;
};

struct noeu_bloc{
	list_var *list_var_bloc;
	noeu_bloc *suivant;
};

struct list_bloc{
	struct noeu_bloc *first;
};

int taille_list_var(list_var *liste);
int add_var_list_var(list_var *liste,char *type, char *nom);
void ajouter_noeu_var(list_var *liste,noeu_var *noeu);
void reset_list_var(list_var *liste);
void ajouter_noeu_bloc(list_bloc *liste);
int ajouter_var(list_bloc *list_bloc,int bloc_level, char *nom, char *type);
int is_declared(list_bloc *list_bloc,int bloc_level, char *nom);
char *get_var(list_bloc *list_bloc,int bloc_level, char *nom);
void remove_list_bloc(list_bloc *list);
int is_param(list_var *liste,char *nom);
char *getNomVar(char *nom,char symb);
noeu_var *new_noeu_var();
list_var *new_list_var();
noeu_bloc *new_noeu_bloc();
list_bloc *new_list_bloc();
char *getNomVarSuf(char *nom,char symb);


