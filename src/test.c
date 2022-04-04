#include <stdio.h>

#include "graf.h"

void main() {
	graph_t* T= malloc(sizeof * T);;
	T->max = 10;
	T->min = 1;
	T->x = 1000;
	T->y = 500;
	T->n = 50;
	T->w = malloc(T->x * T->y * sizeof * T->w);
	int d=generuj_graf(T);
	//zapisz_graf(stdout,T);
	if (d)
		printf("Bladdddddd\n");
	wyznacz_n_siatki(T);
	printf("Liczba grafów: %d\n",T->n);
}