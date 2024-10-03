#pragma once

typedef struct Qap{
  int n;
  int *dis;
  int *flujo;
} Qap;

void print_qap(Qap *qap);

void read_qap(Qap **qap);

void free_qap(Qap *qap);

int objective_function(int* sol, Qap* qap);

void print_sol(int* sol, int n);

int* constructive(Qap *qap);

int* random_sol(int n);

void swap(int* a, int* b);

bool is_local_optimal(int* sol, Qap *qap);

int schnabel_census_prodecure(int m, Qap *qap);
