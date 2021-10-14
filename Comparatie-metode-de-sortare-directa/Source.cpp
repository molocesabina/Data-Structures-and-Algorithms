/**
*@author Moloce Sabina
* 
*Compartie metodele de sortare Bubble, Insert, Selection
* 
*   1. Cazul mediu: Toti algoritmi au complexitatea O(N^2). Se observa ca algoritmul cel mai eficient in acest caz este insertSort, unde
*      T(N) = N^2/4+N*logN. Pe al doilea loc se claseaza selectSort, care desi are aceasi complexitatea a comparatiilor ca bubbleSort(O(N^2)),
*      se deosebeste de acesta prin numarul de atribuiri realizate (care cresc liniar in cazul selectSort si patratic la bubbleSort + 
*      faptul ca atribuiri sunt x3, deci mai specific bubbleSort T(N)=N^2/2 + 3*N^2/4= 5*N^2/4 si pt selectSort T(N)=N^2/2 + 3*N 
*       => se explica diferenta din grafic.
*      
*   2. Cazul favorabil (cand nr. sunt ordonate crescator): Cel mai eficient algorimt in acest caz este bubbleSort deoarece are o complexitatea
*      O(N) (avand un O(1) pt atribuiri, deaoarece se fac 0 swap-uri si O(N) comparatii). Urmatorul alogoritm ca eficienta este insertSort
*      care are complexitatae O(N*logN) (chiar daca nr. de atribuiri este O(N), se apeleaza insa binarySearch pt. fiecare element incepand cu
*      cel de-al doilea => O(N*logN)). Pe ultimul loc se claseaza selectSort care are complexitatea O(N^2), deoarece pozitia minimului se
*      determina trecand prin toata partea neordonata a sirului de fiecare data => se fac mereu acelasi nr. de comparatii in toate cazurile,
*      difera doar nr. de swap-uri care este 0 in cazul favorabil.
* 
*   3. Cazul defavorabil: Toti algoritmi au complexitatea O(N^2). Se observa ca selectSort si insertSort sunt aproximativ la egalitate si
*      sunt mult mai eficienti decat bubbleSort, fapt ce poate fi explicat astfel: 
*      -> nr de atribuiri pentru selectSort 3*N/2, iar pentru insertSort N^2/2-
*      -> nr de comparatii pentru selectSort N^2/2, iar insertSort este N*logN
*      Pentru bubbleSort numarul total de operatii este foarte aproape de 2*N^2 (nr. atribuiri 3*N^2/2 nr. de comparatii N^2/2)
* 
* Stabilitatea
*   
*    BubbleSort: Este stabil, reiese din v[j] > v[j + 1],, daca era >= atunci nu mai era stabil pt ca se putea face swap intre 2 elem egale
*                => nu-si mai pastreaza ordinea relativa
*    InsertSort: Nu este stabil din cauza binarySearch, doar cu modificarea ca sa returneze poz imediat dupa ultimul duplicat ditr-un sir
                 este stabil
*    SelectSort: Nu este stabil. Exemplu 4, 4(*), 0 => 0, 4(*), 4  
*/

#include <stdio.h>
#include <string.h>
#include "Profiler.h"

#define MAX_SIZE 1000
#define STEP_SIZE 100

Profiler p("sortari");

void swap(int* a, int* b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void bubbleSort(int v[], int n) {
    Operation opComp = p.createOperation("bubble-comp", n);
    Operation opAttr = p.createOperation("bubble-attr", n);

    int swapped = 1;
    for (int i = 0; i < n - 1 && swapped == 1; i++) {
        swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            opComp.count();
            if (v[j] > v[j + 1]) {
                swap(&v[j], &v[j + 1]);
                opAttr.count(3);
                swapped = 1;
            }
        }
    }
}

int CompBS;

int binarySearch(int v[], int low, int high, int x) {
    if (high <= low) {
        CompBS++;
        if (x > v[low])
            return low + 1;
        return low;
    }

    int m = (low + high) / 2;

    CompBS++;
    if (x == v[m])
        return m + 1;
    CompBS++;
    if (x > v[m])
        return binarySearch(v, m + 1, high, x);
    return binarySearch(v, low, m - 1, x);
}

