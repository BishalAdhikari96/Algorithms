/*
Name: Bishal Adhikari

->Command:<-
  gcc mersesort.c //It compiles the code
 ./a.out < lab1.dat // lab1.dat is a prototype, any data file could be used
                         to run the code as long as it follows the given format

->objective of the program<-
The following program takes two data tables in ascending order and then the
ranks required to be searched. Then, it applies binary search algorithm to
find the element of the rank of the table if it was to sorted merged.
*/

#include <stdio.h>
#include <stdlib.h>

//no prototype for the function since it has been declared in the begining.
// binary_search_machine has the main binary search algorithm of the code.
int binary_search_machine(int* a,int* b, int rank,int m, int n)
{
  int i,j;
  int high =0, low =0;
  // the following condition gets the low and high value
  if(rank<=m){
    high = rank;
  }
  else {
    low = rank - n;
    high = m;
  }
  //loop to execute binary searched
  while(1){
    i = (low+high)/2;
    j = rank - i;
    printf("low:%d high: %d i: %d j: %d",low,high,i,j);
    printf("\n");

    if (a[i]<=b[j]) {
      if (b[j]<a[i+1]) {
        printf("b[%d] = %d has rank %d\n\n", j , b[j], rank);
        return b[j];//this returns the value b[j] thus terminates the search
      }else{
        low = i+1;
      }
    }else if(b[j]<a[i]){
      if (a[i]<=b[j+1]) {
        printf("a[%d] = %d has rank %d\n\n", i , a[i] , rank);
        return a[i];//this returns the value a[i] thus terminates the search
      }else{
        high = i-1;
      }
    }
  }
  return 0;//return statement to address the value return type of the function
}
//main function
int main(int argc, char const *argv[]) {

  int m,n,p;
  //takes input from the file redirected in linux
  scanf("%d %d %d",&m,&n,&p);
  //dynamic memory allocatoin for the variables
  int*a= malloc((m+2)*sizeof(int));
  int*b=malloc((n+2)*sizeof(int));
  int*rank=malloc(p*sizeof(int));
  //sentinel value declaration
  a[0]=-99999999;
  a[m+1]=99999999;
  b[0]=-99999999;
  b[n+1]=99999999;
  //loop to input the first table
  int i;
  for ( i = 1; i < m+1; i++) {
    scanf("%d",&a[i]);
  }
  //loop to input the first table
  int j;
  for ( j = 1; j < n+1; j++) {
    scanf("%d",&b[j]);
  }
  //loop to input the first table
  int k;
  for( k =0;k<p;k++){
    scanf("%d",&rank[k]);
  }
  // this loop is used to rally through the values of rank
  for ( i = 0; i < p; i++) {
    binary_search_machine(a,b,rank[i],m,n);
  }
  //since the dynamically allocated variables don't get free by itself, using
  //free to free them.
  free(a);
  free(b);
  free(rank);
  return 0; // return for int valuetype of main
}
