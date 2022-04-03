#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graf.h"
#include "queue.h"
#include "prqueue.h"
#include "errors.h"

#define MAX_LENGTH 128
#define LIST_EDGE -1

int czy_sasiaduja(int w1, int w2, int w, int k)
{
    int row1 = (w1 - w1 % k) / k + 1;
    int row2 = (w2 - w2 % k) / k + 1;
    int col1 = w1 % k + 1;
    int col2 = w2 % k + 1;

    return (abs(row1 - row2) == 1 || abs(col1 - col2) == 1);
}

int wczytaj_graf(FILE *inf, graph_t *gp)
{
    char buf[MAX_LENGTH]; // bufor
    int counter;          // ile sąsiadów w jednym linii
    int tmp1, ch;
    double tmp2;        // zmienna do wczytywania danych
    int wierz[4];       // przechowuje indeksy wierzchołków, żeby nie dodać dwóch takich samych w jednej linii
    int czy_znaleziono; // potrzebne do warunku wyżej
    FILE *strstream;

    int edge; // liczba wczytanych danych

    if (fgets(buf, MAX_LENGTH, inf) != NULL) // fgets, bo nie wiadomo jak się zachowa po fscanf -> fgets
    {
        if (sscanf(buf, "%d %d\n", &gp->y, &gp->x) != 2)
        {
            fprintf(stderr, "Nie udało się wczytać wymiarów grafu. Przerywam działanie.\n");
            return ERROR_READ_DIMENSIONS;
        }
    }
    else
    {
        fprintf(stderr, "Nie udało się wczytać wymiarów grafu. Przerywam działanie.\n");
        return ERROR_READ_DIMENSIONS;
    }

    if (gp->y <= 0 || gp->x <= 0) // warunek X > 0, Y > 0
    {
        if (gp->y <= 0)
            fprintf(stderr, "Liczba wierszy musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", gp->y);

        if (gp->x <= 0)
            fprintf(stderr, "Liczba kolumn musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", gp->x);

        return ERROR_NEG_DIMENSIONS;
    }

    if ((gp->w = malloc(gp->y * gp->x * sizeof *gp->w)) == NULL) // alokowanie pamięci na wierzchołki
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na listę sąsiedstwa. Przerywam działanie.\n");

        return ERROR_ALLOC_GRAPH_LIST;
    }

    for (int i = 0; i < gp->y * gp->x; i++)
    {
        if (fgets(buf, MAX_LENGTH, inf) == NULL)
        {
            fprintf(stderr, "Nie udało się wczytać listy sąsiedstwa dla wierzchołka %d. Przerywam działanie\n", i);
            return ERROR_READ_LIST;
        }

        if ((gp->w[i] = malloc(8 * sizeof *gp->w[i])) == NULL) // czy udało się zaalokować pamięć dla 1 wierzchołka
        {
            fprintf(stderr, "Nie udało się zaalokować pamięci na listę sąsiedstwa dla wierzchołka %d. Przerywam działanie.\n", i);

            return ERROR_ALLOC_NODE_LIST;
        }

        counter = 0;

        for (int j = 0; j < strlen(buf); j++) // liczy, ile par danych jest w jednej linii
            if (buf[j] == ':')
                counter++;

        if (counter > 4)
        {
            fprintf(stderr, "Linia %d: W linii znajduje się powyżej 4 sąsiadujących wierzchołków: %d. Dane powyżej limitu zostaną pominięte.\n", i + 2, counter);
        }

        strstream = fmemopen(buf, strlen(buf), "r");
        strstream = NULL;

        if (strstream == NULL)
        {
            fprintf(stderr, "Nie udało się wczytać listy sąsiedstwa dla wierzchołka %d. Przerywam działanie\n", i);

            return ERROR_READ_LIST;
        }

        edge = 0;

        for (int j = 0; j < counter; j++)
        {
            if (edge >= 8) // przerywa, gdy nie ma już miejsce na wstawienie
                break;

            czy_znaleziono = 0;

            if (fscanf(strstream, "%d", &tmp1) != 1)
            {
                fprintf(stderr, "Linia %d: Nie udało się wczytać numeru wierzchołka. Przerywam działanie.\n", i + 2);

                return ERROR_BAD_LIST_FORMAT;
            }

            if (tmp1 < 0 || tmp1 >= gp->x * gp->y) // poza zakresem <0;liczba wierzchołków)
            {
                fprintf(stderr, "Linia %d: Wczytano nieprawidłowy indeks wierzchołka: %d. Przechodzę do następnych danych.\n", i + 2, tmp1);
                continue;
            }

            if (!czy_sasiaduja(i, tmp1, gp->y, gp->x)) // sprawdza, czy mogą ze sobą sąsiadować
            {
                fprintf(stderr, "Linia %d: Wykryto nieprawidłowe połączenie między wierzchołkami %d i %d. Połączenie zostaje pominięte.\n", i + 2, i, tmp1);
                continue;
            }

            while ((ch = fgetc(strstream)) == ' ') // pomija whitespace
                ;

            if (ch != ':') // zły format, nie ma ':' po whitespace
            {
                fprintf(stderr, "Linia %d: Błędny format pliku - wykryto znak \'%c\'. Przerywam działanie.\n", i + 2, ch);

                return ERROR_BAD_LIST_FORMAT;
            }

            if (fscanf(strstream, "%lf", &tmp2) != 1)
            {
                fprintf(stderr, "Linia %d: Nie udało się wczytać wartości wagi. Przerywam działanie.\n", i + 2);

                return ERROR_BAD_LIST_FORMAT;
            }

            if (tmp2 <= 0) // waga <= 0
            {
                fprintf(stderr, "Linia %d: Wczytano nieprawidłową wagę: %f. Przechodzę do następnych danych.\n", i + 2, tmp2);
                continue;
            }

            for (int wi = 0; wi < edge / 2; wi++)
            {
                if (tmp1 == wierz[wi]) // jeżeli w tej linii już był ten wierzchołek
                {
                    fprintf(stderr, "Linia %d: Połączenie między wierzchołkami %d i %d zostało już zdefiniowane. Przechodzę do następnych danych.\n", i + 2, tmp1, wierz[wi]);
                    czy_znaleziono = 1;
                    break;
                }
            }

            if (czy_znaleziono) // pomija połączenie, jeśli już jakieś jest dla tego wierzchołka
                continue;

            if (gp->min == DEFAULT_VALUE || tmp2 < gp->min) // wyznaczanie najmniejszej wagi do zakresu
                gp->min = tmp2;

            if (gp->max == DEFAULT_VALUE || tmp2 > gp->max) // wyznaczanie największej wagi do zakresu
                gp->max = tmp2;

            gp->w[i][edge] = (double)tmp1;
            gp->w[i][edge + 1] = tmp2;

            wierz[edge / 2] = tmp1;

            edge += 2;
        }

        if (edge < 8)
        {
            gp->w[i][edge] = LIST_EDGE;

            if ((gp->w[i] = realloc(gp->w[i], (edge + 1) * sizeof *gp->w[i])) == NULL)
            {
                fprintf(stderr, "Nie udało się realokować pamięci na listę wierzchołka %d. Przerywam działanie.\n", i);

                return ERROR_ALLOC_NODE_LIST;
            }
        }

        fclose(strstream);
    }

    return 1;
}

