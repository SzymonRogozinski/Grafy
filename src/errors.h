#ifndef _ERRORS_H_
#define _ERRORS_H_

#define ERROR_UNKNOWN_PARAM -1 // podano nieznany parametr
#define ERROR_BAD_PARAM -2     // podano bezparametrowy argument wywołania
#define ERROR_NODES_FORMAT -3  // nieprawidłowy format parametru wierzchołków ścieżki
#define ERROR_RANGE_FORMAT -4  // nieprawidłowy format parametru zakresu wag
#define ERROR_OPEN_FILE -5     // nie udało się otworzyć pliku wejściowego

#define ERROR_ALLOC_GRAPH -10      // błąd alokacji pamięci na strukturę grafu
#define ERROR_ALLOC_GRAPH_LIST -11 // błąd alokacji pamięci na listę sąsiedztwa grafu
#define ERROR_ALLOC_QUEUE -12      // błąd alokacji pamięci na strukturę kolejki FIFO
#define ERROR_ALLOC_Q_ARRAY -13    // błąd alokacji pamięci na tablicę elementów kolejki FIFO
#define ERROR_ALLOC_PRQUEUE -14    // błąd alokacji pamięci na strukturę kolejki priorytetowej
#define ERROR_ALLOC_PRQ_ARRAY -15  // błąd alokacji pamięci na tablicę elementów kolejki priorytetowej
#define ERROR_ALLOC_VISITED -16    // błąd alokacji pamięci na tablicę odwiedzonych wierzchołków przez BFS
#define ERROR_ALLOC_DIST -17       // błąd alokacji pamięci na tablicę odległości od wierzchołka początkowego
#define ERROR_ALLOC_PREV -18       // błąd alokacji pamięci na tablicę poprzedników do algorytmu Dijkstry
#define ERROR_ALLOC_PROC -19       // błąd alokacji pamięci na tablicę przetworzonych wierzchołków przez algorytm Dijkstry

#define ERROR_NEG_PATH_NODES -20       // ujemne indeksy wierzchołków ścieżki
#define ERROR_BAD_WEIGHT_RANGE -21     // zakres wartości wag nie spełnia kryteriów
#define ERROR_NEG_COLUMN -22           // ujemna liczba kolumn
#define ERROR_NEG_ROW -23              // ujemna liczba wierszy
#define ERROR_NEG_SUBGRAPH -24         // ujemna liczba grafów w siatce
#define ERROR_UNDEFINED_PATH_NODES -25 // nie zdefiniowano wierzchołków ścieżki
#define ERROR_SUBGRAPH_OVER_NODE -26   // podano liczbę grafów w siatce większą od liczby wierzchołków
#define ERROR_PATH_OUT_OF_RANGE -27    // indeksy wierzchołków ścieżki poza dostępnym zakresem

#define ERROR_NO_AVAILABLE_PATH -30 // nie ma ścieżki między wierzchołkami ST i SP
#define ERROR_GRAPH_INTEGRITY -31   // połączenia w grafie nie są integralne

#define ERROR_READ_DIMENSIONS -40 // nie udało się wczytać wymiarów grafu z pliku
#define ERROR_NEG_DIMENSIONS -41  // wczytano z pliku ujemne wymiary grafu
#define ERROR_ALLOC_NODE_LIST -42 // błąd alokacji pamięci na listę sąsiedztwa dla wierzchołka
#define ERROR_READ_LIST -43       // nie udało się wczytać listy dla wierzchołka
#define ERROR_BAD_LIST_FORMAT -44 // zły format listy sąsiedztwa dla wierzchołka
#define ERROR_QUEUE_FULL -45      // przepełnienie kolejki
#define ERROR_PRQUEUE_FULL -46    // przepełnienie kolejki priorytetowej

#define ERROR_ALLOC_TRAIL -50 // błąd alokacji pamięci na ścieżkę punktów do odcięcia

#endif