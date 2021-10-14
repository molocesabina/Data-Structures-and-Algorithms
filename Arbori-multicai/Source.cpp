#include <stdio.h>
#include "Profiler.h"
#define MAX_CHILD 100
/*
*@author Moloce Sabina
*	Pentru R2 am folosit o structura de nod care are o cheie, numarul de copii si un vector de pointeri la copii.
*	Transformarea din R1 in R2 se face in O(N) (se observa ca avem doar doua for-uri de la 1 la n si ca ce se afla
* in interiorul celor 2 for-uri este constant O(1)), dar se foloseste memorie aditionala O(N) pentru vectorul de pointeri created2.
* 
*	Pentru R3 am folosit o structura de nod care are o cheie, un pointer la primul copil si un pointer la fratele copilului.
*	Transformarea din R2 in R3 se face in O(N), deoarece fiecare nod este procesat doar o data.
* 
*	Transformarea din R1 in R3 se face in O(N) (se observa ca avem doar doua for-uri de la 1 la n si ca ce se afla
* in interiorul celor 2 for-uri este constant O(1)), dar se foloseste memorie aditionala O(N) (dublu fata de tranformarea din R1 in R2
* pentru created3, dar si pentru last_child, care se foloseste pentru a asigura ca transformarea este liniara si de ex daca avem o radacina
* si restul nodurilor sunt copiii ei, atunci daca nu am avea un nod in care sa retinem care este ultimul copil adaugat pe relatia de sibling, 
* atunci ar trebui sa parcurgem mereu toti copii nodului pentru a ajunge la ultimul nod care nu are sibling si sa-l adaugam acolo => O(N^2)).
*/


int parent[] = { 0, 2,7,5,2,7,7,-1,5,2 };
int n = sizeof(parent) / sizeof(parent[0]);

typedef struct node_type {
	int key;
	int nb_children;
	struct node_type* children[MAX_CHILD];
}node;

typedef struct node_b {
	int key;
	struct node_b* child;
	struct node_b* sibling;
}nodeB;

node* new_node(int k) {
	node* p = (node*)malloc(sizeof(node));
	p->key = k;
	p->nb_children = 0;
	return p;
}

nodeB* new_nodeB(int k) {
	nodeB* p = (nodeB*)malloc(sizeof(nodeB));
	p->key = k;
	p->child = NULL;
	p->sibling = NULL;
	return p;
}



void pretty_print1(int to_search, int nivel) {
	for (int i = 1; i < n; i++) {
		if (parent[i] == to_search) {
			for (int j = 0; j < nivel; j++) {
				printf("  ");
			}
			printf("%d \n", i); 
			pretty_print1(i, nivel + 1);
		}
	}
}



// din R1 in R2
node* transform1(){
	node* root = NULL;
	node** created2 = (node**)malloc(n*sizeof(node*));
	for (int i = 1; i < n; i++) {
		created2[i] = new_node(i); 
	}

	for (int i = 1; i < n; i++){
		if (parent[i] == -1){
			root = created2[i];
		}
		else {
			node* p = created2[parent[i]];
			p->children[p->nb_children] = created2[i];
			p->nb_children++;
		}
	}
	return root;
}




void pretty_print2(node* root, int nivel) {
	if (root != NULL) {
		for (int i = 0; i < nivel; i++) {
			printf("  ");
		}
		printf("%d\n", root->key);
		for (int i = 0; i < root->nb_children; i++) {
			pretty_print2(root->children[i], nivel + 1);
		}
	}
}


// din R2 in R3
nodeB* transform2(node* root) {
	if (root == NULL) {
		return NULL;
	}
	else {
		nodeB* n = new_nodeB(root->key);
		int nb_children = root->nb_children;

		if (nb_children != 0) {
			n->child = transform2(root->children[0]);

			nodeB* last_child_added = n->child;

			for (int i = 1; i < nb_children; i++) {
				last_child_added->sibling = transform2(root->children[i]);
			    last_child_added = last_child_added->sibling;
			}
		}
		return n;
	}
}



// din R1 in R3
nodeB* transform3() {
	nodeB* root = NULL;
	nodeB** created3 = (nodeB**)malloc(n * sizeof(nodeB*));
	nodeB** last_child = (nodeB**)malloc(n * sizeof(nodeB*));
	for (int i = 1; i < n; i++) {
		created3[i] = new_nodeB(i); 
	}

	for (int i = 1; i < n; i++) {
		if (parent[i] == -1) {
			root = created3[i];
		}
		else {
			nodeB* p = created3[parent[i]];
			if (p->child == NULL) {
				p->child = created3[i];
				last_child[parent[i]] = created3[i];
			}
			else {
				last_child[parent[i]]->sibling = created3[i];
				last_child[parent[i]] = created3[i];
			}
		}
	}
	return root;
}


void pretty_print3(nodeB* root, int nivel) {
	if (root != NULL) {
		for (int i = 0; i < nivel; i++) {
			printf("  ");
		}
		printf("%d\n", root->key);
		pretty_print3(root->child, nivel + 1);
		pretty_print3(root->sibling, nivel);
	}
}

int main() {
	printf("pretty print 1:\n");
	pretty_print1(-1, 0);

	node* r2 = transform1();
	printf("\n");
	printf("pretty print 2:\n");
	pretty_print2(r2, 0);


	nodeB* r3 = transform2(r2);
	printf("\n");
	printf("pretty print 3:\n");
	pretty_print3(r3, 0);

	r3 = transform3();
	printf("\n");
	printf("pretty print 3:\n");
	pretty_print3(r3, 0);

	return 0;
}