int t[100];

extern int printd( int i );

int calcul(int x, int y) {
  return (x*y);
}

int main() {
  int i;
  for (i=0; i<100; i=i+1) {
    int j;
    j=i+1;
    t[i]= calcul(i,j);
  }
  printd(t[i-1]);
  //return 0;
}