void zainicjalizuj_graf(graph_t *gp)
{
    gp->x = DEFAULT_VALUE;
    gp->y = DEFAULT_VALUE;
    gp->n = DEFAULT_VALUE;
    gp->min = DEFAULT_VALUE;
    gp->max = DEFAULT_VALUE;
    gp->w = NULL;
}

void zapisz_graf(FILE *ouf, graph_t *gp)
{
    int n = (gp->x) * (gp->y); // liczba wierzchołków

    fprintf(ouf, "%d %d\n", gp->y, gp->x);

    if (gp->w == NULL) // jeżeli tablica jest pusta jakimś cudem
    {
        for (int i = 0; i < n; i++)
            fprintf(ouf, "\n");

        return;
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(ouf, "\t ");

        for (int j = 0; j < 8; j += 2) // max zakres indeksów to 0-7
        {
            if (gp->w[i][j] == LIST_EDGE) // przerywa pętlę, jeżeli nie ma następnego sąsiada
                break;

            fprintf(ouf, "%d :%lf  ", (int)gp->w[i][j], gp->w[i][j + 1]);
        }

        fprintf(ouf, "\n");
    }
}

void zwolnij_graf(graph_t *gp)
{
    int n = (gp->x) * (gp->y); // liczba wierzchołków

    for (int i = 0; i < n; i++)
        if (gp->w[i] != NULL)
            free(gp->w[i]);

    if (gp->w != NULL)
        free(gp->w);
}

