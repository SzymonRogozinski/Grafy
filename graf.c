#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "graf.h"

int wczytaj_graf(FILE * F, GraF* gp) {
	int a, b;
	char* buf = malloc(32*sizeof(buf)); //Bufor
	char c; //wczytywanie znak�w
	double g; //Przerzut na double
	int len = 0; //D�ugo�� buforu
	int i = 0; //Ile wierzcho�k�w wczytano
	if (fscanf(F,"%d %d", &a, &b) != 2)
		return 1;
	//Wczytywanie wymiar�w
	gp->x = b;
	gp->y = a;
	//alokowanie pami�ci
	gp->w = malloc(a*b*sizeof(gp->w));
	gp->w[0] = malloc(9 * sizeof(gp->w[0]));
	//Wczytanie pierwszego elementu, robi� poza p�tl�, przez pocz�tkowe znaki bia�e
	if(fscanf(F,"%lf",&g)!=1)
		return 1;
	gp->w[0] = &g;
	printf("%lf\n", gp->w[0][0]);
	gp->w[0]++;

	while((c=fgetc(F))!= EOF) {
		if (isalnum(c) || c == '.') { //Wczytuje liczb� do bufora
			*(buf + len) = c;
			len++;
		}
		else if (c == '\n') { //Zwi�ksza index wierzcho�ka i oznacza koniec jako -1
			g=-1;
			gp->w[i] = &g;
			i++;
		}
		else if(len!=0) { //Dodaje liczb� do w
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
