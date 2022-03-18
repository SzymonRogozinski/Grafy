#ifndef _GRAF_H_
#define _GRAF_H_

typedef struct Graf
{
	int x;		// liczba kolumn
	int y;		// liczba wierszy
	int n;		// liczba spójnych grafów
	double **w; // lista sąsiedstwa dla wierzchołków i wag
} graph_t;

int wczytaj_graf(FILE *, graph_t *); // wczytuje graf z pliku
void zapisz_graf(FILE *, graph_t *); // zapisuje graf do pliku
void free_graf(graph_t *);			 // zwalnia pamięć po strukturach grafu

#endif