int sprawdz_integralnosc(graph_t *gp)
{
    if (gp->w == NULL) // gdyby jakimś cudem tablica była pusta
        return 0;

    int w2;             // indeks drugiego wierzchołka (w1 to pierwszy)
    double waga;        // do przechowywania wagi
    int czy_znaleziono; // sprawdza czy znaleziono połączenie gdy przeszło po 4 sąsiedzie i nadal nie znalazło

    for (int w1 = 0; w1 < gp->x * gp->y; w1++)
    {
        for (int i = 0; i < 8; i += 2)
        {
            if (gp->w[w1][i] == LIST_EDGE)
                break;

            w2 = gp->w[w1][i];
            waga = gp->w[w1][i + 1];
            czy_znaleziono = 0;

            for (int j = 0; j < 8; j += 2) // TODO: jeżeli doszło do 8 i nadal nie znalazło
            {
                if (gp->w[w2][j] == LIST_EDGE) // jeżeli doszło do końca listy i nie było w1
                {
                    fprintf(stderr, "Nie znaleziono obustronnego połączenia między wierzchołkami %d i %d. Przerywam działanie.\n", w1, w2);
                    return 0;
                }

                if (gp->w[w2][j] == w1) // jest połączenie obustronne w1 -- w2
                {
                    if (gp->w[w2][j + 1] == waga)
                    {
                        czy_znaleziono = 1;
                        break;
                    }
                    else
                    {
                        fprintf(stderr, "Wykryto niezgodność wag na krawędzi między wierzchołkami %d i %d: %g a %g. Przerywam działanie.\n", w1, w2, gp->w[w2][j + 1], waga);
                        return 0;
                    }
                }
            }

            if (!czy_znaleziono) // jeżeli wsród 4 sąsiadów nie znaleziono połączenia
            {
                fprintf(stderr, "Nie znaleziono obustronnego połączenia między wierzchołkami %d i %d. Przerywam działanie.\n", w1, w2);
                return 0;
            }
        }
    }

    return 1;
}

int znajdz_droge_bfs(graph_t *gp, int st, int sp)
{
    int *czy_odwiedzono = calloc(gp->x * gp->y, sizeof *czy_odwiedzono); // czy odwiedzono wierzchołek podczas BFS (0 - nie, 1 - tak)
    queue_t *kolejka = zainicjalizuj_kolejke(gp->x * gp->y);

    if (czy_odwiedzono == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na tablicę odwiedzonych wierzchołków. Przerywam działanie.\n");

        exit(ERROR_ALLOC_VISITED);
    }

    dodaj_element(kolejka, st);
    czy_odwiedzono[st] = 1;
    int tmp;

    while (!czy_pusta(kolejka)) // wykonuj, dopóki w kolejce są elementy
    {
        tmp = usun_element(kolejka);

        for (int i = 0; i < 8; i += 2)
        {
            if (gp->w[tmp][i] == LIST_EDGE)
                break;

            if (!czy_odwiedzono[(int)gp->w[tmp][i]])
            {
                dodaj_element(kolejka, (int)gp->w[tmp][i]);

                czy_odwiedzono[(int)gp->w[tmp][i]] = 1;
            }
        }
    }

    tmp = czy_odwiedzono[sp]; // jeżeli odwiedzono SP zaczynąc z ST, to zwraca 1, w przeciwnym wypadku 0
    free(czy_odwiedzono);
    free(kolejka->queue);
    free(kolejka);

    return tmp;
}

