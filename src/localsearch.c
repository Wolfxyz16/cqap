#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "qap.h"

// busqueda local, funcion de vecindario y criterio de seleccion
// recibe una solucion y devuelve una matriz con las soluciones vecinas
int** swap_local_search(int *sol, int n)
{
  int i, j, cont_vecinos = 0;
  int num_vecinos = (n * (n - 1)) / 2;
  int** vecinos = (int**) malloc(sizeof(int*) * num_vecinos); 
  int new_vecino[n];

  for (i = 0; i < n-1; i++) 
  {
    for (j = i+1; j < n; j++)
    {
      vecinos[cont_vecinos] = (int*) malloc(sizeof(int) * n);
      memcpy(new_vecino, sol, sizeof(int) * n);
      swap(&new_vecino[i], &new_vecino[j]);
      memcpy(vecinos[cont_vecinos], new_vecino, sizeof(int)*n);
      cont_vecinos = cont_vecinos + 1;
    }
  }
  return vecinos;
}

// dada una solucion encuentra el optimo local a partir del best first
int* best_first(Qap *qap, int *initial_solution)
{
  unsigned int obj_function_cont = 1;
  int i, j;
  int best_value = objective_function(initial_solution, qap);
  int new_value;
  int *new_vecino = (int*) malloc(sizeof(int)*qap->n);
  int *best_sol = (int*) malloc(sizeof(int)*qap->n);
  int *random_permutation = (int*) malloc(sizeof(int)*qap->n);
  bool sol_update = true;

  // copiamos el vector inicial
  memcpy(new_vecino, initial_solution, sizeof(int)*qap->n);
  memcpy(best_sol, initial_solution, sizeof(int)*qap->n);

  // vamos evaluando los vecinos mientras la solucion se actualice
  while (sol_update == true) 
  {
    // si no mejoramos el value, estamos en un optimo local, paramos.
    sol_update = false;
    i = 0, j = i + 1;
    random_permutation = random_sol(qap->n);
    // vamos a recorrer los vecinos 1 a 1
    while (sol_update == false && i < qap->n - 1)
    {
      memcpy(new_vecino, best_sol,sizeof(int) * qap->n);
      swap(&new_vecino[random_permutation[i] - 1], &new_vecino[random_permutation[j] - 1]);
      new_value = objective_function(new_vecino, qap);
      obj_function_cont = obj_function_cont + 1;
      if( new_value < best_value )
      {
        best_value = new_value;
        memcpy(best_sol, new_vecino, sizeof(int)*qap->n);
        sol_update == true;
      }
      i = i + 1;
      j = j + 1;
    }
  }
  printf("BEST FIRST SEARCH: Han habido %d evaluaciones.\n", obj_function_cont);
  free(new_vecino);
  free(random_permutation);
  return best_sol;
}
