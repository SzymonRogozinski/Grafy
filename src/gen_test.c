#include <stdio.h>
#include <stdlib.h>

#include "graf.h"

int main(int argc, char **argv)
{
    if (argc < 6)
    {
        printf("Użycie: ./gen_test [x] [y] [n] [min] [max]\n");
        return 1;
    }

    graph_t *G = malloc(sizeof *G);
    zainicjalizuj_graf(G);

	G->x = atoi(argv[1]);
	G->y = atoi(argv[2]);
	G->n = atoi(argv[3]);
	G->min = atof(argv[4]);
	G->max = atof(argv[5]);

    if (generuj_graf(G) != 1)
    {
        fprintf(stderr, "Wystąpił błąd podczas generowania grafu.\n");
        return 1;
    }

    wyznacz_n_siatki(G);

    printf("X = %d\n", G->x);
    printf("Y = %d\n", G->y);
    printf("MIN = %g\n", G->min);
    printf("MAX = %g\n", G->max);
    printf("N = %d\n", G->n);

    printf("\nWierzchołki:\n");

    for (int i = 0; i < G->x * G->y; i++)
    {
        printf("%d. wierzchołek: \t", i);

        for (int j = 0; j < 8; j += 2)
        {
            if (G->w[i][j] == DEFAULT_VALUE)
                break;

            printf("%d:%g\t", (int)G->w[i][j], G->w[i][j + 1]);
        }

        printf("\n");
    }

    printf("\n");

    if (sprawdz_integralnosc(G))
        printf("Dane w grafie są integralne.\n");
    else
        printf("Dane w grafie nie są integralne.\n");

    FILE *ouf = fopen("wyj.txt", "w+");

    if (ouf != NULL)
    {
        zapisz_graf(ouf, G);
        return 0;
    }
    else
    {
        fprintf(stderr, "Nie udało się wyeksportować grafu do pliku.\n");
        return 1;
    }
}