void wyznacz_n_siatki(graph_t *gp)
{
    int *czy_odwiedzono = calloc(gp->x * gp->y, sizeof *czy_odwiedzono); // czy odwiedzono wierzchołek podczas BFS (0 - nie, 1 - tak)

    if (czy_odwiedzono == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na tablicę odwiedzonych wierzchołków. Przerywam działanie.\n");

        exit(ERROR_ALLOC_VISITED);
    }

    queue_t *kolejka = zainicjalizuj_kolejke(gp->x * gp->y);
    int w = 0; // wierzchołek od którego zaczynamy szukać
    gp->n = 0;
    int czy_szukac = 1; // czy są jeszcze jakieś nieodwiedzone wierzchołki
    int tmp;

    while (czy_szukac)
    {
        czy_szukac = 0;
        gp->n++;

        dodaj_element(kolejka, w);
        czy_odwiedzono[w] = 1;

        while (!czy_pusta(kolejka)) // wykonuj, dopóki w kolejce są elementy
        {
            tmp = usun_element(kolejka);

            for (int i = 0; i < 8; i += 2)
            {
                if (gp->w[tmp][i] == LIST_EDGE)
                    break;

                if (!czy_odwiedzono[(int)gp->w[tmp][i]])
                {
                    dodaj_element(kolejka, (int)gp->w[tmp][i]);

                    czy_odwiedzono[(int)gp->w[tmp][i]] = 1;
                }
            }
        }

        for (int i = 0; i < gp->x * gp->y; i++)
        {
            if (!czy_odwiedzono[i])
            {
                czy_szukac = 1;
                w = i;
                break;
            }
        }
    }

    free(czy_odwiedzono);
    free(kolejka->queue);
    free(kolejka);
}

void wyswietl_sciezke(int *poprzednik, int w)
{
    if (poprzednik[w] != DEFAULT_VALUE) // dopóki są poprzednicy to idź dalej do tyłu
        wyswietl_sciezke(poprzednik, poprzednik[w]);

    printf("%d ", w);
}

