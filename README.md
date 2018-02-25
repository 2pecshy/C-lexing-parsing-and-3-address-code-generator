# code-generation-3-address-code-and-more

Projet compilation

compilateur cfe->cbe:

fonctionnalitées:

	-parseur cfe (etat: test ok)
	-detècte les commentaires, ignore tout ce qui ce trouve après "//" sur la même ligne et tout ce qui se touve entre "/*" et "*/" (etat: test ok)
	-verifie qu'une variable n'est pas déclarée deux fois dans le meme bloc (etat: test ok)
	-verifie qu'une fonction n'est pas déclarée plusieurs fois. (etat: test ok)
	-verifie qu'il n'y a pas deux fois le même identificateur en paramètre d'une fonction déclarée. (etat: test ok)
	-verifie le type de retour d'une fonction, une fonction int ne peut pas retourner void et inversement.(etat: test ok)
	-verifie que la fonction est déjà déclarée lors d'un appel de fonction. (etat: test ok)	(problème rencontré: la gestion des appels récursifs)
	-verifie lors de l'afectation d'une variable, que celle-ci est déjà déclarée, soit dans le meme bloc soit dans un bloc inférieur (etat: test ok)
	-verifie que les variables utilisées dans les expressions sont déjà déclarées. (etat: test ok)
	-verifie lors d'un appel de fonction que son nombre de paramètre est correct. (etat: test ok)

	-génération de code:

		-boucle for (etat: test ok)
		-boucle while(etat: test ok)
		-generation du if(etat: test ok)
		-generation du switch case(etat: test ok)
		-generation du break (etat: test ok)
		-génération code 3 adresses (etat: test ok)
		-génération tableau multidimension (etat: test ok)
		
	
================================compiler et lancer les tests================================

compiler le projet:

	lancer la commande: make

compiler le projet et lancer les tests:

	lancer la commande: make test

lancer les tests généré entièrement par gcc :

	lancer la commande: ./runSource

================================génération de code================================

génération du for:

tag_debut: ;
	if(not(condition)) 
		goto tag_fin;
		{instruction;
		afféctation de la condition;}
	goto tag_debut;
tag_fin;
suite prgm....

---------------------------------------------------
génération du while:

goto L1
L2 : < corps_de_la_boucle >
L1 : if ( < test > ) goto L2

---------------------------------------------------
génération du if
if (! < test > ) goto L
< instruction(s) > /* du then */
L : < suite_du_programme >

---------------------------------------------------
génération du swhich case:
//ex:

int i = 2;
shitch(i){

	case 1: break;
	case 2: i = i + 1;
	case 3: i = -3;
	default: i = i;
}
//gen:

if(!(i == 1)) goto L1;	//case 1:
	goto L2;	//break
	goto L3;
L1: ;
if(!(i == 2)) goto L5;  //case 2;
L3: ;
	i = i + 1;
	goto L4;
L5: ;
if(!(i == 3)) goto L6;  //case 3;
L4: ;
	i = -3;
	goto L7;
L6: ;			//default
L7: ;
	i = i;
L2: ;
suite prgm...

---------------------------------------------------
génération du break:

-le break est transformé en un goto qui pointe hors d'une boucle for, while ou d'un switch case.

---------------------------------------------------
génération du code 3 adresses:

-les déclarations des variables tmp sont déclarées en local.
-pas d'optimisation sur le nombre de variable tmp générée (réutilisation de variable).

---------------------------------------------------
génération des tableau:

calcule de la taille d'un tableau multidimensionnels:

	taille de tab[a][b]; = a*b 
	
	ex:
	int tab[4][5]; -> int tab[20];

calcule de l'adresse mémoire:

	int tab[3][2][2];

	adresse de tab[i][j][k] = i*2*2 + j*2 + k
