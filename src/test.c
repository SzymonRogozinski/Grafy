#include <stdio.h>

#include "graf.h"

void main() {
	graph_t* T= malloc(sizeof * T);;
	T->max = 10;
	T->min = 1;
	T->x = 6;
	T->y = 5;
	T->n = 5;
	T->w = malloc(T->x * T->y * sizeof * T->w);
	int d=generuj_graf(T);
	zapisz_graf(stdout,T);
}