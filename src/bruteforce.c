#include <stdlib.h>
#include <string.h>
#include "qap.h"

int factorial(int n)
{
  int x = 1;
  while (n > 1) 
  {
    x *= n;
    n = n - 1;
  }
  return x;
}

void reverse(int arr[], int start, int end) {
    while (start < end) {
        swap(&arr[start], &arr[end]);
        start++;
        end--;
    }
}

void next_permutation(int *arr, int n) {
    // Find the pivot index
    int pivot = -1;
    for (int i = n - 2; i >= 0; i--) {
        if (arr[i] < arr[i + 1]) {
            pivot = i;
            break;
        }
    }

    // If pivot point does not exist, 
    // reverse the whole array
    if (pivot == -1) {
        reverse(arr, 0, n - 1);
        return;
    }

    // Find the element from the right that
    // is greater than pivot
    for (int i = n - 1; i > pivot; i--) {
        if (arr[i] > arr[pivot]) {
            swap(&arr[i], &arr[pivot]);
            break;
        }
    }

    // Reverse the elements from pivot + 1 to the end
    reverse(arr, pivot + 1, n - 1);
}

// bruteforce, recibe longitud de la solucion, y las matrices distancia y flujo
int* bruteforce(Qap *qap)
{
  unsigned long long int max_permutations = factorial(qap->n);
  size_t sol_size = sizeof(int) * qap->n;
  int i = 0, j, best_objective_function;
  int* solucion = (int*) malloc(sol_size);

  // populamos la solucion inicial. Ej {1, 2, 3}
  for (j = 0; j < qap->n; j++) solucion[j] = j+1;

  // calculamos el valor de la funcion objetivo
  best_objective_function = objective_function(solucion, qap);
  int* new_permutation = (int*) malloc(sol_size);
  memcpy(new_permutation, solucion, sol_size);

  // recorremos todas las permutaciones, nos quedamos con el array que tenga mayor funcion objetivo
  while (i < max_permutations) 
  {
    next_permutation(new_permutation, qap->n);
    int new_objective_function = objective_function(new_permutation, qap);
    if (new_objective_function < best_objective_function) {
      memcpy(solucion, new_permutation, sol_size);
      best_objective_function = new_objective_function;
    }
    i = i + 1;
  }
  free(new_permutation);
  return solucion;
}
