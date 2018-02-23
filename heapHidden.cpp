/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: February 23, 2017
*
* The variable 'last' and 'total' serve to test the algorithm
*/

#include <bits/stdc++.h>

using namespace std;
#define N (1 << 25)

int bound;
int last, total;

struct Tree{
    int key;
    struct Tree *left, *right, *parent;
};

Tree *heap;

Tree* newNode(int x);
int nextPow(int x);
Tree* search(Tree* tree, int &x);

Tree* makeHead(Tree* node, int x);
void createMiddleNode(Tree *tree, int x);
void add(Tree* tree, int &x);
void insert(Tree *&tree, int &x);

int replace(Tree *&tree);
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
            insert(tree, x);
            //check_is_correct(tree);
        }

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





Tree* newNode(Tree *parent, int x){

    Tree *tree = (Tree*) malloc(sizeof(Tree));
    if (tree == NULL){
        puts("Err of allocation");
        exit(0);
    }
    tree->key = x;
    tree->left = tree->right = NULL;
    tree->parent = parent;
    total++;
    return tree;
}

//search the superior limit of x that is pow of 2
int nextPow(int x){

    x |= (x>>1);
    x |= (x>>2);
    x |= (x>>4);
    x |= (x>>8);
    x |= (x>>16);

    return x+1;
}

Tree* search(Tree* tree, int &x){

    while(tree != NULL){
        if (tree->key == x) return tree;

        if (x < tree->key) tree = tree->left;
        else tree = tree->right;
    }

    return NULL;
}




Tree* makeHead(Tree* tree, int x){
    bound = nextPow(x);
    Tree* head = newNode(NULL, x);
    head->left = tree;
    tree->parent = head;
    return head;
}

void createMiddleNode(Tree *tree, int x){
    Tree *child = tree->left;
    tree->left = newNode(tree, x);
    tree->left->left = child;
    child->parent = tree->left;
}

void add(Tree *tree, int &x){

    int middle = bound >> 1;
    int jump = middle >> 1;
    int limit;
    bool right = 0;

    while(tree->key != x){
        if (x < middle){
            if (x > tree->key) swap(x, tree->key);

            if (tree->left == NULL){
                tree->left = newNode(tree, x);
                if (right == 0) heap = tree->left;
                break;
            }
            if (right == 0 && tree->left->key < jump){
                //has a missing interval between the nodes, make node between then or jump to the next limit
                limit = nextPow(tree->left->key);
                if (x >= limit){
                    createMiddleNode(tree, x);
                    break;
                }
                middle = limit >> 1;
                jump = middle >> 1;
            }else{
                middle -= jump;
                jump >>= 1;
            }
            tree = tree->left;

        }else if (x > middle){
            if (x < tree->key) swap(x, tree->key);

            if (tree->right == NULL){
                tree->right = newNode(tree, x);
                break;
            }
            tree = tree->right;
            middle += jump;
            jump >>= 1;
            right = 1;

        //else keeps x on the node
        }else swap(x, tree->key);
    }
}

void insert(Tree *&tree, int &x){

    if (tree == NULL){
        bound = nextPow(x);
        tree = newNode(NULL, x);
        heap = tree;
    }else if (bound <= x) tree = makeHead(tree, x);
    else add(tree, x);
}





int replace(Tree *&tree){

    int x;
    if (tree->left == NULL){
        x = tree->key;
        if (tree->right == NULL){
            free(tree);
            tree = NULL;
        }else tree->key = replace(tree->right);
    }else x = replace(tree->left);

    return x;
}

void popHeap(Tree *&tree){

    if (heap == NULL) return;
    total--;
    if (heap->right == NULL){
        if (heap->parent == NULL){
            free(heap);
            heap = NULL;
            tree = NULL;
        }else{
            heap = heap->parent;
            free(heap->left);
            heap->left = NULL;
        }
    }else{
        heap->key = replace(heap->right);
    }
}





void print(Tree *tree, int i){

    if (tree == NULL) return;

    print(tree->left, i+1);
    if (tree->parent)
        cout << tree->key << " pai: " << tree->parent->key << " alt: " << i << endl;
    else
        cout << tree->key << " " << i << endl;
    print(tree->right, i+1);
}

void check_is_correct(Tree *tree){

    last = -1;
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

