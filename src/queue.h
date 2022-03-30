#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct Kolejka
{
    int *queue; // elementy kolejki
    int front;  // indeks elementu na początku kolejki
    int end;    // indeks ostatniego elementu kolejki
    int size;   // maks. rozmiar kolejki
    int n;      // liczba elementów w kolejce
} queue_t;

queue_t *zainicjalizuj_kolejke(int); // inicjalizuje kolejkę o określonym rozmiarze
int dodaj_element(queue_t *, int);   // dodaje element do kolejki; 0 - nie udało się dodać, 1 - udało się
int usun_element(queue_t *);         // wyjmuje element z początku kolejki
int czy_pusta(queue_t *);            // sprawdza, czy w kolejce są jakieś elementy; 0 - tak, 1 - nie
void zrestartuj_kolejke(queue_t *)   // przywraca wartości domyślne kolejki

#endif