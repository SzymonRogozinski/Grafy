#ifndef __GRAPH__
#define __GRAPH__

typedef struct graf {
	int x; //liczba kolumn
	int y; //liczba wierszy
	//int n; Liczba graf�w
	double*** w; //wierzcho�ki
}GF;

void wczytaj_graf();

#endif 
