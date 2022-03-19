#ifndef _GRAF_H_
#define _GRAF_H_

typedef struct Graf
{
	int x;			 // liczba kolumn
	int y;			 // liczba wierszy
	int n;			 // liczba spójnych grafów
	double min, max; // zakres wartości na wagach krawędzi w grafie
	double **w;		 // lista sąsiedstwa dla wierzchołków i wag
} graph_t;

void zainicjalizuj_graf(graph_t *);	 // inicjalizuje zmienne w strukturze na wartości tymczasowe
int wczytaj_graf(FILE *, graph_t *); // wczytuje graf z pliku
void zapisz_graf(FILE *, graph_t *); // zapisuje graf do pliku
void zwolnij_graf(graph_t *);		 // zwalnia pamięć po strukturach grafu

#endif