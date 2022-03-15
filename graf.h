#ifndef __GRAPH__
#define __GRAPH__
#include <stdio.h>

typedef struct graf {
	int x; //liczba kolumn
	int y; //liczba wierszy
	//int n; Liczba grafów
	double*** w; //wierzcho³ki
}GraF;

int wczytaj_graf(FILE *, GraF*);

#endif 