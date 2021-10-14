#include <stdio.h>
#include "Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5
/*
*@author Moloce Sabina
*       Build O(N)
*       Select O(logN)
*       Delete O(logN)
*/

Profiler p("os");

int op_attr;
int op_comp;

typedef struct node_type {
	int key;
	int size;
	struct node_type* left;
	struct node_type* right;
}node;


node* create_node(int k) {
    node* p = (node*)malloc(sizeof(node));
    p->key = k;
    p->size = 0;
    p->left = NULL;
    p->right = NULL;
    return p;
}


node * os_select(node * root, int i) {
    if (root != NULL) {
        int r = 1;
        op_comp++;
        if (root->left != NULL) {
            r += root->left->size;
            op_attr++;
        }
        if (i == r) {
            return root;
        }
        else {
            if (i < r) {
                return os_select(root->left, i);
            }
            else {
                return os_select(root->right, i - r);
            }
        }
    }
    else {
        op_comp++;
        return NULL;
    }
}



node* min_tree(node* root) {
    if (root == NULL)
        return NULL;
    while (root->left != NULL) {
        root = root->left;
        op_attr++;
        op_comp++;
    }
    return root;
}

node* os_delete(node* root, int key) {
    node* p;
    op_comp++;
    if (root == NULL)
        return NULL;
    if (root->key == key) {
        if (root->left == NULL) {
            op_comp += 2;
            op_attr;
            p = root->right;
            free(root);
            return p;
        }
        if (root->right == NULL) {
            op_comp += 3;
            op_attr++;
            p = root->left;
            free(root);
            return p;
        }
        op_comp += 3;
        op_attr += 4;
        p = min_tree(root->right);
        root->key = p->key;
        root->size--;
        root->right = os_delete(root->right, p->key);

    }
    else if (key < root->key) {
        op_comp += 2;
        op_attr += 2;
        root->size--;
        root->left = os_delete(root->left, key);
    }
    else {
        op_attr += 2;
        op_comp += 2;
        root->size--;
        root->right = os_delete(root->right, key);
    }

    return root;
}


node* to_pbt(int a[], int left, int right) {
    if (left > right)
        return NULL;

    int m = (left +right) / 2;
    op_attr += 2;
    node* root = create_node(a[m]);
    root->size = right - left + 1;

    op_attr += 2;
    root->left = to_pbt(a, left, m - 1);
    root->right = to_pbt(a, m + 1, right);
    return root;
}


void pretty_print(node* root, int nivel) {
    if (root != NULL) {
        for (int i = 0; i < nivel; i++) {
            printf("  ");
        }
        printf("%d s: %d\n", root->key, root->size);
        pretty_print(root->left, nivel + 1);
        pretty_print(root->right, nivel + 1);
    }
}




void demo() {
    int n = 11;
    int a[] = { 1,2,3,4,5,6,7,8,9,10,11 };
   
    node* root=to_pbt(a, 0, n-1);
    pretty_print(root, 0);


    for (int i = 0; i < 3; i++) {
        int num = rand() % (n-i) + 1;
        node* nd = os_select(root, num);
        printf("nodul cu a %d cea mai mica cheie este %d\n", num, nd->key);
        root=os_delete(root, nd->key);
        printf("am sters nodul selectat\n");
        pretty_print(root, 0);
    }

}

void perf() {
    int a[MAX_SIZE];
    FillRandomArray(a, MAX_SIZE, 1, MAX_SIZE, true, ASCENDING);
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation opAttr_b = p.createOperation("build-attr", n);
        Operation opComp_b = p.createOperation("build-comp", n);

        Operation opAttr_s = p.createOperation("select-attr", n);
        Operation opComp_s = p.createOperation("select-comp", n);

        Operation opAttr_d = p.createOperation("delete-attr", n);
        Operation opComp_d = p.createOperation("delete-comp", n);
        for (int test = 0; test < NR_TESTS; test++) {
            op_attr = 0; op_comp = 0;
            node* root = to_pbt(a, 0, n-1);
            opAttr_b.count(op_attr);
            opComp_b.count(op_comp);
            
            for (int i = 0; i < n; i++) {
                op_attr = 0; op_comp = 0;
                int num = rand() % (n-i) + 1;
                node* nd = os_select(root,num);
                opAttr_s.count(op_attr);
                opComp_s.count(op_comp);

                op_attr = 0; op_comp = 0;
                root=os_delete(root, nd->key);
                opAttr_d.count(op_attr);
                opComp_d.count(op_comp);
            }

        }
    }

    p.divideValues("build-attr", NR_TESTS);
    p.divideValues("build-comp", NR_TESTS);

    p.divideValues("select-attr", NR_TESTS);
    p.divideValues("select-comp", NR_TESTS);

    p.divideValues("delete-attr", NR_TESTS);
    p.divideValues("delete-comp", NR_TESTS);

    p.addSeries("select", "select-attr", "select-comp");
    p.addSeries("delete", "delete-attr", "delete-comp");
    p.addSeries("build", "build-attr", "build-comp");

    p.createGroup("attr", "select-attr", "delete-attr","build-attr");
    p.createGroup("comp", "select-comp", "delete-comp","build-comp");


    p.createGroup("totalOpp", "select", "delete", "build");
    p.showReport();

}

int main() {
    srand(time(NULL));
    demo();
    //perf();
	return 0;
}