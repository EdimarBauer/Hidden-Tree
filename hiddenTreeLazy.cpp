/*
* Create by: Edimar Bauer
* Data: 19/12/2017
* 
* Consider no key equal -1
*/
#include <iostream>
#include <queue>

using namespace std;
#define SIZE (1 << 24)
priority_queue<int, vector<int>, greater<int> > fila;

typedef struct node{
    int key;
    struct node* left;
    struct node* right;
}Tree;

Tree* aloc(int x){

    Tree *node = (Tree*) malloc(sizeof(Tree));
    if (node == NULL){
        puts("Err of alocation");
        exit(0);
    }

    node->key = x;
    node->left = node->right = NULL;

    return node;
}

int findingKey(Tree* node, int x, int low, int high){

    int middle;
    do{
        middle = (low + high) / 2;

        if (x <= middle){
            node = node->left;
            high = middle;
        }else{
            node = node->right;
            low = middle;
        }

        if (node == NULL) return x;
    }while(node->key != x);

    return -1;
}

void insertion(Tree** tree, int x, int low, int high){

    if (*tree == NULL){
        *tree = aloc(x);
        return;
    }

    Tree *node = *tree;
    int middle;

    while(node->key != x){

        if (node->key == -1){
            node->key = findingKey(node, x, low, high);
            return;
        }

        middle = (low + high) / 2;

        if (x <= middle){
            //base case
            if (node->left == NULL){
                node->left = aloc(x);
                return;
            }
            //down to the left
            node = node->left;
            high = middle;
        }else{
            //base case
            if (node->right == NULL){
                node->right = aloc(x);
                return;
            }
            //down to the right
            node = node->right;
            low = middle;
        }
    }
}

Tree* searching(Tree* node, int x, int low, int high){

    int middle;
    while(node->key != x){
        middle = (low + high) / 2;

        if (x <= middle){
            node = node->left;
            high = middle;
        }else{
            node = node->right;
            low = middle;
        }

        if (node == NULL) return NULL;
    }

    return node;
}

Tree* deletionLazy(Tree* node, int x, int low, int high){

    if (node == NULL) return NULL;

    if (node->key == x){
        node->key = -1;
    }else{
        int middle = (low + high) / 2;
        if (x <= middle){
            node->left = deletionLazy(node->left, x, low, middle);
        }else{
            node->right = deletionLazy(node->right, x, middle, high);
        }
    }

    if (node->left == NULL and node->right == NULL){
        free(node);
        return NULL;
    }

    return node;
}

void print(Tree* node){

    if (node == NULL) return;

    if (node->key != -1) fila.push(node->key);
    print(node->left);
    print(node->right);
    if (node->key != -1){
        cout << fila.top() << " ";
        fila.pop();
    }
}

int main(){

    Tree* tree = NULL;
    int random, low, high;
    low = 0;
    high = SIZE;

    for (int i = 0; i < SIZE; i++){
        random = rand() % SIZE;
        insertion(&tree, random, low, high);
    }

    for (int i = 0; i < SIZE; i++){
        random = rand() % SIZE;
        tree = deletionLazy(tree, random, low, high);
    }

    print(tree);
    puts("");
}
