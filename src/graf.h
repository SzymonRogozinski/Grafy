#ifndef _GRAF_H_
#define _GRAF_H_

#include <stdio.h>

#define DEFAULT_VALUE -1

typedef struct Graf
{
	int x;			 // liczba kolumn
	int y;			 // liczba wierszy
	int n;			 // liczba spójnych grafów
	double min, max; // zakres wartości na wagach krawędzi w grafie
	double **w;		 // lista sąsiedstwa dla wierzchołków i wag
} graph_t;

int sprawdz_integralnosc(graph_t *);	   // sprawdza, czy połączenia i wagi w grafie zgadzają się ze sobą (1 - tak, 0 - nie)
int czy_sasiaduja(int, int, int, int);	   // sprawdza, czy dwa wierzchołki sąsiadują ze sobą w grafie-siatce (1 - tak, 0 - nie)
void zainicjalizuj_graf(graph_t *);		   // inicjalizuje zmienne w strukturze na wartości tymczasowe
int wczytaj_graf(FILE *, graph_t *);	   // wczytuje graf z pliku
void zapisz_graf(FILE *, graph_t *);	   // zapisuje graf do pliku
void zwolnij_graf(graph_t *);			   // zwalnia pamięć po strukturach grafu
int znajdz_droge_bfs(graph_t *, int, int); // sprawdza, czy istnieje droga między dwoma wierzchołkami; 0 - nie, 1 - tak
void wyznacz_n_siatki(graph_t *);		   // wyznacza liczbę spójnych grafów w siatce
void znajdz_droge(graph_t *, int, int);	   // wyznacza najkrótszą drogę między dwoma wierzchołkami
void wyswietl_sciezke(int *, int);		   // wyświetla ścieżkę poprzedników
int generuj_graf(int, int, double, double, int);	//Generuje graf o podanych parametrach

#endif