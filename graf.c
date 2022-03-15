#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "graf.h"

int wczytaj_graf(FILE * F, GraF* gp) {
	int a, b;
	char* buf = malloc(32*sizeof(buf)); //Bufor
	char c; //wczytywanie znaków
	double g; //Przerzut na double
	int len = 0; //D³ugoœæ buforu
	int i = 0; //Ile wierzcho³ków wczytano
	if (fscanf(F,"%d %d", &a, &b) != 2)
		return 1;
	//Wczytywanie wymiarów
	gp->x = b;
	gp->y = a;
	//alokowanie pamiêci
	gp->w = malloc(a*b*sizeof(gp->w));
	gp->w[0] = malloc(9 * sizeof(gp->w[0]));
	//Wczytanie pierwszego elementu, robiê poza pêtl¹, przez pocz¹tkowe znaki bia³e
	if(fscanf(F,"%lf",&g)!=1)
		return 1;
	gp->w[0] = &g;
	printf("%lf\n", gp->w[0][0]);
	gp->w[0]++;

	while((c=fgetc(F))!= EOF) {
		if (isalnum(c) || c == '.') { //Wczytuje liczbê do bufora
			*(buf + len) = c;
			len++;
		}
		else if (c == '\n') { //Zwiêksza index wierzcho³ka i oznacza koniec jako -1
			g=-1;
			gp->w[i] = &g;
			i++;
		}
		else if(len!=0) { //Dodaje liczbê do w
			*(buf + len) = '\0';
			g= atof(buf);
			gp->w[i] = &g;
			printf("%lf\n", gp->w[i][0]);
			gp->w[i]++;
			len = 0;
		}

	}



	return 0;
}