int binarySearch_stabil(int v[], int low, int high, int x) {
    if (high <= low) {
        CompBS++;
        if (x >= v[low])
            return low + 1;
        return low;
    }

    int m = (low + high) / 2;

    CompBS++;
    if (x >= v[m])
        return binarySearch(v, m + 1, high, x);
    return binarySearch(v, low, m - 1, x);
}

void insertSort(int v[], int n) {
    Operation opComp = p.createOperation("insert-comp", n);
    Operation opAttr = p.createOperation("insert-attr", n);
    int i, j, x;
    for (i = 1; i < n; i++) {
        x = v[i];
        opAttr.count();
        j = i - 1;

        CompBS = 0;
        int poz = binarySearch_stabil(v,0, j, x);
        opComp.count(CompBS);
        while (j >= poz) {
            v[j + 1] = v[j];
            opAttr.count();
            j--;
        }

        if (j != i-1) {
            v[j + 1] = x;
            opAttr.count();
        }
     

    }
}


void selectSort(int v[], int n) {
    Operation opComp = p.createOperation("select-comp", n);
    Operation opAttr = p.createOperation("select-attr", n);
   
    int i, j, pmin;
    for (i = 0; i < n-1 ; i++) {
        pmin = i;
        for (j = i + 1; j < n; j++) {
            opComp.count();
            if (v[j] < v[pmin]) {
                pmin = j;
            }
        }

        if (pmin != i) {
            swap(&v[i], &v[pmin]);
            opAttr.count(3);
        }
    }
}


void afiseaza(int v[], int n) {
    printf("v : ");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void demo() {
    int v[] = { 7,2,8,5,9,1,6 };
    int v_copy[] = { 7,2,8,5,9,1,6 };
    int v_second_copy[] = { 7,2,8,5,9,1,6 };
    int n = sizeof(v) / sizeof(v[0]);
    
    printf("\nBubble:\n");
    afiseaza(v,n);
    bubbleSort(v, n);
    afiseaza(v, n);

    printf("\nInsert:\n");
    afiseaza(v_copy, n);
    insertSort(v_copy, n);
    afiseaza(v_copy, n);

    printf("\nSelect:\n");
    afiseaza(v_second_copy, n);
    selectSort(v_second_copy, n);
    afiseaza(v_second_copy, n);
}


void perf(int order) {
    int v[MAX_SIZE];
    int v_copy[MAX_SIZE];
    int v_second_copy[MAX_SIZE];

    int NR_TESTS = 1;
    if (order == 0) {
        NR_TESTS = 5;
    }
    
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(v, n, 10, 50000, false, order);
            memcpy(v_copy, v, n*sizeof(int));
            memcpy(v_second_copy, v, n*sizeof(int));
            bubbleSort(v, n);
            insertSort(v_copy, n);
            selectSort(v_second_copy, n);
        }
    }

    p.divideValues("bubble-attr", NR_TESTS);
    p.divideValues("bubble-comp", NR_TESTS);
    p.addSeries("bubble", "bubble-attr", "bubble-comp");

    p.divideValues("insert-attr", NR_TESTS);
    p.divideValues("insert-comp", NR_TESTS);
    p.addSeries("insert", "insert-attr", "insert-comp");

    p.divideValues("select-attr", NR_TESTS);
    p.divideValues("select-comp", NR_TESTS);
    p.addSeries("select", "select-attr", "select-comp");

    p.createGroup("attr", "bubble-attr", "insert-attr", "select-attr");
    p.createGroup("comp", "bubble-comp", "insert-comp", "select-comp");
    p.createGroup("sort", "bubble", "insert", "select");
}

void perf_all() {
    perf(UNSORTED);
    p.reset("best");
    perf(ASCENDING);
    p.reset("worst");
    perf(DESCENDING);
    p.showReport();
}

int main() {
    //demo();
    perf_all();
    return 0;
}