void znajdz_droge(graph_t *gp, int st, int sp)
{
    int *czy_odwiedzono = calloc(gp->x * gp->y, sizeof *czy_odwiedzono); // czy odwiedzono wierzchołek podczas BFS (0 - nie, 1 - tak)

    if (czy_odwiedzono == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na tablicę odwiedzonych wierzchołków. Przerywam działanie.\n");

        exit(ERROR_ALLOC_VISITED);
    }

    queue_t *kolejka = zainicjalizuj_kolejke(gp->x * gp->y);
    prqueue_t *kolejka_prio = zainicjalizuj_kolejke_pr(gp->x * gp->y);

    dodaj_element(kolejka, st);
    czy_odwiedzono[st] = 1;
    int tmp, tmp2;

    while (!czy_pusta(kolejka)) // wykonuj, dopóki w kolejce są elementy
    {
        tmp = usun_element(kolejka);

        for (int i = 0; i < 8; i += 2)
        {
            if (gp->w[tmp][i] == LIST_EDGE)
                break;

            if (!czy_odwiedzono[(int)gp->w[tmp][i]])
            {
                dodaj_element(kolejka, (int)gp->w[tmp][i]);

                czy_odwiedzono[(int)gp->w[tmp][i]] = 1;
            }
        }
    }

    for (int i = 0; i < gp->x * gp->y; i++)
        if (czy_odwiedzono[i]) // jeżeli odwiedzono w tym podgrafie, to dodaj do kolejki priorytetowej
            dodaj_element_pr(kolejka_prio, i);

    free(czy_odwiedzono);
    free(kolejka->queue);
    free(kolejka);

    // teraz w kolejce są wszystkie wierzchołki z podgrafu, to można szukać Dijkstrą

    double *dyst = malloc((gp->x * gp->y) * sizeof *dyst);                   // odległość wierzchołków od ST
    int *poprzednik = malloc((gp->x * gp->y) * sizeof *dyst);                // przechowuje poprzednika do wyznaczenia drogi
    int *czy_przetworzono = calloc(gp->x * gp->y, sizeof *czy_przetworzono); // określa, czy dany wierzchołek został już przetworzony; 0 - nie, 1 - tak

    if (dyst == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na tablicę odległości. Przerywam działanie,\n");

        exit(ERROR_ALLOC_DIST);
    }

    if (poprzednik == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na tablicę poprzedników. Przerywam działanie,\n");

        exit(ERROR_ALLOC_PREV);
    }

    if (czy_przetworzono == NULL)
    {
        fprintf(stderr, "Nie udało się zaalokować pamięci na tablicę przetworzonych wierzchołków. Przerywam działanie,\n");

        exit(ERROR_ALLOC_PROC);
    }

    for (int i = 0; i < gp->x * gp->y; i++)
    {
        dyst[i] = DBL_MAX; // ustawiamy odległości dla wszystkich wierzchołków jako "nieskończoność"
    }

    dyst[st] = 0;                   // dystans ST od ST jest równy 0
    poprzednik[st] = DEFAULT_VALUE; // i nie ma on poprzednika

    while (!czy_pusta_pr(kolejka_prio))
    {
        tmp = usun_element_pr(kolejka_prio, dyst);
        czy_przetworzono[tmp] = 1;

        for (int i = 0; i < 8; i += 2)
        {
            tmp2 = (int)gp->w[tmp][i]; // indeks sprawdzanego sąsiada

            if (tmp2 == LIST_EDGE)
                break;

            if (!czy_przetworzono[tmp2] && (dyst[tmp] + gp->w[tmp][i + 1] < dyst[tmp2]))
            {
                dyst[tmp2] = dyst[tmp] + gp->w[tmp][i + 1];
                poprzednik[tmp2] = tmp;
            }
        }
    }

    printf("Długość najkrótszej ścieżki między %d i %d: %g\n", st, sp, dyst[sp]);
    printf("Droga: ");
    wyswietl_sciezke(poprzednik, sp);
    printf("\n");

    free(dyst);
    free(poprzednik);
    free(czy_przetworzono);
    free(kolejka_prio->queue);
    free(kolejka_prio);
}

