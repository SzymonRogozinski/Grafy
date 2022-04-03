#include "queue.h"
#include <stdio.h>

int main(void)
{
    printf("1. Inicjalizacja kolejki o rozmiarze 10.\n");
    queue_t *kolejka = zainicjalizuj_kolejke(10);
    printf("N=%d, SIZE=%d\n", kolejka->n, kolejka->size);

    printf("2. Dodaj 5 do kolejki.\n");
    dodaj_element(kolejka, 5);
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    printf("3. Dodaj 7 do kolejki.\n");
    dodaj_element(kolejka, 7);
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    printf("4. Dodaj 3 do kolejki.\n");
    dodaj_element(kolejka, 3);
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    printf("5. Dodaj 12 do kolejki.\n");
    dodaj_element(kolejka, 12);
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    printf("6. Usuń element z kolejki.\n");
    printf("Usunięto: %d\n", usun_element(kolejka));
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    printf("7. Usuń element z kolejki.\n");
    printf("Usunięto: %d\n", usun_element(kolejka));
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    printf("8. Dodaj 15 do kolejki.\n");
    dodaj_element(kolejka, 15);
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    printf("9. Usuń element z kolejki.\n");
    printf("Usunięto: %d\n", usun_element(kolejka));
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    printf("10. Usuń element z kolejki.\n");
    printf("Usunięto: %d\n", usun_element(kolejka));
    printf("N=%d, FRONT=%d, END=%d\n", kolejka->n, kolejka->queue[kolejka->front], kolejka->queue[kolejka->end]);

    return 0;
}