#include <stdio.h>
#include <stdlib.h>

#include "graf.h"

int wczytaj_graf(FILE * F, GraF* gp) {
	int a, b;
	if (fscanf(F,"%d %d", &a, &b) != 2)
		return 1;
	//Wczytywanie wymiarów
	gp->x = b;
	gp->y = a;

	return 0;
}
