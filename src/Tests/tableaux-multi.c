/* TEST TABLEAUX MULTIDIMENSIONNELS MINIC */
int tab[3][4][5];
extern int printd(int i);
int main() {
  tab[0][1][2] = 3;
  tab[1][2][3] = tab[0][1][2];
  printd(tab[1][2][3]);
  return 0;
}
