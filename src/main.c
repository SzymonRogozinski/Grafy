#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "graf.h"

#define DEFAULT_X 10
#define DEFAULT_Y 10
#define DEFAULT_N 1
#define DEFAULT_MIN 0
#define DEFAULT_MAX 1

int main(int argc, char **argv)
{
    int opt;
    char *in = NULL; // plik wejściowy
    char *out = NULL; // plik wyjściowy
    int byParam = 0; // czy podano dane wejściowe przez argumenty wywołania; 0 - nie, 1 - tak

    graph_t *gp = malloc(sizeof *gp);
    zainicjalizuj_graf(gp);

    int st = -1; // wierzchołki START - STOP do wyznaczania drogi
    int sp = -1;

    while ((opt = getopt(argc, argv, "s:x:y:n:r:f:o:")) != -1)
    {
        switch (opt)
        {
        case 's':
            if (sscanf(optarg, "%d,%d", &st, &sp) != 2)
            {
                fprintf(stderr, "Błędny format wartości argumentu \"-s\" - oczekiwano \"<st>,<sp>\". Przerywam działanie.\n");

                exit(EXIT_FAILURE);
            }

            if (st < 0 || sp < 0)
            {
                fprintf(stderr, "Indeksy wierzchołków nie mogą być ujemne - wczytano “%d,%d”. Przerywam działanie.\n", st, sp);

                exit(EXIT_FAILURE);
            }

            break;
        case 'r':
            byParam = 1;

            if (sscanf(optarg, "%d-%d", &gp->min, &gp->max) != 2)
            {
                fprintf(stderr, "Błędny format wartości argumentu \"-r\" - oczekiwano \"<min>-<max>\". Przerywam działanie.\n");

                exit(EXIT_FAILURE);
            }

            if (gp->min < 0 || gp->max <= gp->min)
            {
                fprintf(stderr, "Błędny zakres wag krawędzi - wczytano “%d-%d”. Przerywam działanie.\n", gp->min, gp->max);

                exit(EXIT_FAILURE);
            }
            break;
        case 'x':
            byParam = 1;
            gp->x = atoi(optarg);

            if (gp->x <= 0)
            {
                fprintf(stderr, "Liczba kolumn musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", atoi(optarg));
                exit(EXIT_FAILURE);
            }

            break;
        case 'y':
            byParam = 1;
            gp->y = atoi(optarg);

            if (gp->y <= 0)
            {
                fprintf(stderr, "Liczba wierszy musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", atoi(optarg));
                exit(EXIT_FAILURE);
            }

            break;
        case 'n':
            byParam = 1;
            gp->n = atoi(optarg);

            if (gp->n <= 0)
            {
                fprintf(stderr, "Liczba spójnych grafów musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", atoi(optarg));
                exit(EXIT_FAILURE);
            }

            break;
        case 'f':
            in = optarg;
            break;
        case 'o':
            out = optarg;
            break;
        default:
            fprintf(stderr, "Wykryto błędny parametr w argumentach wywołania. Przerywam działanie.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (optind < argc)
    {
        fprintf(stderr, "\nWykryto błędny parametr w argumentach wywołania. Przerywam działanie.\n");

        exit(EXIT_FAILURE);
    }

    if (st == -1 || sp == -1)
    {
        fprintf(stderr, "Nie zdefiniowano wierzchołków, między którymi miałaby zostać wyznaczona droga. Przerywam działanie.\n");

        exit(EXIT_FAILURE);
    }

    // GENEROWANIE GRAFU

    if (in != NULL) // jeżeli podano plik wejściowy
    {
        if (byParam) // jeżeli podano też dane przez argumenty wywołania
        {
            printf("Podano dane wejściowe poprzez argumenty wywołania oraz plik wejściowy. Generuję graf na podstawie danych z pliku...\n");
        }

        return 0; // TODO: wczytywanie grafu
    }
    else // generowanie na podstawie danych wejściowych
    {
        if (gp->x == -1)
        {
            printf("Nie podano parametru: liczba kolumn (X). Używam wartości domyślnej X=%d.\n", DEFAULT_X);
            gp->x = DEFAULT_X;
        }
        if (gp->y == -1)
        {
            printf("Nie podano parametru: liczba wierszy (Y). Używam wartości domyślnej Y=%d.\n", DEFAULT_Y);
            gp->y = DEFAULT_Y;
        }
        if (gp->n == -1)
        {
            printf("Nie podano parametru: liczba spójnych grafów (N). Używam wartości domyślnej N=%d.\n", DEFAULT_N);
            gp->n = DEFAULT_N;
        }
        if (gp->min == -1 || gp->max == -1)
        {
            printf("Nie podano parametru: zakres wartości wag (MIN-MAX). Używam wartości domyślnej MIN=%d, MAX=%d.\n", DEFAULT_MIN, DEFAULT_MAX);
            gp->min = DEFAULT_MIN;
            gp->max = DEFAULT_MAX;
        }

        if (gp->n > gp->x * gp->y)
        {
            fprintf(stderr, "Liczba spójnych grafów musi być mniejsza od liczby wierzchołków. Przerywam działanie.\n");

            exit(EXIT_FAILURE);
        }
    }
    // po wygenerowaniu grafu

    if (out != NULL)
    {
        return 0; // TODO: zapisanie grafu do pliku
    }

    if (st >= gp->x * gp->y || sp >= gp->x * gp->y) // wierzchołek ST/SP poza dostępnym zakresem
    {
        fprintf(stderr, "Indeksy wierzchołków ST=%d, SP=%d nie należą do zakresu <0;%d>. Przerywam działanie.\n", st, sp, gp->x * gp->y - 1);

        exit(EXIT_FAILURE);
    }

    return 0;
}