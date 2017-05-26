all: projet

cfe.tab.c cfe.tab.h: cfe.y
	bison -d cfe.y

lex.yy.c: ANSI-C.lex cfe.tab.h
	flex ANSI-C.lex

list_bloc_var.o: list_bloc_var.h list_bloc_var.c
	gcc -o list_bloc_var.o -c list_bloc_var.c

list_fonction.o: list_bloc_var.h list_fonction.h list_fonction.c
	gcc -o list_fonction.o -c list_fonction.c

pile_identificateur.o: pile_identificateur.h pile_identificateur.c
	gcc -o pile_identificateur.o -c pile_identificateur.c

generateur_string.o: generateur_string.h pile_identificateur.h pile_int.h generateur_string.c
	gcc -o generateur_string.o -c generateur_string.c

pile_int.o: pile_int.h pile_int.c
	gcc -o pile_int.o -c pile_int.c
	
cfe.tab.o: cfe.tab.c cfe.tab.h list_bloc_var.h
	gcc -o cfe.tab.o -c cfe.tab.c -lfl

lex.yy.o: lex.yy.c cfe.tab.h list_bloc_var.h
	gcc -o lex.yy.o -c lex.yy.c -lfl

projet: list_bloc_var.o list_fonction.o pile_identificateur.o generateur_string.o pile_int.o cfe.tab.o lex.yy.o
	gcc -o projet list_bloc_var.o cfe.tab.o lex.yy.o list_fonction.o pile_identificateur.o generateur_string.o pile_int.o -lfl
	rm -vf cfe.tab.c lex.yy.c cfe.tab.h *.o

clean:
	rm projet cfe.tab.c lex.yy.c cfe.tab.h *.o Tests/*_3d.c Tests/build/* -rfv

test: all
	./runTest
