#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include "../include/qap.h"

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void print_qap(Qap *qap)
{
  int i, j;
  int n = qap-> n;

  printf("n = %d\n", qap->n);

  printf("DISTANCIAS\n");
  for (i = 0; i < n; i++) 
  {
    for (j = 0; j < n; j++)
    {
      printf("%d \t", qap->dis[i*n+j]);
    }
    printf("\n");
  }

  printf("\nFLUJOS\n");
  for (i = 0; i < n; i++) 
  {
    for (j = 0; j < n; j++)
    {
      printf("%d \t", qap->flujo[i*n+j]);
    }
    printf("\n");
  }
}


// reads a file and load a qap struct
void read_qap(Qap **qap)
{
  int i, n;
  FILE *file = fopen("./datos/tai20a.dat", "r");
  char buffer[128];

  if (file == NULL) {
    perror("Error leyendo el archivo");
  }

  (*qap) = (Qap*) malloc(sizeof(Qap));

  fscanf(file, "%d", &n);

  (*qap)->n = n;
  (*qap)->dis = (int*) malloc(sizeof(int)*n*n);
  (*qap)->flujo = (int*) malloc(sizeof(int)*n*n);

  for(i = 0; i < n*n; i++)
  {
    fscanf(file, "%d", &((*qap)->dis[i]));
  }

  for(i = 0; i < n*n; i++)
  {
    fscanf(file, "%d", &((*qap)->flujo[i]));
  }

  fclose(file);
}

// free the malloc from the qap
void free_qap(Qap *qap)
{
  free(qap->dis);
  free(qap->flujo);
  free(qap);
}

int objective_function(int *sol, Qap *qap)
{
  int i, j, ubicacion_fabrica_i, ubicacion_fabrica_j, valor_distancia, valor_flujo, value = 0;

  for (i = 0; i < qap->n; i++)
  {
    ubicacion_fabrica_i = sol[i] - 1;
    for (j = 0; j < qap->n; j++)
    {
      ubicacion_fabrica_j = sol[j] - 1;
      valor_flujo = qap->flujo[ubicacion_fabrica_i*qap->n + ubicacion_fabrica_j];
      valor_distancia = qap->dis[i*qap->n + j];
      value += valor_flujo * valor_distancia; 
    }
  }
  return value;
}

void print_sol(int* sol, int n)
{
  int i;
  for (i = 0; i < n; i++) 
  {
    printf("%d ", sol[i]);
  }
  printf("\n");
}

// construye la mejor solucion de tamaño n siguiendo un algoritmo
int* constructive(Qap *qap)
{
  int i, j, k;
  int n = qap->n;
  int* fila_flujo = (int*) calloc(n, sizeof(int) * n);
  int* fila_dis = (int*) calloc(n, sizeof(int) * n);
  int* sol = (int*) malloc(sizeof(int) * n);
  int max, min;
  int max_pos = 0, min_pos = 0;
  
  // sumamos las filas de la matriz de flujo y la matriz de distancias. Las guardamos en una lista
  for ( i = 0; i < n; i++ )
  {
    for ( j = 0; j < n; j++ ) 
    {
      fila_flujo[i] = fila_flujo[i] + qap->flujo[i*n + j];
      fila_dis[i] = fila_dis[i] + qap->dis[i*n + j];
    }
  }
  //
  // encontramos el mayor flujo y lo juntamos con la menor distancia
  for ( k = 0 ; k < n ; k++ )
  {
    max = fila_flujo[max_pos];
    min = fila_dis[min_pos];
    for ( i = 1 ; i < n ; i++ )
    {
      if(fila_flujo[i] > max) 
      {
        max = fila_flujo[i];
        max_pos = i;
      }
      if(fila_dis[i] < min)
      {
        min = fila_dis[i];
        min_pos = i;
      }
    }
    fila_flujo[max_pos] = INT_MIN;
    fila_dis[min_pos] = INT_MAX;
    sol[min_pos] = max_pos + 1;

    max_pos = 0;
    min_pos = 0;
  }

  free(fila_flujo);
  free(fila_dis);
  return sol;
}

int* random_sol(int n)
{
  int i, j;
  int* sol = (int*) malloc(sizeof(int) * n);

  for ( i = 0; i < n; i++ ) sol[i] = i+1;

  srand(time(NULL));

  for ( i = n -1; i > 0; i-- ) 
  {
    j = rand() % (i+1);
    swap(&sol[i], &sol[j]);
  }

  return sol;
}

// devuelve true o false si la solucion es un optimo local
bool is_local_optimal(int* sol, Qap *qap) 
{
  int* new_vecino = (int*) malloc(sizeof(int) * qap->n);
  int value = objective_function(sol, qap);
  int new_value;
  int i = 0;
  int j = i + 1;

  while (i < qap->n - 1) 
  {
    if ( j == qap->n -1 ) 
    {
      i += 1;
      j = i + 1;
    }
    memcpy(new_vecino, sol, sizeof(int) * qap->n);
    swap(&new_vecino[i], &new_vecino[j]);

    new_value = objective_function(new_vecino, qap);

    if ( new_value < value ) {
      free(new_vecino);
      return false;
    }

    j += 1;
  }
  free(new_vecino);
  return true;
}

// 2000
// m -> numero de soluciones generadas
int schnabel_census_prodecure(int m, Qap *qap)
{
  // generamos m soluciones al azar
  int num_optimos_locales = 0;
  int* rand_sol;
  int i;

  #pragma omp parallel for private(rand_sol) reduction(+:num_optimos_locales)
  for ( i = 0 ; i < m; i++ ) 
  {
    rand_sol = random_sol(qap->n);
    if (is_local_optimal(rand_sol, qap)) num_optimos_locales += 1;
    free(rand_sol);
  }

  // ratio = (float) num_optimos_locales / (float) m;

  return num_optimos_locales;
}
