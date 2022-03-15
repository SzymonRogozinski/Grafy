#include <stdio.h>
#include <stdlib.h>

#include "graf.h"

int main() {
	FILE* F = fopen("mygraph","r"); //Testowy plik, do sprawdzania, czy dzia³a wczytywanie
	if (F == NULL)
		return 1;
	GraF* G = malloc(sizeof(GraF));
	if(wczytaj_graf(F,G)==1)
		return 1;
	return 0;
}