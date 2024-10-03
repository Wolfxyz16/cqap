#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "qap.h"
#include "bruteforce.h"
#include "localsearch.h"

int main(int argc, char *argv[])
{ 
  // el optimo de n=10 es 135028
  // leemos el problema qap
  Qap *qap = NULL;
  read_qap(&qap);
  int num_locales;

  // reservamos memoria para la solucion
  // int *sol = (int*) malloc(sizeof(int) * qap.n);
  // int *sol1, *sol2;

  // sol = best_first(&qap, sol_random);
  // sol = bruteforce(&qap);
  // sol1 = constructive(&qap);
  // sol2 = random_sol(qap.n);
  //
  // sol1 = best_first(&qap, sol2);
  //
  // print_sol(sol2, qap.n);
  
  int m = INT_MAX / 2;
  printf("Calculando schnabel con m=%d.\n", m);
  num_locales = schnabel_census_prodecure(m, qap);

  printf("maximos locales en %d = %d\n", m, num_locales);

  // printf("random = %d\n", objective_function(sol2, &qap));
  // printf("max_local = %d\n", objective_function(sol1, &qap));

  free_qap(qap);

  // free(sol1);
  // free(sol2);
  return EXIT_SUCCESS;
}
