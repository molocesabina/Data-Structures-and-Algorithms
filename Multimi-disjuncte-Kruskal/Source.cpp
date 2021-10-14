#include <stdio.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100

/*
*@author Moloce Sabina
*	Complexitate Kruskal: O(ElogE)
*   Complexitatea pentru m operatii de make_set, find_set, union pe o multime cu n elemente este O(m*alfa(n)), unde alfa(n) o functie care creste foarte incet
* deci putem sa vedem timpul ca fiind liniar.
*/

int n;
Profiler k("krusk");
int op_attr;
int op_comp;


typedef struct node_type {
	int key;
	struct node_type* parent;
	int rank;
}node;

typedef struct edge_type {
	node* p1;
	node* p2;
	int weight;
	int status;
}edge;


enum { NOT_ADDED, ADDED};


node* make_set(int key) {
	node* p = (node*)malloc(sizeof(node));
    p->key = key;
	p->rank = 0;
	p->parent = p;

	k.countOperation("make-set-attr", n, 3);
	k.countOperation("kruskal", n, 3);
	return p;
}


node* find_set(node* x) {
	op_comp++;
	k.countOperation("kruskal", n);

	if (x != x->parent) {
		op_attr++;
		k.countOperation("kruskal", n);

		x->parent = find_set(x->parent);
	}
	return x->parent;
}


void uniune(node* x, node* y) {
	node* rep_x = find_set(x);
	node* rep_y = find_set(y);
	k.countOperation("uniune-attr", n, op_attr);
	k.countOperation("uniune-comp", n, op_comp);

	k.countOperation("uniune-comp", n);
	k.countOperation("kruskal", n);

	if (rep_x->rank > rep_y->rank) {
		k.countOperation("uniune-attr", n);
		k.countOperation("kruskal", n);

		rep_y->parent = rep_x;
	}
	else {
		k.countOperation("uniune-attr", n);
		k.countOperation("kruskal", n);

		rep_x->parent = rep_y;

		k.countOperation("uniune-comp", n);
		k.countOperation("kruskal", n);
		if (rep_x->rank == rep_y->rank) {
			rep_y->rank++;

			k.countOperation("uniune-attr", n);
			k.countOperation("kruskal", n);
		}
	}
}




edge* make_edge(node* p1, node* p2, int w) {
	edge* e = (edge*)malloc(sizeof(edge));
	e->p1 = p1;
	e->p2 = p2;
	e->weight = w;
	e->status = NOT_ADDED;
	return e;
}



void swap(edge** a, edge** b) {
	edge* aux = *a;
	*a = *b;
	*b = aux;
	k.countOperation("kruskal", n, 3);
}

int partition(edge** a, int p, int r) {
	edge* pivot = a[r];
	k.countOperation("kruskal", n);
	int i = p - 1;
	for (int j = p; j < r; j++) {
		k.countOperation("kruskal", n);
		if (a[j]->weight <= pivot->weight) {
			i++;
			swap(&a[i], &a[j]);
		}
	}
	swap(&a[i + 1], &a[r]);
	return i + 1;
}


int randomPartition(edge** a, int p, int r) {
	int random = p + rand() % (r - p + 1);
	swap(&a[random], &a[r]);
	return partition(a, p, r);
}


void quick_sort_edges(edge** a, int p, int r) {
	if (p < r) {
		int q = randomPartition(a, p, r);
		quick_sort_edges(a, p, q - 1);
		quick_sort_edges(a, q + 1, r);
	}
}



void kruskal(node** noduri_graf, edge** muchii_graf, int nr_muchii) {
	quick_sort_edges(muchii_graf,0, nr_muchii-1);
	for (int i = 0; i < nr_muchii; i++) {
		op_attr = 0; op_comp = 0;
		node* rep_p1 = find_set(muchii_graf[i]->p1);
		node* rep_p2 = find_set(muchii_graf[i]->p2);
		k.countOperation("find-set-attr", n, op_attr);
		k.countOperation("find-set-comp", n, op_comp);
	
		if (rep_p1 != rep_p2) {
			op_attr = 0; op_comp = 0;
			uniune(muchii_graf[i]->p1, muchii_graf[i]->p2);
			muchii_graf[i]->status = ADDED;
		}
	}
}

void perf() {
	node* noduri_graf[MAX_SIZE];
	edge* muchii_graf[MAX_SIZE * 4];
	int ok = 0;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {

		for (int i = 0; i < n; i++) {
			noduri_graf[i] = make_set(i);
		}

		for (int i = 0; i < 4 * n; i++) {
			int p1_idx, p2_idx;
			if (i < n - 1) {
				p1_idx = i+1;
				p2_idx = rand() % (i+1);
			}
			else {
				p1_idx = rand() % n;
				p2_idx = rand() % n;
			}
			int weight = rand() % 10000 + 1;
			muchii_graf[i] = make_edge(noduri_graf[p1_idx], noduri_graf[p2_idx], weight);
	    }
		
		kruskal(noduri_graf, muchii_graf, 4 * n);
		k.countOperation("make-set-comp", n, 0);
	}
	k.addSeries("make-set", "make-set-attr", "make-set-comp");
	k.addSeries("find-set", "find-set-attr", "find-set-comp");
	k.addSeries("uniune", "uniune-attr", "uniune-comp");

	k.createGroup("attr", "make-set-attr", "find-set-attr", "uniune-attr");
	k.createGroup("comp", "make-set-comp", "find-set-comp", "uniune-comp");
	k.createGroup("totalOpp", "make-set", "find-set", "uniune");

	k.showReport();
}

void print_set(node* p) {
	printf("%d ", p->key);
	if (p->parent != p)
		print_set(p->parent);
}

void demo() {
	int n = 5;
	node** s = (node**)malloc(n * sizeof(node*));

	for (int i = 0; i < n; i++) {
		s[i] = make_set(i);
	}

	for (int i = 0; i < n; i++) {
		print_set(s[i]);
	}

	for (int i = n-2; i >= 0 ; i--) {
		uniune(s[i], s[i + 1]);
	}

	printf("\n");
	for (int i = 0; i < n; i++) {
		print_set(s[i]);
		printf("\n");
	}

	
	for (int i = 0; i < n; i++) {
		s[i] = make_set(i);
	}

	edge** e = (edge**)malloc(7 * sizeof(edge*));
	e[0] = make_edge(s[0], s[1], 3);
	e[1] = make_edge(s[0], s[2], 7);
	e[2] = make_edge(s[1], s[2], 5);
	e[3] = make_edge(s[1], s[3], 4);
	e[4] = make_edge(s[2], s[3], 2);
	e[5] = make_edge(s[2], s[4], 9);
	e[6] = make_edge(s[3], s[4], 8);
	
	printf("\nKruskal:\n");
    kruskal(s, e, 7);
	for(int i=0;i<7;i++){
		if(e[i]->status == ADDED)
			printf("muchia: %d - %d cost %d\n", e[i]->p1->key, e[i]->p2->key, e[i]->weight);	
	}

}


int main() {
	srand(time(NULL));
	demo();
	//perf();
	return 0;
}