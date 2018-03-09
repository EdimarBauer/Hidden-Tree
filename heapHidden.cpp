/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: February 23, 2017
*
* Updated: March 09, 2018
*
* The variable 'last' and 'total' serve to test the algorithm
*/

#include <bits/stdc++.h>

using namespace std;
#define N (1 << 24)

int last, total;

struct Tree{
    int key;
    struct Tree *left, *right;
};

//pointer for the most left element (element of minimum)
Tree *heap;
stack<Tree*> pilha;   //store the parents of left nodes to update the heap after remotion

Tree* newNode(int x);
Tree* search(Tree *tree, int x);

void insert(Tree *&root, int x);

int replace(Tree **tree);
void popHeap(Tree *&tree);

void print(Tree *tree, int i);
void check_is_correct(Tree *tree);
void check(Tree *tree);



int main(){

    Tree* tree = NULL;
    int x;

    for (int j = 0; j < 2; j++){
        printf("Inserting...\n");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            if (rand() % 2) x = -x;
            insert(tree, x);
            //check_is_correct(tree);
        }
        //print(tree, 0);
        printf("Removing...\n");
        while(heap != NULL){
            popHeap(tree);
            //check_is_correct(tree);
        }
    }

    //print(tree, 0);
}



//#####################################################################################################
//#####################################################################################################





Tree* newNode(int x){

    Tree *tree = (Tree*) malloc(sizeof(Tree));
    if (tree == NULL){
        puts("Err of allocation");
        exit(0);
    }
    tree->key = x;
    tree->left = tree->right = NULL;
    total++;
    return tree;
}

Tree* search(Tree* tree, int x){

    while(tree != NULL){
        if (tree->key == x) return tree;

        if (x < tree->key) tree = tree->left;
        else tree = tree->right;
    }

    return NULL;
}




void insert(Tree *&root, int x){

    if (root == NULL){
        root = newNode(x);
        heap = root;
    }else{
        Tree *tree = root;
        int middle = 0;
        int jump = (1 << 30);
        bool right = 0;

        while(tree->key != x){
            if (x < middle){
                if (x > tree->key) swap(x, tree->key);

                if (tree->left == NULL){
                    tree->left = newNode(x);
                    if (right == 0){
                        pilha.push(tree);
                        heap = tree->left;
                    }
                    break;
                }
                middle -= jump;
                jump /= 2;
                tree = tree->left;
            }else if (x > middle){
                if (x < tree->key) swap(x, tree->key);

                if (tree->right == NULL){
                    tree->right = newNode(x);
                    break;
                }
                tree = tree->right;
                middle += jump;
                jump /= 2;
                right = 1;
            //else keeps x on the node
            }else swap(x, tree->key);
        }
    }
}





int replace(Tree **tree){

    while((*tree)->left != NULL) tree = &((*tree)->left);

    int x = (*tree)->key;
    if ((*tree)->right == NULL){
        free(*tree);
        *tree = NULL;
    }else (*tree)->key = replace(&(*tree)->right);

    return x;
}

void popHeap(Tree *&tree){

    if (tree == NULL) return;
    total--;
    if (heap->right == NULL){
        free(heap);
        if (pilha.empty()){
            heap = tree = NULL;
        }else{
            heap = pilha.top();
            pilha.pop();
            heap->left = NULL;
        }
    }else heap->key = replace(&(heap->right));
}





void print(Tree *tree, int i){

    if (tree == NULL) return;

    print(tree->left, i+1);
    cout << tree->key << " " << i << endl;
    print(tree->right, i+1);
}

void check_is_correct(Tree *tree){

    last = INT_MIN;
    int to = total;
    check(tree);
    if (total != 0){
        printf("Diference of %d elements\n", total);
        exit(0);
    }
    total = to;
}

void check(Tree *tree){

    if (tree == NULL) return;

    check(tree->left);
    if (last >= tree->key){
        printf("Err between nodes %d and %d\n", last, tree->key);
        exit(0);
    }
    last = tree->key;
    total--;
    check(tree->right);
}

