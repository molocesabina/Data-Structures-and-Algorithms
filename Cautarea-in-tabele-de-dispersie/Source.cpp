
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"


#define N 10007
#define NR_TESTS 5
#define c1 1
#define c2 1
int nr_op;

/*
*@author Moloce Sabina
*      
*   Pentru cheile care se gasesc in tabela, timpul mediu de cautare depinde de factorul de umplere si  1/alfa*ln(1/(1- alfa))
*   Pentru cheile care nu se gasesc in tabela, timpul mediu de cautare este  1/(1- alfa)
*    
*
*    Factor de umplere   |Efort mediu gasite  |Efort maxim gasite  |Efort mediu negasite|Efort maxim negasite
*    0.80                |2.00                |30                  |5.79                |51
*    0.85                |2.28                |41                  |8.99                |69
*    0.90                |2.80                |59                  |15.63               |88
*    0.95                |3.66                |105                 |34.04               |188
*    0.99                |5.55                |263                 |110.40              |829
*
*
*/


typedef struct cell{
	int key;
	int status;
} hashCell;

enum { FREE, OCCUPIED };

int h_prime(int k, int size){
    return k % size;
}

int hash(int k, int size, int i){
    return (h_prime(k, size) + c1 * i + c2 * i * i) % size;
}

void print_table(hashCell* t, int size){
    printf("tabela de dispersie este \n");
    for (int i = 0; i < size; i++){
        if (t[i].status == OCCUPIED)
            printf("t[%d] = %d\n", i, t[i].key);
        else
            printf("t[%d] = FREE\n", i);
    }
}

void insert_key(hashCell* t, int k, int size){
    for (int i = 0; i < size; i++) {
        int h = hash(k, size, i);
        if (t[h].status == FREE){
            t[h].key = k;
            t[h].status = OCCUPIED;
            return;
        }
    }
    printf("tabela plina, nu se poate insera %d\n", k);
}

int search_key(hashCell* t, int k, int size) {
    nr_op = 0;
    for (int i = 0; i < size; i++) {
        nr_op++;
        int h = hash(k, size, i);
        if (t[h].status == FREE)
            return -1;
        if (t[h].key == k)
            return h;
    }
    return -1;
}

void free_table(hashCell* t, int size){
    for (int i = 0; i < size; i++) {
        t[i].status = FREE;
    }
}

void demo() {
    int size = 19;
    hashCell* t = (hashCell*)malloc(size * sizeof(hashCell));
     
    int to_insert[18] = { 500,13,78,18,19,38,7,150,189,1000,98,256,455,678,78,9,100, 36};
    free_table(t, size);
    for (int i = 0; i < 18; i++){
        insert_key( t, to_insert[i], size);
    }
    print_table(t, size);

    int to_search[6] = { 678,7,500,12,40,10 }; 
    for (int i = 0; i < 6; i++) {
        int p = search_key( t, to_search[i], size);
        if (p != -1)
            printf("am gasit cheia %d la poz %d in tabela\n", to_search[i], p);
        else
            printf("nu am gasit cheia %d in tabela\n", to_search[i]);
    }
}


void perf() {
    int n_size[5] = { 8006,8506,9007,9507,9907 };
    int v[N + 1500];
    int indecsi[1500];

    int max_op_gasite;
    int nr_op_gasite;
    int max_op_negasite;
    int nr_op_negasite;
    int max_gasite = 0;;
    int max_negasite = 0;

    int spatii = 20;
    printf("%-*s|%-*s|%-*s|%-*s|%-*s\n", spatii, "Factor de umplere", spatii, "Efort mediu gasite", spatii, "Efort maxim gasite", spatii, "Efort mediu negasite",spatii, "Efort maxim negasite");
 
    hashCell* t = (hashCell*)malloc(N * sizeof(hashCell));
    for (int i = 0; i < 5; i++) {
        int n = n_size[i];
        
        max_op_gasite = 0;
        max_op_negasite = 0;
        nr_op_gasite = 0;
        nr_op_negasite = 0;
        for (int test = 0; test < NR_TESTS; test++) {
            free_table(t, N);

            FillRandomArray(v, n + 1500, 10, 50000, true, UNSORTED);
            FillRandomArray(indecsi, 1500, 0, n - 1, true, UNSORTED);

            for (int j = 0; j < n; j++) {
                insert_key(t, v[j], N);
            }

            max_gasite = 0;
            for (int j = 0; j < 1500; j++) {
                search_key(t, v[indecsi[j]], N);
                nr_op_gasite += nr_op;
                if (nr_op > max_gasite)
                    max_gasite = nr_op;
            }
            max_op_gasite += max_gasite;

            max_negasite = 0;
            for (int j = n; j < n + 1500; j++) {
                search_key(t, v[j], N);
                nr_op_negasite += nr_op;
                if (nr_op > max_negasite)
                    max_negasite = nr_op;
            }
            max_op_negasite += max_negasite;
        }
        max_op_gasite /= 5;
        max_op_negasite /= 5;
        nr_op_gasite /= 5;
        nr_op_negasite /= 5;
        printf("%-*.2f|%-*.2f|%-*d|%-*.2f|%-*d\n", spatii,(float)n/N, spatii,(float)nr_op_gasite/1500,spatii, max_op_gasite, spatii, (float)nr_op_negasite/1500, spatii,max_op_negasite);
    }

    free(t);
}

int main() {
    //demo();
    perf();
}
