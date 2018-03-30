/*
* Author: Edimar Bauer
* Date: March 29, 2018
*/

#include <bits/stdc++.h>

using namespace std;
#define T (1 << 31)
#define N (1 << 23)

priority_queue<unsigned int, vector<unsigned int>, greater<unsigned int> > fila;
int total;
unsigned int last;

struct Tree{
    unsigned int key;
    Tree *left, *right;
};

unsigned int random();
Tree* newNode(unsigned int x);
Tree* search(Tree *tree, unsigned int x);
void insert(Tree *&root, unsigned int x);
int replace(Tree **tree);
void remove(Tree *&root, unsigned int x);

void check_is_correct(Tree *tree);
void check(Tree *tree);
void print(Tree *tree);
void printar(Tree *tree, int i);

int main(){

    Tree *tree = NULL;
    int x;

    for (int j = 0; j < 1; j++){
        printf("Inserting...\n");
        for (int i = 0; i < N; i++){
            //x = rand() % N;
            x = random();
            //printf("Inserting %d\n", x);
            insert(tree, x);
            //print(tree, 0);
            //puts("");
            //check_is_correct(tree);
        }
        //print(tree);
        //check_is_correct(tree);
        printf("Removing...\n");
        for (int i = 0; i < 0; i++){
            x = rand() % N;
            //printf("Removing %d\n", x);
            remove(tree, x);
            //print(tree, 0);
            //puts("");
            //check_is_correct(tree);
        }
        //print(tree);
        //check_is_correct(tree);
    }
    //printar(tree, 0);
}

//#####################################################################################################
//#####################################################################################################


unsigned int random(){

    unsigned int x = 0;
    for (int i = 0; i < 32; i++){
        x += (rand()%2) << i;
    }
    //printf("%u\n", x);
    return x;
}

Tree* newNode(unsigned int x){

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

Tree* search(Tree* tree, unsigned int x){

    unsigned int refe = N >> 1;
    while(tree != NULL){
        if (x == tree->key) return tree;

        if (x & refe) tree = tree->right;
        else tree = tree->left;

        refe >>= 1;
    }

    return NULL;
}





void insert(Tree *&root, unsigned int x){

    if (root == NULL){
        root = newNode(x);
        return;
    }
    Tree *tree = root;
    unsigned int refe = T;

    while(tree->key != x){
        if (x & refe){
            if (tree->right == NULL){
                tree->right = newNode(x);
                return;
            }
            tree = tree->right;
        }else{
            if (tree->left == NULL){
                tree->left = newNode(x);
                return;
            }
            tree = tree->left;
        }
        refe >>= 1;
    }
}

int replace(Tree **tree){
    do{
        if ((*tree)->left != NULL) tree = &((*tree)->left);
        else if ((*tree)->right != NULL) tree = &((*tree)->right);
        else{
            int x = (*tree)->key;
            free(*tree);
            *tree = NULL;
            return x;
        }
    }while(1);
}

void remove(Tree *&root, unsigned int x){

    Tree **tree = &root;
    unsigned int refe = N >> 1;

    while(*tree != NULL){
        if (x == (*tree)->key){
            if ((*tree)->left == NULL && (*tree)->right == NULL){
                free(*tree);
                *tree = NULL;
            }else (*tree)->key = replace(tree);
            total--;
            break;
        }else
            if (x & refe) tree = &((*tree)->right);
            else tree = &((*tree)->left);

        refe >>= 1;
    }
}




void printar(Tree *tree, int i){

    if (tree == NULL) return;

    printar(tree->left, i+1);
    printf("%u  alt: %d\n", tree->key, i);
    printar(tree->right, i+1);
}

void print(Tree* tree){

    if (tree == NULL) return;

    fila.push(tree->key);
    print(tree->left);
    print(tree->right);
    while(!fila.empty() && tree->key >= fila.top()){
        cout << fila.top() << " ";
        fila.pop();
    }
}


void check_is_correct(Tree *tree){

    last = 0;
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

    fila.push(tree->key);
    check(tree->left);
    check(tree->right);
    while(!fila.empty() && tree->key >= fila.top()){
        if (last && fila.top() <= last){
            printf("ERR. Discontinue values in %u and %u\n", last, fila.top());
            exit(0);
        }
        last = fila.top();
        fila.pop();
    }
    total--;
}

