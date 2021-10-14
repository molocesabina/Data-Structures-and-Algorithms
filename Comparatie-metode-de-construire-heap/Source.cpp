/*
*@author Moloce Sabina
* 
* bottomUp are complexitatea O(N), iar topDown are O(NlogN)
* 
* BuildHeap_bottomUp se aplica atunci cand cunoastem dimensunea array-ului de la inceput si aceasta ramane constanta, este mai rapid si
* se foloseste la sortare.
* 
* BuildHeap_topDown se aplica atunci cand dimensiunea variaza si cand elementele de pus in heap vin cate unul, este mai incet si se 
* foloseste la cozile de prioritate (se adauga si heap_pop ca si instructiune)
*/

#include <stdio.h>
#include <string.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("heap");

void afiseaza(int a[], int n) {
    printf("a : ");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void swap(int* a, int* b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void heapify(int a[], int n, int i, int* at, int* c) {
    int largest = i;
    int l = 2 * i + 1;  
    int r = 2 * i + 2;

    (*c)++;
    if (l < n && a[l] > a[largest])
        largest = l;

    (*c)++;
    if (r < n && a[r] > a[largest])
        largest = r;

    if (largest != i) {
        swap(&a[i], &a[largest]);
        (*at) = (*at) + 3;
        heapify(a, n, largest, at, c);
    }

}

void buildHeap_bottomUp(int a[], int n) {
    Operation opComp = p.createOperation("bottomUp-comp", n);
    Operation opAttr = p.createOperation("bottomUp-attr", n);
    int at, c;
    for (int i = n / 2 - 1; i >= 0; i--) {
        at = 0;
        c = 0;
        heapify(a, n, i, &at, &c);
        opComp.count(c);
        opAttr.count(at);
    }
}

void heapSort(int a[], int n) {
    int attr = 0, c = 0;
    for (int i = n / 2 - 1; i >= 0; i--){
        heapify(a, n, i, &attr, &c);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(&a[0], &a[i]);
        heapify(a, i, 0, &attr, &c);
    }
}

void heapPush(int a[], int x, int i, int* at, int* c) {
    a[i] = x;
    (*at)++;
    while (i > 0 && a[(i - 1) / 2] < a[i]) { 
        (*c)++;
        swap(&a[(i - 1) / 2], &a[i]);
        (*at) = (*at) + 3;
        i = (i - 1) / 2;
    }
}

void buildHeap_topDown(int a[], int b[], int n) {
    Operation opComp = p.createOperation("topDown-comp", n);
    Operation opAttr = p.createOperation("topDown-attr", n);
    int at, c;
    for (int i = 0; i < n; i++) {
        at = 0;
        c = 0;
        heapPush(a, b[i], i, &at, &c);
        opAttr.count(at);
        opComp.count(c);
    }
}


void demo_buildHeap_bottomUp() {
	int a[] = { 7,2,8,5,9,1,6,3 };
	int n = sizeof(a) / sizeof(a[0]);
    afiseaza(a, n);
    buildHeap_bottomUp(a, n);
    printf("Dupa buildHeap bottomUp ");
    afiseaza(a, n);
}

void demo_heapSort() {
    int a[] = { 7,2,8,5,9,1,6,3 };
    int n = sizeof(a) / sizeof(a[0]);
    afiseaza(a, n);
    heapSort(a, n);
    printf("Dupa heapSort ");
    afiseaza(a, n);
}

void afiseaza_b(int a[], int n) {
    printf("b : ");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void demo_heapBuild_topDown() {
    int a[] = { 0,0,0,0,0,0,0,0 };
    int b[] = { 7,2,8,5,9,1,6,3 };
    int n = sizeof(a) / sizeof(a[0]);
    afiseaza_b(b, n);
    afiseaza(a, n);
    buildHeap_topDown(a, b, n);
    printf("Dupa buildHeap_topDown ");
    afiseaza(a, n);
}

void perf(int order) {
    int a1[MAX_SIZE];
    int a2[MAX_SIZE];
    int b[MAX_SIZE];


    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(a1, n, 10, 5000, false, order);
            memcpy(b, a1, n * sizeof(int));
            buildHeap_bottomUp(a1, n);
            buildHeap_topDown(a2, b, n);
        }
    }

    p.divideValues("bottomUp-attr", NR_TESTS);
    p.divideValues("bottomUp-comp", NR_TESTS);
    p.addSeries("bottomUp", "bottomUp-attr", "bottomUp-comp");

    p.divideValues("topDown-attr", NR_TESTS);
    p.divideValues("topDown-comp", NR_TESTS);
    p.addSeries("topDown", "topDown-attr", "topDown-comp");

    p.createGroup("attr", "bottomUp-attr", "topDown-attr");
    p.createGroup("comp", "bottomUp-comp", "topDown-comp");
    p.createGroup("totalHeapOpp", "bottomUp", "topDown");

}

void perf_all()
{
    perf(UNSORTED);
    p.reset("worst");
    perf(ASCENDING);
    p.showReport();
}

int main() {
    demo_buildHeap_bottomUp();
    demo_heapSort();
    demo_heapBuild_topDown();
    //perf_all();
	return 0;
}


