#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "errors.h"

queue_t *zainicjalizuj_kolejke(int rozmiar)
{
    queue_t *kolejka = malloc(sizeof *kolejka);

    if (kolejka == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na strukturę kolejki. Przerywam działanie.\n");

        exit(ERROR_ALLOC_QUEUE);
    }

    kolejka->n = 0;
    kolejka->queue = malloc(rozmiar * sizeof *kolejka->queue);
    kolejka->size = rozmiar;
    kolejka->front = 0;
    kolejka->end = -1;

    if (kolejka->queue == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na tablicę elementów kolejki. Przerywam działanie.\n");

        exit(ERROR_ALLOC_Q_ARRAY);
    }

    return kolejka;
}

void dodaj_element(queue_t *kolejka, int el)
{
    if (kolejka->end == kolejka->size - 1) // jeżeli kolejka jest pełna, to przerwij działanie
    {
        fprintf(stderr, "Kolejka jest pełna, nie można dodać więcej elementów.\n");

        exit(ERROR_QUEUE_FULL);
    }

    kolejka->end++;
    kolejka->queue[kolejka->end] = el;
    kolejka->n++;
}

int usun_element(queue_t *kolejka)
{
    if (czy_pusta(kolejka))
    {
        fprintf(stderr, "Kolejka jest pusta, nie można wyjąć żadnych elementów.\n");
        return -1;
    }

    kolejka->front++;
    kolejka->n--;

    return kolejka->queue[kolejka->front - 1];
}

int czy_pusta(queue_t *kolejka)
{
    return (kolejka->n == 0);
}

void zrestartuj_kolejke(queue_t *kolejka)
{
    kolejka->n = 0;
    kolejka->front = 0;
    kolejka->end = -1;
}