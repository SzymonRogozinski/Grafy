# Grafy

Projekt z przedmiotu JIMP2 w semestrze 2022L napisany w języku C.

## Kompilowanie programu

```bash
make
```

lub

```bash
cc -o sgraph src/main.c src/graf.c src/queue.c src/prqueue.c
```

## Uruchamianie programu

```bash
./sgraph [argumenty wywołania]
```

Parametry programu:

- wymagane
  - `-s [wierzchołek początkowy],[wierzchołek końcowy]`

- opcjonalne
  - `-x [liczba kolumn w grafie]`
  - `-y [liczba wierszy w grafie]`
  - `-n [liczba podgrafów w siatce]`
  - `-r [minimalna waga],[maksymalna waga]`
  - `-f [plik wejściowy]`
  - `-o [plik wyjściowy]`

## Przykładowe użycia

- `./sgraph -s 1,10 -o wyj.txt`
- `./sgraph -f data/graph1 -s 1,3`
- `./sgraph -x 100 -y 90 -s 21,37`
- `./sgraph -x 200 -y 130 -n 5 -s 4,340`
- `./sgraph -s 300,564 -x 10 -y 400 -n 3 -r 0-100`
- `./sgraph -s 7,94 -r 10-100`
- `./sgraph -s 10,54 -x 30 -r 0-2`
- `./sgraph -s 21,77`
- `./sgraph -s 1,55 -n 3 -x 100`
- `./sgraph -s 6,63 -n 3 -x 100 -y 400 -r 10-20 -o graf.txt`
