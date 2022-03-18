#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "graf.h"

#define max_lenght 128

// Funkcja wczytuje graf do struktury
int wczytaj_graf(FILE *F, graph_t *gp)
{
	int a, b;
	char *buf = malloc(max_lenght * sizeof(buf)); // Bufor
	double l, w, r, y;							  // Zmienne do wczytywania double
	int g, q, e, t;
	int edge = 0; // Liczba wczytanych danych
	int i = 0;	  // Ile wierzcho�k�w wczytano
	if (fscanf(F, "%d %d\n", &a, &b) != 2)
		return 1;
	// Wczytywanie wymiar�w
	gp->x = b;
	gp->y = a;
	// alokowanie pami�ci, rzutuje na typ, bo wcze�niej nie dzia�a�o XD
	gp->w = malloc(a * b * sizeof(double *));
	while (fgets(buf, max_lenght, F) != NULL && i < a * b)
	{
		if ((gp->w[i] = malloc(9 * sizeof(double))) == NULL) // Sprawdzanie czy otrzymano wska�nik
			return 1;
		if (sscanf(buf, "%d :%lf  %d :%lf  %d :%lf  %d :%lf", &g, &l, &q, &w, &e, &r, &t, &y) == 8)
		{ // Podano 4 pary
			gp->w[i][0] = g;
			gp->w[i][1] = l;
			gp->w[i][2] = q;
			gp->w[i][3] = w;
			gp->w[i][4] = e;
			gp->w[i][5] = r;
			gp->w[i][6] = t;
			gp->w[i][7] = y;
			gp->w[i][8] = -1;
			edge = 9;
		}
		else if (sscanf(buf, "%d :%lf  %d :%lf  %d :%lf", &g, &l, &q, &w, &e, &r) == 6)
		{ // Podano 3 pary
			gp->w[i][0] = g;
			gp->w[i][1] = l;
			gp->w[i][2] = q;
			gp->w[i][3] = w;
			gp->w[i][4] = e;
			gp->w[i][5] = r;
			gp->w[i][6] = -1;
			edge = 7;
		}
		else if (sscanf(buf, "%d :%lf  %d :%lf", &g, &l, &q, &w) == 4)
		{ // Podano 2 pary
			gp->w[i][0] = g;
			gp->w[i][1] = l;
			gp->w[i][2] = q;
			gp->w[i][3] = w;
			gp->w[i][4] = -1;
			edge = 5;
		}
		else if (sscanf(buf, "%d :%lf", &g, &l) == 2)
		{ // Podano 1 par�
			gp->w[i][0] = g;
			gp->w[i][1] = l;
			gp->w[i][2] = -1;
			edge = 3;
		}
		else // Brak danych lub niepoprawny format
			return 1;
		if ((gp->w[i] = (double *)realloc(gp->w[i], (edge) * sizeof(double))) == NULL) // Obci�cie pami�ci
			return 1;
		i++;
	}
	if (i < a * b) // Wymiar jest wi�kszy ni� liczba wierzcho�k�w
		return 1;
	free(buf);
	return 0;
}
// Funkcja zapisuje graf do pliku
void zapisz_graf(FILE *F, graph_t *gp)
{
	int n = (gp->x) * (gp->y); // Liczba wierzcho�k�w
	int j;
	fprintf(F, "%d %d\n", gp->x, gp->y);
	for (int i = 0; i < n; i++)
	{
		j = 0;
		fprintf(F, "\t ");
		while (gp->w[i][j] != -1)
		{
			fprintf(F, "%d :%lf ", (int)gp->w[i][j], gp->w[i][j + 1]);
			j += 2;
		}
		fprintf(F, "\n");
	}
}

void free_graf(graph_t *gp)
{
	int n = (gp->x) * (gp->y); // liczba wierzchołków

	for (int i = 0; i < n; i++)
		if (gp->w[i] != NULL)
			free(gp->w[i]);

	if (gp->w != NULL)
		free(gp->w);
}