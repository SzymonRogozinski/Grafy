#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "graf.h"
#include "errors.h"

#define DEFAULT_X 10
#define DEFAULT_Y 10
#define DEFAULT_N 1
#define DEFAULT_MIN 0
#define DEFAULT_MAX 1

#define PARAM_COLUMN_COUNT 'x'
#define PARAM_ROW_COUNT 'y'
#define PARAM_PATH_NODES 's'
#define PARAM_GRAPH_COUNT 'n'
#define PARAM_WEIGHT_RANGE 'r'
#define PARAM_OUTPUT_FILE 'o'
#define PARAM_INPUT_FILE 'f'

int main(int argc, char **argv)
{
    int opt, errnum;
    char *in = NULL;                  // plik wejściowy
    char *out = NULL;                 // plik wyjściowy
    int byParam = 0;                  // czy podano dane wejściowe przez argumenty wywołania; 0 - nie, 1 - tak
    graph_t *gp = malloc(sizeof *gp); // struktura grafu

    if (gp == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na strukturę grafu. Przerywam działanie.\n");

        return ERROR_ALLOC_GRAPH;
    }

    zainicjalizuj_graf(gp);

    int st = DEFAULT_VALUE; // wierzchołki START - STOP do wyznaczania drogi
    int sp = DEFAULT_VALUE;

    while ((opt = getopt(argc, argv, "s:x:y:n:r:f:o:")) != -1)
    {
        switch (opt)
        {
        case PARAM_PATH_NODES:
            if (sscanf(optarg, "%d,%d", &st, &sp) != 2)
            {
                fprintf(stderr, "Błędny format wartości argumentu \"-s\" - oczekiwano \"<st>,<sp>\". Przerywam działanie.\n");

                return ERROR_NODES_FORMAT;
            }

            if (st < 0 || sp < 0)
            {
                fprintf(stderr, "Indeksy wierzchołków nie mogą być ujemne - wczytano “%d,%d”. Przerywam działanie.\n", st, sp);

                return ERROR_NEG_PATH_NODES;
            }

            break;
        case PARAM_WEIGHT_RANGE:
            byParam = 1;

            if (sscanf(optarg, "%lf-%lf", &gp->min, &gp->max) != 2)
            {
                fprintf(stderr, "Błędny format wartości argumentu \"-r\" - oczekiwano \"<min>-<max>\". Przerywam działanie.\n");

                return ERROR_RANGE_FORMAT;
            }

            if (gp->min < 0 || gp->max <= gp->min)
            {
                fprintf(stderr, "Błędny zakres wag krawędzi - wczytano “%g-%g”. Przerywam działanie.\n", gp->min, gp->max);

                return ERROR_BAD_WEIGHT_RANGE;
            }
            break;
        case PARAM_COLUMN_COUNT:
            byParam = 1;
            gp->x = atoi(optarg);

            if (gp->x <= 0)
            {
                fprintf(stderr, "Liczba kolumn musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", atoi(optarg));
                return ERROR_NEG_COLUMN;
            }

            break;
        case PARAM_ROW_COUNT:
            byParam = 1;
            gp->y = atoi(optarg);

            if (gp->y <= 0)
            {
                fprintf(stderr, "Liczba wierszy musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", atoi(optarg));
                return ERROR_NEG_ROW;
            }

            break;
        case PARAM_GRAPH_COUNT:
            byParam = 1;
            gp->n = atoi(optarg);

            if (gp->n <= 0)
            {
                fprintf(stderr, "Liczba spójnych grafów musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", atoi(optarg));
                return ERROR_NEG_SUBGRAPH;
            }

            break;
        case PARAM_INPUT_FILE:
            in = optarg;
            break;
        case PARAM_OUTPUT_FILE:
            out = optarg;
            break;
        default:
            fprintf(stderr, "Wykryto błędny parametr w argumentach wywołania. Przerywam działanie.\n");
            return ERROR_UNKNOWN_PARAM;
        }
    }

    if (optind < argc)
    {
        fprintf(stderr, "\nWykryto błędny parametr w argumentach wywołania. Przerywam działanie.\n");

        return ERROR_BAD_PARAM;
    }

    if (st == DEFAULT_VALUE || sp == DEFAULT_VALUE)
    {
        fprintf(stderr, "Nie zdefiniowano wierzchołków, między którymi miałaby zostać wyznaczona droga. Przerywam działanie.\n");

        return ERROR_UNDEFINED_PATH_NODES;
    }

    // GENEROWANIE GRAFU

    if (in != NULL) // jeżeli podano plik wejściowy
    {
        if (byParam) // jeżeli podano też dane przez argumenty wywołania
        {
            printf("Podano dane wejściowe poprzez argumenty wywołania oraz plik wejściowy. Generuję graf na podstawie danych z pliku...\n");
        }

        FILE *inf = fopen(in, "r");

        if (inf == NULL)
        {
            fprintf(stderr, "Nie udało się otworzyć pliku wejściowego. Przerywam działanie.\n");

            return ERROR_OPEN_FILE;
        }

        if ((errnum = wczytaj_graf(inf, gp)) != 1)
        {
            fprintf(stderr, "Wystąpił błąd podczas wczytywania pliku wejściowego.\n");

            return errnum;
        }

        if (!sprawdz_integralnosc(gp))
        {
            fprintf(stderr, "Dane w grafie nie są integralne.\n");

            return ERROR_GRAPH_INTEGRITY;
        }

        wyznacz_n_siatki(gp);

        fclose(inf);
    }
    else // generowanie na podstawie danych wejściowych
    {
        if (gp->x == DEFAULT_VALUE)
        {
            printf("Nie podano parametru: liczba kolumn (X). Używam wartości domyślnej X=%d.\n", DEFAULT_X);
            gp->x = DEFAULT_X;
        }
        if (gp->y == DEFAULT_VALUE)
        {
            printf("Nie podano parametru: liczba wierszy (Y). Używam wartości domyślnej Y=%d.\n", DEFAULT_Y);
            gp->y = DEFAULT_Y;
        }
        if (gp->n == DEFAULT_VALUE)
        {
            printf("Nie podano parametru: liczba spójnych grafów (N). Używam wartości domyślnej N=%d.\n", DEFAULT_N);
            gp->n = DEFAULT_N;
        }
        if (gp->min == DEFAULT_VALUE || gp->max == DEFAULT_VALUE)
        {
            printf("Nie podano parametru: zakres wartości wag (MIN-MAX). Używam wartości domyślnej MIN=%d, MAX=%d.\n", DEFAULT_MIN, DEFAULT_MAX);
            gp->min = DEFAULT_MIN;
            gp->max = DEFAULT_MAX;
        }

        if (gp->n > gp->x * gp->y)
        {
            fprintf(stderr, "Liczba spójnych grafów musi być mniejsza od liczby wierzchołków. Przerywam działanie.\n");

            return ERROR_SUBGRAPH_OVER_NODE;
        }
    }
    // po wygenerowaniu grafu

    if (gp->n != 1) // jeżeli graf nie jest spójny
    {
        printf("Wygenerowany graf nie jest spójny. Wyznaczenie drogi między wierzchołkami może być niemożliwe.\n");
    }

    if (out != NULL)
    {
        FILE *ouf = fopen(out, "w+");

        if (ouf == NULL)
        {
            fprintf(stderr, "Nie udało się otworzyć pliku do zapisania grafu.\n");
        }
        else
        {
            zapisz_graf(ouf, gp);
            printf("Zapisano graf do pliku %s.\n", out);
        }

        fclose(ouf);
    }

    if (st >= gp->x * gp->y || sp >= gp->x * gp->y) // wierzchołek ST/SP poza dostępnym zakresem
    {
        fprintf(stderr, "Indeksy wierzchołków ST=%d, SP=%d nie należą do zakresu <0;%d>. Przerywam działanie.\n", st, sp, gp->x * gp->y - 1);

        return ERROR_PATH_OUT_OF_RANGE;
    }

    if (!znajdz_droge_bfs(gp, st, sp)) // jeżeli nie znaleziono drogi między dwoma wierzchołkami
    {
        fprintf(stderr, "Nie udało się znaleźć drogi między wierzchołkami %d i %d. Przerywam działanie.\n", st, sp);

        return ERROR_NO_AVAILABLE_PATH;
    }

    znajdz_droge(gp, st, sp);

    if (gp != NULL)
    {
        zwolnij_graf(gp);
        free(gp);
    }

    return EXIT_SUCCESS;
}