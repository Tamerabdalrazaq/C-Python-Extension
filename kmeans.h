#ifndef FUNCTIONS_H
#define FUNCTIONS_H

double **k_means(int k, int n, int d, double epsilon, int iter, int *centroids_initial_indexes, double **data);
void free_matrix(double **matrix, int k);
#endif