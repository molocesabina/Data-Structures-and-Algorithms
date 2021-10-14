/*
*@author Moloce Sabina
*   
*       QuickSort vs HeapSort
*   Pentru cazul mediu statistic ambii alogritmi au complexitatea O(NlogN), dar HeapSort are o constanta multiplicativa mai mare. In cazul
* defavorabil complexitatea pentru QuickSort(fara inbunatatiri) este O(N^2), HeapSort ramane insa tot O(NlogN).
*       
*      Quicksort vs Quicksort(random_partition)
*   Pentru cazul mediu statistic se comporta asemanator O(NlogN), insa pentru cazul defavorabil cand sirul este ordonat crescator/descrescator
*   QuickSort(random_partition) are tot O(NlogN), iar Quicksort are complexitatea O(N^2)
*/

#include <stdio.h>
#include <string.h>
#include "Profiler.h"

#define MAX_SIZE 3000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("SortAdvanced");

int op_attr;
int op_comp;
int op_attr2; 
int op_comp2;

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

void heapSort(int a[], int n) {
    Operation opAttr = p.createOperation("heapSort-attr", n);
    Operation opComp = p.createOperation("heapSort-comp", n);
    int attr, comp;
    for (int i = n / 2 - 1; i >= 0; i--) {
        attr = 0; comp = 0;
        heapify(a, n, i, &attr, &comp);
        opAttr.count(attr);
        opComp.count(comp);
    }

    for (int i = n - 1; i > 0; i--) {
        attr = 0; comp = 0;
        swap(&a[0], &a[i]);
        opAttr.count(3);
        heapify(a, i, 0, &attr,&comp);
        opAttr.count(attr);
        opComp.count(comp);
    }
}


int partition(int a[], int p, int r) {
    int pivot = a[r];
    op_attr++;
    op_attr2++;
    int i = p - 1;
    for (int j = p; j < r; j++) {
        op_comp++;
        op_comp2++;
        if (a[j] <= pivot) {
            i++;
            swap(&a[i], &a[j]);
            op_attr += 3;
            op_attr2 += 3;
        }
    }
    swap(&a[i + 1], &a[r]);
    op_attr += 3;
    op_attr2 += 3;
    return i + 1;
}



void quickSort(int a[], int p, int r) {
    if (p < r) {
        int q = partition(a, p, r);
        quickSort(a, p, q - 1);
        quickSort(a, q + 1, r);
    }
}


int randomPartition(int a[], int p, int r) {
    srand(time(NULL));
    int random = p + rand() % (r - p + 1);;
    swap(&a[random], &a[r]);
    op_attr2 += 3;
    return partition(a, p, r);
}


int randomizedSelect(int a[], int p, int r, int i) {
    if (p == r)
        return a[p];
    int q = randomPartition(a, p, r);
    int k = q - p + 1;
    if (i == k)
        return a[q];
    else if (i < k)
        return randomizedSelect(a, p, q - 1, i);
    else return randomizedSelect(a, q + 1, r, i - k);
}


void quickSort_v2(int a[], int p, int r) {
    if (p < r) {
        int q = randomPartition(a, p, r);
        quickSort_v2(a, p, q - 1);
        quickSort_v2(a, q + 1, r);
    }
}


void demo_heapSort() {
    int a[] = { 7,2,8,4,5,9,1,6,3 };
    int n = sizeof(a) / sizeof(a[0]);
    afiseaza(a, n);
    heapSort(a, n);
    printf("Dupa heapSort ");
    afiseaza(a, n);
}

void demo_quickSort() {
    int a[] = { 1,2,7,8,4,5,3,6,9 };
    int n = sizeof(a) / sizeof(a[0]);
    afiseaza(a, n);
    quickSort(a, 0, n-1);
    printf("Dupa quickSort ");
    afiseaza(a, n);
}

void demo_quickSort_v2() {
    int a[] = { 7,2,8,4,5,9,1,6,3 };
    int n = sizeof(a) / sizeof(a[0]);
    afiseaza(a, n);
    quickSort_v2(a, 0, n - 1);
    printf("Dupa quickSort cu random_partition: ");
    afiseaza(a, n);
}

void demo_randomizedSelect() {
    int a[]= { 7,2,8,4,5,9,1,6,3 };
    int n = sizeof(a) / sizeof(a[0]);
    afiseaza(a, n);
    for (int i = 0; i < n; i++) {
        printf("al %d-lea numar este %d\n", i+1,randomizedSelect(a, 0, n - 1,i+1));
    }
    printf("\n");
    afiseaza(a, n);
}


void perf(int order, int varianta) {
    int a1[MAX_SIZE];
    int a2[MAX_SIZE];
   

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(a1, n, 10, 50000, false, order);
            memcpy(a2, a1, n * sizeof(int));
           
            if (varianta == 1) {
                heapSort(a1, n);
            }

            op_attr = 0; op_comp = 0;
            Operation opAttr = p.createOperation("quickSort-attr", n);
            Operation opComp = p.createOperation("quickSort-comp", n);
            quickSort(a2, 0, n - 1);
            opAttr.count(op_attr);
            opComp.count(op_comp);

            if (varianta == 2) {
                op_attr2 = 0; op_comp2 = 0;
                Operation opAttr2 = p.createOperation("quickSort-v2-attr", n);
                Operation opComp2 = p.createOperation("quickSort-v2-comp", n);
                quickSort_v2(a1, 0, n - 1);
                opAttr2.count(op_attr2);
                opComp2.count(op_comp2);
            }
        }
    }


    if (varianta == 1) {
        p.divideValues("heapSort-attr", NR_TESTS);
        p.divideValues("heapSort-comp", NR_TESTS);
        p.addSeries("heapSort", "heapSort-attr", "heapSort-comp");

        p.createGroup("attr", "heapSort-attr", "quickSort-attr");
        p.createGroup("comp", "heapSort-comp", "quickSort-comp");
        p.createGroup("totalOpp", "heapSort", "quickSort");
    }


    p.divideValues("quickSort-attr", NR_TESTS);
    p.divideValues("quickSort-comp", NR_TESTS);
    p.addSeries("quickSort", "quickSort-attr", "quickSort-comp");


    if (varianta == 2) {

        p.divideValues("quickSort-v2-attr", NR_TESTS);
        p.divideValues("quickSort-v2-comp", NR_TESTS);
        p.addSeries("quickSort-v2", "quickSort-v2-attr", "quickSort-v2-comp");

        p.createGroup("attr", "quickSort-attr", "quickSort-v2-attr");
        p.createGroup("comp", "quickSort-comp", "quickSort-v2-comp");
        p.createGroup("totalOppQuick", "quickSort", "quickSort-v2");
    }

}


void perf_all()
{
    perf(UNSORTED, 1);
    p.reset("quicks-mediu");
    perf(UNSORTED, 2);
    p.reset("quicks-asc");
    perf(ASCENDING,2);
    p.reset("quicks-desc");
    perf(DESCENDING, 2);
    p.showReport();
}


int main() {
    //demo_heapSort();
    //demo_quickSort();
    //demo_randomizedSelect();
    //demo_quickSort_v2();
    perf_all();
    return 0;
}
