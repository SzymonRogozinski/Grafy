#ifndef __GRAPH__
#define __GRAPH__
#include <stdio.h>

typedef struct graf {
	int x; //liczba kolumn
	int y; //liczba wierszy
	//int n; Liczba graf�w
	double*** w; //wierzcho�ki
}GraF;

int wczytaj_graf(FILE *, GraF*);

#endif 