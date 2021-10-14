/*
*@author Moloce Sabina
*   Complexitate mergeKlists O(N*logK)
*   Se observa ca pentru merge cu k=5/10/100 liste si n variabil, complexitatea daca privim logK ca o constanat O(N*loK) => O(N) 
*     curbele cresc liniar doar constantele multiplicative difera.
*   Pentru n=10000 si k variaza, complexitate O(10000*logk) => O(logk) curba creste logaritmic
*/

#include <stdio.h>
#include <string.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100

Profiler p("MergeLists");

int op_attr;
int op_comp;

typedef struct nodetype {
    int key;
    struct nodetype* next;
}node;

node* create_node(int key) {
    node* p = (node*)malloc(sizeof(node));
    p->key = key;
    p->next = NULL;
    return p;
}

void insert_first(node** first, int givenkey) {
    node* p = create_node(givenkey);
    if (*first == NULL)
        *first = p;
    else{
        p->next = *first;
        *first = p;
    }
}

void print_list(node* first) {
    node* q = first;
    while (q != NULL) {
        printf("%d ", q->key);
        q = q->next;
    }
    printf("\n");
}

node* create_list(int nr_elem) {
    int v[MAX_SIZE];
    FillRandomArray(v, nr_elem, 10, 500, false, 2);

    node* lista = NULL;
    for (int i = 0; i < nr_elem; i++) {
        insert_first(&lista, v[i]);
    }
    return lista;
}

node** create_lists( int n,int k) { 
    node** liste = (node**)malloc(k * sizeof(node*));
    
    for (int i = 0; i < k; i++) {
        if (i < n % k) 
            liste[i] = create_list(n / k + 1);
        else
            liste[i] = create_list(n / k);
    }
    return liste;
}


node* merge(node* lista1, node* lista2){
    node* result = NULL;

    if (lista1 == NULL)
        return lista2;
    else if (lista2 == NULL)
        return lista1;

    op_comp++;
    op_attr += 2;
    if (lista1->key <= lista2->key) {
        result = lista1;
        result->next = merge(lista1->next, lista2);
    }
    else {
        result = lista2;
        result->next = merge(lista1, lista2->next);
    }
    return result;
}

void swap(node** n1, node** n2) {
    node* aux = *n1;
    *n1 = *n2;
    *n2 = aux;
}

void heapify(node** liste, int k, int i) {
	int smallest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	
    op_comp++;
	if (l < k && liste[l]->key < liste[smallest]->key) {
		smallest = l;
	}

    op_comp++;
	if (r < k && liste[r]->key < liste[smallest]->key) {
		smallest = r;
	}

	if (smallest != i) {
        swap(&liste[i], &liste[smallest]);
        op_attr += 3;
		heapify(liste, k, smallest);
	}
}

void bottomUp(node** liste, int k){
	for (int i = k/2-1; i>=0; i--) {
		heapify(liste, k, i);
	}
}


node* mergeKlists(node** liste, int n, int k) {
	node* merged = NULL;
	node* last = NULL;
    bottomUp(liste, k);

	while (k > 2) {
        op_attr += 2;
		if (merged == NULL){
			merged = liste[0];
			last = merged;
		}
		else{
			last->next = liste[0];
            last = last->next;
		}
		
        op_attr++;
        liste[0] = liste[0]->next;
      
		if (liste[0] == NULL){
			liste[0] = liste[k-1];
            op_attr++;
            k--;
		}
        
        heapify(liste, k, 0);
	}

    if(last!=NULL)
        last->next = merge(liste[0], liste[1]);
    else
        merged = merge(liste[0], liste[1]);
	return merged;
}

void demo() {
    int k = 4;
    int n = 20;
    
    node** liste = create_lists(n, k);
    for (int i = 0; i < k; i++) {
        print_list(liste[i]);
    }
    printf("\n");
    
    node* merged = mergeKlists(liste, n, k);
    print_list(merged);
    free(merged);
}

void perf_vary_n() {
    int k;
    node** liste;

    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        k = 5;
        liste = create_lists(n, k);
        Operation opAttr1 = p.createOperation("mergeKlists-k=5-attr", n);
        Operation opComp1 = p.createOperation("mergeKlists-k=5-comp", n);
        op_attr = 0; op_comp = 0;
        mergeKlists(liste, n, k);
        opAttr1.count(op_attr);
        opComp1.count(op_comp);

        k = 10;
        liste = create_lists(n, k);
        Operation opAttr2 = p.createOperation("mergeKlists-k=10-attr", n);
        Operation opComp2 = p.createOperation("mergeKlists-k=10-comp", n);
        op_attr = 0; op_comp = 0;
        mergeKlists(liste, n, k);
        opAttr2.count(op_attr);
        opComp2.count(op_comp);

        k = 100;
        liste = create_lists(n, k);
        Operation opAttr3 = p.createOperation("mergeKlists-k=100-attr", n);
        Operation opComp3 = p.createOperation("mergeKlists-k=100-comp", n);
        op_attr = 0; op_comp = 0;
        mergeKlists(liste, n, k);
        opAttr3.count(op_attr);
        opComp3.count(op_comp);
    }

    p.addSeries("mergeKlists-k=5", "mergeKlists-k=5-attr", "mergeKlists-k=5-comp");
    p.addSeries("mergeKlists-k=10", "mergeKlists-k=10-attr", "mergeKlists-k=10-comp");
    p.addSeries("mergeKlists-k=100", "mergeKlists-k=100-attr", "mergeKlists-k=100-comp");

    p.createGroup("attr", "mergeKlists-k=5-attr", "mergeKlists-k=10-attr", "mergeKlists-k=100-attr");
    p.createGroup("comp", "mergeKlists-k=5-comp", "mergeKlists-k=10-comp", "mergeKlists-k=100-comp");
    p.createGroup("mergeKlists", "mergeKlists-k=5", "mergeKlists-k=10", "mergeKlists-k=100");
}

void perf_vary_k() {
    node** liste;
    int n = 10000;
    for (int k = 10; k <= 500; k += 10) {
        Operation opAttr = p.createOperation("mergeKlists-n=10000-attr", k);
        Operation opComp = p.createOperation("mergeKlists-n=10000-comp", k);
        liste = create_lists(n, k);
        op_attr = 0; op_comp = 0;
        mergeKlists(liste, n, k);
        opAttr.count(op_attr);
        opComp.count(op_comp);
    }
    p.addSeries("mergeKlists-n=10000", "mergeKlists-n=10000-attr", "mergeKlists-n=10000-attr");
}

void perf() {
    perf_vary_n();
    p.reset("n=10000");
    perf_vary_k();
    p.showReport();
}

int main() {
    demo();
    //perf();
    return 0;
}