int generuj_graf(graph_t *G)
{
    srand(time(NULL));
    if (G->w == NULL)
        return 1;
    for (int i = 0; i < G->x * G->y; i++)
    {
        G->w[i] = malloc(8 * sizeof *G->w[i]);
        if (G->w[i] == NULL)
            return 1;
    }
    // Zapisywanie sąsiadów
    int position;
    for (int i = 0; i < G->x * G->y; i++)
    {
        position = 0;
        if (i >= G->x)
        { // Czy ma nad sobą sąsiada
            G->w[i][position] = i - G->x;
            position += 2;
        }
        if (i % G->x != 0)
        { // Czy ma po lewej sąsiada
            G->w[i][position] = i - 1;
            position += 2;
        }
        if ((i + 1) % G->x != 0)
        { // Czy ma po prawej sąsiada
            G->w[i][position] = i + 1;
            position += 2;
        }
        if (i / G->x != G->y - 1)
        { // Czy ma pod sobą sąsiada
            G->w[i][position] = i + G->x;
            position += 2;
        }
        if (position < 8)
        {
            G->w[i][position] = -1.0; // Flaga końcowa
            if ((G->w[i] = realloc(G->w[i], (position + 1) * sizeof *G->w[i])) == NULL)
                return 1;
        }
    }
    // Zapisywanie wag
    for (int i = 0; i < G->x * G->y; i++)
    {
        position = 1;
        if (i >= G->x)
        { // Czy ma nad sobą sąsiada |Przepisuje|
            G->w[i][position] = G->w[i - G->x][szukaj_wierzcholek(i - G->x, i, G) + 1];
            position += 2;
        }
        if (i % G->x != 0)
        { // Czy ma po lewej sąsiada |Przepisuje|
            G->w[i][position] = G->w[i - 1][szukaj_wierzcholek(i - 1, i, G) + 1];
            position += 2;
        }
        if ((i + 1) % G->x != 0)
        { // Czy ma po prawej sąsiada |Losuje|
            G->w[i][position] = losuj(G->min, G->max);
            position += 2;
        }
        if (i / G->x != G->y - 1)
        { // Czy ma pod sobą sąsiada |Losuje|
            G->w[i][position] = losuj(G->min, G->max);
        }
    }

    return 0; // Jeśli wszystko poprawne
}

// Szuka wierzchołka w tablicy sąsiedztwa innego wierzchołka
int szukaj_wierzcholek(int edge, int seek, graph_t *G)
{
    int i = 0;
    if (seek < 0)
        return -1;
    while (G->w[edge][i] != (double)seek && i < 8)
        i += 2;
    return i < 8 ? i : -1;
}

// Losuje liczbę z podanego zakresu
double losuj(double min, double max)
{
    return (double)rand() / RAND_MAX * (max - min) + min;
}

