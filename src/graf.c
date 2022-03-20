#include <stdlib.h>
#include <string.h>

#include "graf.h"

#define MAX_LENGTH 128

int czy_sasiaduja(int w1, int w2, int w, int k)
{
	if (w1 >= k && w1 - k == w2) // w1 nie jest w 1. wierszu i sąsiaduje z górnym
		return 1;

	if (w1 < k * (w - 1) && w1 + k == w2) // w1 nie jest w ostatnim wierszu i sąsiaduje z dolnym
		return 1;

	if (w1 % k != 0 && w1 - 1 == w2) // w1 nie jest w pierwszej kolumnie i sąsiaduje z lewym
		return 1;

	if ((w1 - w) % k != 0 && w1 + 1 == w2) // w1 nie jest w ostatniej kolumnie i sąsiaduje z prawym
		return 1;

	return 0;
}

// TODO: walidacja danych
// - liczba spójnych grafów
// - spójność danych: czy graf jest nieskierowany, czy dwa sasiędnie wierzchołki nawzajem się zawierają
int wczytaj_graf(FILE *inf, graph_t *gp)
{
	char buf[MAX_LENGTH]; // bufor
	int counter;		  // ile sąsiadów w jednym linii
	int tmp1, ch;
	double tmp2; // zmienna do wczytywania danych
	FILE *strstream;

	int edge; // liczba wczytanych danych

	if (fgets(buf, MAX_LENGTH, inf) != NULL) // fgets, bo nie wiadomo jak się zachowa po fscanf -> fgets
	{
		if (sscanf(buf, "%d %d\n", &gp->y, &gp->x) != 2)
		{
			fprintf(stderr, "Nie udało się wczytać wymiarów grafu. Przerywam działanie.\n");
			return 1;
		}
	}
	else
	{
		fprintf(stderr, "Nie udało się wczytać wymiarów grafu. Przerywam działanie.\n");
		return 1;
	}

	if (gp->y <= 0 || gp->x <= 0) // warunek X > 0, Y > 0
	{
		if (gp->y <= 0)
			fprintf(stderr, "Liczba wierszy musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", gp->y);

		if (gp->x <= 0)
			fprintf(stderr, "Liczba kolumn musi być większa od zera – wczytano “%d”. Przerywam działanie.\n", gp->x);

		return 1;
	}

	if ((gp->w = malloc(gp->y * gp->x * sizeof *gp->w)) == NULL) // alokowanie pamięci na wierzchołki
	{
		fprintf(stderr, "Nie udało się zaalokować pamięci na listę sąsiedstwa. Przerywam działanie.\n");
		return 1;
	}

	for (int i = 0; i < gp->y * gp->x; i++)
	{
		if (fgets(buf, MAX_LENGTH, inf) == NULL)
		{
			fprintf(stderr, "Nie udało się wczytać listy sąsiedstwa dla wierzchołka %d. Przerywam działanie\n", i);
			return 1;
		}

		if ((gp->w[i] = malloc(8 * sizeof *gp->w[i])) == NULL) // czy udało się zaalokować pamięć dla 1 wierzchołka
		{
			fprintf(stderr, "Nie udało się zaalokować pamięci na listę sąsiedstwa dla wierzchołka %d. Przerywam działanie.\n", i);
			return 1;
		}

		counter = 0;

		for (int j = 0; j < strlen(buf); j++) // liczy, ile par danych jest w jednej linii
			if (buf[j] == ':')
				counter++;

		if (counter > 4)
		{
			fprintf(stderr, "W linii znajduje się powyżej 4 sąsiadujących wierzchołków: %d. Dane powyżej limitu zostaną pominięte.\n", counter);
		}

		strstream = fmemopen(buf, strlen(buf), "r");

		if (strstream == NULL)
		{
			fprintf(stderr, "Nie udało się wczytać listy sąsiedstwa dla wierzchołka %d. Przerywam działanie\n", i);
			return 1;
		}

		edge = 0;

		for (int j = 0; j < counter; j++)
        {
            if (edge >= 8) // przerywa, gdy nie ma już miejsce na wstawienie
                break;

            if (fscanf(strstream, "%d", &tmp1) != 1)
            {
                fprintf(stderr, "Linia %d: Nie udało się wczytać numeru wierzchołka. Przerywam działanie.\n", i + 2);
                return 1;
            }

            if (tmp1 < 0 || tmp1 >= gp->x * gp->y) // poza zakresem <0;liczba wierzchołków)
            {
                fprintf(stderr, "Linia %d: Wczytano nieprawidłowy indeks wierzchołka: %d. Przechodzę do następnych danych.\n", i + 2, tmp1);
                continue;
            }

            if (czy_sasiaduja(i, tmp1, gp->y, gp->x) == 0) // sprawdza, czy mogą ze sobą sąsiadować
            {
                fprintf(stderr, "Wykryto nieprawidłowe połączenie między wierzchołkami %d i %d. Połączenie zostaje pominięte.\n", i, tmp1);
                continue;
            }

            while ((ch = fgetc(strstream)) == ' ') // pomija whitespace
                ;

            if (ch != ':') // zły format, nie ma ':' po whitespace
            {
                fprintf(stderr, "Linia %d: Błędny format pliku - wykryto znak \'%c\'. Przerywam działanie.\n", i + 2, ch);
                return 1;
            }

            if (fscanf(strstream, "%lf", &tmp2) != 1)
            {
                fprintf(stderr, "Linia %d: Nie udało się wczytać wartości wagi. Przerywam działanie.\n", i + 2);
                return 1;
            }

            if (tmp2 <= 0) // waga <= 0
            {
                fprintf(stderr, "Linia %d: Wczytano nieprawidłową wagę: %f. Przechodzę do następnych danych.\n", i + 2, tmp2);
                continue;
            }

            gp->w[i][edge] = (double) tmp1;
            gp->w[i][edge + 1] = tmp2;

            edge += 2;
        }

        if (edge < 8)
        {
            gp->w[i][edge] = -1;

            if ((gp->w[i] = realloc(gp->w[i], (edge + 1) * sizeof *gp->w[i])) == NULL)
            {
                fprintf(stderr, "Nie udało się realokować pamięci na listę. Przerywam działanie.\n");
                return 1;
            }
        }

	return 0;
}

void zainicjalizuj_graf(graph_t *gp)
{
	gp->x = -1;
	gp->y = -1;
	gp->n = -1;
	gp->min = -1;
	gp->max = -1;
}

void zapisz_graf(FILE *ouf, graph_t *gp)
{
	int n = (gp->x) * (gp->y); // liczba wierzchołków

	fprintf(ouf, "%d %d\n", gp->y, gp->x);

	for (int i = 0; i < n; i++)
	{
		fprintf(ouf, "\t ");

		for (int j = 0; j < 8; j += 2) // max zakres indeksów to 0-7
		{
			if (gp->w[i][j] == -1) // przerywa pętlę, jeżeli nie ma następnego sąsiada
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