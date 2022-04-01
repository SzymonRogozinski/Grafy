#ifndef _PRQUEUE_H_
#define _PRQUEUE_H_

#include <float.h>

typedef struct Kolejka_Priorytetowa
{
    int *queue; // elementy kolejki
    int size;   // maks. rozmiar kolejki
    int n;      // liczba elementów w kolejce
} prqueue_t;

prqueue_t *zainicjalizuj_kolejke_pr(int); // inicjalizuje kolejkę o określonym rozmiarze
int dodaj_element_pr(prqueue_t *, int);        // dodaje element do kolejki
int usun_element_pr(prqueue_t *, double *);    // wyjmuje element z kolejki
int czy_pusta_pr(prqueue_t *);                 // sprawdza, czy w kolejce są jakieś elementy; 0 - tak, 1 - nie

#endif