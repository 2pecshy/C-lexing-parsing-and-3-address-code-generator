extern int printd( int i);

int fact(int n) {
  if ( n <= 1 )
    return 1;
  return n*fact(n-1);
}

int main(int i) {
  printd(fact(10));
  return 0;
}
