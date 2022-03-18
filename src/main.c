#include <stdio.h>
#include <stdlib.h>

#include "graf.h"

int main()
{
	FILE *F = fopen("mygraph", "r"); // testowy plik do sprawdzania, czy działa wczytywanie

	if (F == NULL)
		return 1;

	graph_t *G = malloc(sizeof *G);

	if (wczytaj_graf(F, G) == 1)
		return 1;

	return 0;
}