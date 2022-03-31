#include <stdlib.h>
#include <stdio.h>

#include "prqueue.h"

prqueue_t *zainicjalizuj_kolejke(int rozmiar)
{
    prqueue_t *kolejka = malloc(sizeof *kolejka);
    kolejka->n = 0;
    kolejka->queue = malloc(rozmiar * sizeof *kolejka->queue);
    kolejka->size = rozmiar;

    return kolejka;
}

int dodaj_element_pr(prqueue_t *kolejka, int el)
{
    if (kolejka->n == kolejka->size) // jeżeli kolejka jest pełna, to przerwij działanie
    {
        fprintf(stderr, "Kolejka jest pełna, nie można dodać więcej elementów.\n");
        return 0;
    }

    kolejka->queue[kolejka->n] = el;
    kolejka->n++;

    return 1;
}

int usun_element_pr(prqueue_t *kolejka, double *dyst)
{
    if (czy_pusta_pr(kolejka))
    {
        fprintf(stderr, "Kolejka jest pusta, nie można wyjąć żadnych elementów.\n");
        return -1;
    }

    double min = DBL_MAX; // będzie przechowywać najmniejszy dystans
    int ind = -1;         // indeks w kolejce
    int num = -1;         // indeks wierzchołka w grafie

    for (int i = 0; i < kolejka->n; i++)
    {
        if (dyst[kolejka->queue[i]] < min)
        {
            min = dyst[kolejka->queue[i]];
            num = kolejka->queue[i];
            ind = i;
        }
    }

    kolejka->n--;
    kolejka->queue[ind] = kolejka->queue[kolejka->n];

    return num;
}

int czy_pusta_pr(prqueue_t *kolejka)
{
    return (kolejka->n == 0);
}