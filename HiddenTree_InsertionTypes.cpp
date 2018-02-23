/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: November 23, 2017
*
* Update: February 21, 2018
*
* Consider positive integer key
*
* Here has 4 insertion functions types. 3 functions dinamic and 1 with fixed interval
*/

#include <stdlib.h>
#include <iostream>
#include <stdio.h>

using namespace std;
#define N (1 << 24)

int bound = 2, last, total;

struct Tree{
    int key;
    struct Tree *left, *right;
};

Tree *newNode(int x);
int nextPow(int x);

void propagationKey(Tree *tree);
int findKey(Tree *tree, int x, int middle, int jump);
bool existKey(Tree *tree, int x);

void insertionDinamic(Tree *&root, int x);
void insertionStep(Tree *&root, int x);
void insertionMemory(Tree *&root, int x);
void insertionFixed(Tree *&root, int x);

void print(Tree *tree);
void print(Tree *tree, int i);
void check_is_correct(Tree *tree);
void check(Tree *tree);


int main(){

    Tree* tree = NULL;
    int x;

    for (int i = 0; i < N; i++){
        x = rand() % N;
        insertionDinamic(tree, x);
        //insertionStep(tree, x);
        //insertionMemory(tree, x);
        //insertionFixed(tree, x);
        //check_is_correct(tree);
    }
    //print(tree);
    //puts("");
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

Tree* newNode(int x){

    Tree *tree = (Tree*) malloc(sizeof(Tree));
    if (tree == NULL){
        puts("Err of alocation");
        exit(0);
    }
    tree->key = x;
    tree->left = tree->right = NULL;
    total++;
    return tree;
}

int nextPow(int x){

    x |= (x>>1);
    x |= (x>>2);
    x |= (x>>4);
    x |= (x>>8);
    x |= (x>>16);

    return x+1;
}





void propagationKey(Tree *tree){

    while(tree->left != NULL and tree->left->key > tree->key){
        swap(tree->key, tree->left->key);
        tree = tree->left;
    }
}

int findKey(Tree *tree, int x, int middle, int jump){

    do{
        if (x < middle){
            tree = tree->left;
            middle -= jump;
        }else{
            tree = tree->right;
            middle += jump;
        }
        jump <<= 1;

        if (tree == NULL) return x;
    }while(tree->key != x);

    return -1;
}

bool existKey(Tree *tree, int x){

    while(tree && tree->key >= x){
        if (tree->key == x) return true;
        tree = tree->left;
    }
    return false;
}




void insertionDinamic(Tree *&root, int x){

    if (root == NULL){
        bound = nextPow(x);
        root = newNode(x);
        return;
    }else if (bound <= x) {
        bound = nextPow(x);
        Tree* head = newNode(x);
        head->left = root;
        root = head;
        return;
    }
    Tree *tree = root;
    int middle = bound >> 1;
    int jump = middle >> 1;
    int limit;
    bool right = 0;

    while(tree->key != x){
        if (x < middle){
            if (x > tree->key) swap(x, tree->key);

            if (tree->left == NULL){
                tree->left = newNode(x);
                break;
            }
            if (right == 0 && tree->left->key < jump){
                //has a missing interval between the nodes, make node between then or jump to the next limit
                limit = nextPow(tree->left->key);
                if (x >= limit){
                    Tree *child = tree->left;
                    tree->left = newNode(x);
                    tree->left->left = child;
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
                tree->right = newNode(x);
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

void insertionStep(Tree *&root, int x){

    if (root == NULL){
        root = newNode(x);
        return;
    }
    if (x >= bound or root->key >= bound){
        if (x <= root->key && findKey(root, x, bound/2, bound/4) == -1) return;
        Tree *head;
        if (x > root->key) head = newNode(x);
        else{
            head = newNode(root->key);
            root->key = x;
            propagationKey(root);
        }
        head->left = root;
        root = head;
        bound <<= 1;
        return;
    }

    Tree *tree = root;
    int middle = bound>>1;
    int jump = middle>>1;
    bool right = 0;

    while(tree->key != x){
        if (x < middle){
            if (x > tree->key) swap(x, tree->key);

            if (tree->left == NULL){
                tree->left = newNode(x);
                break;
            }
            tree = tree->left;
            middle -= jump;
            jump >>= 1;
        }else if (x > middle){
            if (x < tree->key){
                if (right == 0 && existKey(tree->left, x)) break;
                swap(x, tree->key);
                if (right == 0) propagationKey(tree);
            }
            if (x >= (middle << 1)){
                insertionStep(root, x);
                break;
            }
            if (tree->right == NULL){
                tree->right = newNode(x);
                break;
            }
            tree = tree->right;
            middle += jump;
            jump >>= 1;
            right = 1;
        }else{
            if (right == 0 && x < tree->key && existKey(tree->left, x)) break;
            swap(x, tree->key);
            propagationKey(tree);
        }
    }
}

void insertionMemory(Tree *&root, int x){

    if (root == NULL){
        bound = nextPow(x);
        root = newNode(x);
        return;
    }
    Tree *tree;
    if (x >= bound){
        do{
            tree = newNode(-1);
            tree->left = root;
            root = tree;
            bound <<= 1;
        }while(x >= bound);

        root->key = x;
        return;
    }

    tree = root;
    int middle = bound >> 1;
    int jump = middle >> 1;

    while(tree->key != x){
        if (x < middle){
            if (x > tree->key && tree->key != -1) swap(x, tree->key);

            if (tree->left == NULL){
                tree->left = newNode(x);
                break;
            }
            tree = tree->left;
            middle -= jump;
        }else if (x > middle){
            if (tree->key == -1){
                tree->key = x;
                break;
            }
            if (x < tree->key) swap(x, tree->key);

            if (tree->right == NULL){
                tree->right = newNode(x);
                break;
            }
            tree = tree->right;
            middle += jump;
        }else{
            if (tree->key == -1){
                tree->key = x;
                break;
            }
            swap(x, tree->key);
            continue;
        }

        jump >>= 1;
    }
}

void insertionFixed(Tree *&root, int x){

    if (root == NULL){
        root = newNode(x);
        return;
    }

    Tree *tree = root;
    int middle = N>>1;
    int jump = middle>>1;

    while(tree->key != x){
        if (x < middle){
            if (x > tree->key) swap(x, tree->key);

            if (tree->left == NULL){
                tree->left = newNode(x);
                break;
            }
            tree = tree->left;
            middle -= jump;
        }else if (x > middle){
            if (x < tree->key) swap(x, tree->key);

            if (tree->right == NULL){
                tree->right = newNode(x);
                break;
            }
            tree = tree->right;
            middle += jump;
        }else{
            swap(x, tree->key);
            continue;
        }
        jump >>= 1;
    }
}






void print(Tree *tree){

    if (tree == NULL) return;

    print(tree->left);
    printf("%d ", tree->key);
    print(tree->right);
}

void print(Tree *tree, int i){

    if (tree == NULL) return;

    print(tree->left, i+1);
    printf("%d  height: %d\n", tree->key, i);
    print(tree->right, i+1);
}

void check_is_correct(Tree *tree){

    last = -1;
    int to = total;
    check(tree);
    if (total != 0) printf("Diference of %d elements\n", total);
    total = to;
}

void check(Tree *tree){

    if (tree == NULL) return;

    check(tree->left);
    if (tree->key != -1){
        if (last >= tree->key){
            printf("Err between nodes %d and %d\n", last, tree->key);
            exit(0);
        }
        last = tree->key;
    }
    total--;
    check(tree->right);
}

