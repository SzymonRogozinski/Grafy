#include <stdio.h>

#include "graf.h"

void main() {
	graph_t* T= malloc(sizeof * T);;
	T->max = 10;
	T->min = 1;
	T->x = 10;
	T->y = 6;
	T->n = 20;
	T->w = malloc(T->x * T->y * sizeof * T->w);
	int d=generuj_graf(T);
	zapisz_graf(stdout,T);
	if (d)
		printf("Bladdddddd\n");
}