#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define DEFAULT_X 10
#define DEFAULT_Y 10
#define DEFAULT_N 1
#define DEFAULT_MIN 0
#define DEFAULT_MAX 1


int main(int argc, char **argv)
{
    int opt;
    char *in = NULL;
    char *out = NULL;
    int byParam = 0; // czy podano dane wejściowe przez argumenty wywołania; 0 - nie, 1 - tak
    // -1 - brak wartości
    int x = -1;
    int y = -1;
    int n = -1;
    int min = -1;
    int max = -1;
    int st = -1;
    int sp = -1; // TODO: zmienić te wartości na strukturę jak już będzie

    while ((opt = getopt(argc, argv, "s:x:y:n:r:f:o:")))
    {
        switch (opt)
        {
            case 's':
                if (sscanf(optarg, "%d,%d", &st, &sp) != 2)
                {
                    fprintf(stderr,
                            "Błędny format wartości argumentu \"-s\" - oczeki   wano \"<st>,<sp>\". Przerywam działanie.\n");

                    exit(EXIT_FAILURE);
                }

                if (st < 0 || sp < 0)
                {
                    fprintf(stderr,
                            "Indeksy wierzchołków nie mogą być ujemne - wczytano “%d,%d”. Przerywam działanie.\n", st,
                            sp);

                    exit(EXIT_FAILURE);
                }

                break;
            case 'r':
                byParam = 1;

                if (sscanf(optarg, "%d-%d", &min, &max) != 2)
                {
                    fprintf(stderr,
                            "Błędny format wartości argumentu \"-r\" - oczekiwano \"<min>-<max>\". Przerywam działanie.\n");

                    exit(EXIT_FAILURE);
                }

                if (min < 0 || max <= min)
                {
                    fprintf(stderr, "Błędny zakres wag krawędzi - wczytano “%d-%d”. Przerywam działanie.\n", min, max);

                    exit(EXIT_FAILURE);
                }
                break;
            case 'x':
                byParam = 1;
                x = atoi(optarg);

                if (x <= 0)
                {
                    fprintf(stderr, "Liczba kolumn musi być większa od zera – wczytano “%d”. Przerywam działanie.",
                            atoi(optarg));
                    exit(EXIT_FAILURE);
                }

                break;
            case 'y':
                byParam = 1;
                y = atoi(optarg);

                if (y <= 0)
                {
                    fprintf(stderr, "Liczba wierszy musi być większa od zera – wczytano “%d”. Przerywam działanie.",
                            atoi(optarg));
                    exit(EXIT_FAILURE);
                }

                break;
            case 'n':
                byParam = 1;
                n = atoi(optarg);

                if (n <= 0)
                {
                    fprintf(stderr,
                            "Liczba spójnych grafów musi być większa od zera – wczytano “%d”. Przerywam działanie.",
                            atoi(optarg));
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
                fprintf(stderr, "Wykryto błędny parametr w argumentach wywołania. Przerywam działanie.");
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
        fprintf(stderr,
                "Nie zdefiniowano wierzchołków, między którymi miałaby zostać wyznaczona droga. Przerywam działanie.\n");

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
    } else // generowanie na podstawie danych wejściowych
    {
        if (x == -1)
        {
            printf("Nie podano parametru: liczba kolumn (X). Używam wartości domyślnej X=%d.", DEFAULT_X);
            x = DEFAULT_X;
        }
        if (y == -1)
        {
            printf("Nie podano parametru: liczba wierszy (Y). Używam wartości domyślnej Y=%d.", DEFAULT_Y);
            y = DEFAULT_Y;
        }
        if (n == -1)
        {
            printf("Nie podano parametru: liczba spójnych grafów (N). Używam wartości domyślnej N=%d.", DEFAULT_N);
            n = DEFAULT_N;
        }
        if (min == -1 || max == -1)
        {
            printf("Nie podano parametru: zakres wartości wag (MIN-MAX). Używam wartości domyślnej MIN=%d, MAX=%d.",
                   DEFAULT_MIN, DEFAULT_MAX);
            min = DEFAULT_MIN;
            max = DEFAULT_MAX;
        }

        if (n > x * y)
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

    if (st >= x * y || sp >= x * y) // wierzchołek ST/SP poza dostępnym zakresem
    {
        fprintf(stderr, "Indeksy wierzchołków ST=%d, SP=%d nie należą do zakresu <0;%d>. Przerywam działanie.\n", st,
                sp, x * y - 1);

        exit(EXIT_FAILURE);
    }


    return 0;
}