// Funkcja dzieląca jeden graf na dwa
int dziel_graf(graph_t *G)
{
    int r, p;       // Zmienna losowa
    int size = 100; // Wielkość zaalokowanej pamięci
    int n = 0;      // Długość ścieżki
    int *trail;
    if ((trail = malloc(size * sizeof *trail)) == NULL) //ścieżka punktów
        return 0;
    do                                                              // Robi to dopóki, nie wylosuje wierzchołka, który nie ma 4 sąsiadów, ale ma chociaż jednego
        r = ((int)losuj(0, G->y)) * (G->x) + ((int)losuj(0, G->x)); // Losowanie z całego zakresu wierzchołków
    while (ile_sasiadow(G, r) == 4 || ile_sasiadow(G, r) == 0);
    trail[n++] = r;
    int i;
    int j;
    do
    {
        p = G->w[r][(int)losuj(0, ile_sasiadow(G, r)) * 2];
        // sprawdzanie, czy wierzchołek się nie powtórzył
        for (i = 0; i < n; i++)
            if (trail[i] == p)
                break;
        // Jeśli nie, dodaje
        if (i == n)
        {
            trail[n++] = p;
            r = p;
            if (n == size)
            {
                size *= 2;
                if ((trail = realloc(trail, size * sizeof *trail)) == NULL)
                    return 0; // Błąd
            }
        }
    } while (ile_sasiadow(G, r) == 4);
    if (n == 2)
    {                                                                   // Jeśli są tylko dwa wierzchołki, czyli pętla skończyła działanie po jednym wykonaniu
        if (ile_sasiadow(G, trail[0]) + ile_sasiadow(G, trail[1]) == 2) // Wierzchołki są połączone tylko ze sobą
            return zerwanie_polaczenia(G, trail[0], trail[1]);
        else
        {
            // Sprawdzanie połączeń
            for (i = 0; i < 2; i++)
            {
                for (j = 0; j < ile_sasiadow(G, trail[i]); j++)
                {
                    if (ile_sasiadow(G, G->w[trail[i]][j * 2]) == 1) // Czy jeden z wierzchołków jest jedynym połączeniem z jakimś wierzchołkiem
                        return zerwanie_polaczenia(G, trail[i], G->w[trail[i]][j * 2]);
                }
            }
            // Oderwanie tych dwóch wierzchołków nie spowoduje podzielenia grafu na więcej niż 2, odrywam połączenia zewnętrzne
            // Pierwszy wierzchołek
            while (ile_sasiadow(G, trail[0]) != 1)
            {
                if (G->w[trail[0]][0] != trail[1])
                    if (!zerwanie_polaczenia(G, trail[0], G->w[trail[0]][0]))
                        return 0;
                    else if (!zerwanie_polaczenia(G, trail[0], G->w[trail[0]][2]))
                        return 0;
            }
            // Drugi wierzchołek
            while (ile_sasiadow(G, trail[1]) != 1)
            {
                if (G->w[trail[1]][0] != trail[0])
                    if (!zerwanie_polaczenia(G, trail[1], G->w[trail[1]][0]))
                        return 0;
                    else if (!zerwanie_polaczenia(G, trail[1], G->w[trail[1]][2]))
                        return 0;
            }
        }
    }
    else
    {
        // Sprawdzanie połączeń
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < ile_sasiadow(G, trail[i]); j++)
            {
                if (ile_sasiadow(G, G->w[trail[i]][j * 2]) == 1) // Czy jeden z wierzchołków jest jedynym połączeniem z jakimś wierzchołkiem
                    return zerwanie_polaczenia(G, trail[i], G->w[trail[i]][j * 2]);
            }
        }
        // Jeśli wszystko dobrze, to tnij wzdłuż ścieżki
        for (i = 0; i < n; i++)
        {
            j = 0;
            while (j < ile_sasiadow(G, trail[i]))
            {
                // Tnie lewe albo dolne połączenie
                if (szukaj_wierzcholek(trail[i], i % G->x != 0, G) != -1 || szukaj_wierzcholek(trail[i], trail[i] / G->x != G->y - 1, G) != -1)
                    zerwanie_polaczenia(G, trail[i], G->w[trail[i]][j * 2]);
                else
                    j++;
            }
        }
    }
    free(trail);
    return 1;
}
// Funkcja zrywa połączenie miedzy dwoma wierzchołkami
int zerwanie_polaczenia(graph_t *G, int q, int p)
{
    int x, y; // Indeksy połączeń
    int n, m; // Liczba sąsiadów
    x = szukaj_wierzcholek(q, p, G);
    y = szukaj_wierzcholek(p, q, G);
    if (x < 0 || y < 0)
        return 0;
    n = ile_sasiadow(G, q); // Ile sąsiadów q
    m = ile_sasiadow(G, p); // Ile sąsiadów p
    // Przesuwanie tablicy wierzchołka q
    for (int i = x; i < n * 2; i += 2)
    {
        G->w[q][i] = G->w[q][i + 2];
        G->w[q][i + 1] = G->w[q][i + 3];
    }
    G->w[q][n * 2 - 1] = -1.0;
    // Przesuwanie tablicy wierzchołka p
    for (int i = x; i < m * 2; i += 2)
    {
        G->w[p][i] = G->w[p][i + 2];
        G->w[p][i + 1] = G->w[p][i + 3];
    }
    G->w[q][m * 2 - 1] = -1.0;
    return (G->w[q] = realloc(G->w[q], (n * 2 + 1) * sizeof *G->w[q])) == NULL || (G->w[p] = realloc(G->w[p], (m * 2 + 1) * sizeof *G->w[p])) == NULL;
}

// Liczy ile sąsiadów ma wierzchołek
int ile_sasiadow(graph_t *G, int edge)
{
    int n = 0;

    while (n < 4 && G->w[edge][n * 2] != -1.0)
        n++;

    return n;
}
