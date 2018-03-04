/*
* Create by: Edimar Jacob Bauer
* Data: 19/12/2017
*
* Update: February 21, 2018
* Update: March 03, 2018
*
* Consider positive integer key
*/
#include <iostream>
#include <queue>

using namespace std;
#define N (1 << 14)

priority_queue<int, vector<int>, greater<int> > fila;


struct Tree{
    int key;
    struct Tree *left, *right;
};

Tree* newNode(int x);
int findKey(Tree *tree, int &x, int low, int high);

Tree* search(Tree *tree, int &x, int low, int high);
void insert(Tree *&root, int &x, int low, int high);
void remove(Tree *&tree, int &x, int low, int high);

void print(Tree* tree);

int main(){

    Tree* tree = NULL;
    int x, low = 0, high = N;

    for (int j = 0; j < 2; j++){
        puts("Inserting...");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            insert(tree, x, low, high);
        }

        puts("Removing...");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            remove(tree, x, low, high);
        }
    }
    //print(tree);
    //puts("");
}

//#####################################################################################################
//#####################################################################################################

Tree* newNode(int x){

    Tree *tree = (Tree*) malloc(sizeof(Tree));
    if (tree == NULL){
        puts("Err of alocation");
        exit(0);
    }

    tree->key = x;
    tree->left = tree->right = NULL;
    return tree;
}

int findKey(Tree* tree, int &x, int low, int high){

    int middle;
    do{
        middle = (low + high) / 2;

        if (x <= middle){
            tree = tree->left;
            high = middle;
        }else{
            tree = tree->right;
            low = middle;
        }

        if (tree == NULL) return x; //means the element was not found, so it will be inserted
    }while(tree->key != x);

    return -1;  //means the element already exists in the tree, nothing to do
}





Tree* search(Tree* tree, int &x, int low, int high){

    int middle;
    while(tree != NULL){
        if (x == tree->key) return tree;

        middle = (low + high) / 2;
        if (x <= middle){
            tree = tree->left;
            high = middle;
        }else{
            tree = tree->right;
            low = middle;
        }
    }

    return NULL;
}

void insert(Tree *&root, int &x, int low, int high){

    if (root == NULL){
        root = newNode(x);
        return;
    }
    Tree *tree = root;
    int middle;

    while(tree->key != x){
        if (tree->key == -1){
            //look for the occurrence of x lower the tree
            tree->key = findKey(tree, x, low, high);
            return;
        }

        middle = (low + high) / 2;
        if (x < middle){
            if (tree->left == NULL){
                tree->left = newNode(x);
                return;
            }
            tree = tree->left;
            high = middle;
        }else if (x > middle){
            if (tree->right == NULL){
                tree->right = newNode(x);
                return;
            }
            tree = tree->right;
            low = middle;
        }else{
            swap(x, tree->key);
        }
    }
}

void remove(Tree *&tree, int &x, int low, int high){

    if (tree == NULL) return;

    if (x == tree->key) tree->key = -1;
    else{
        int middle = (low + high) / 2;
        if (x <= middle) remove(tree->left, x, low, middle);
        else remove(tree->right, x, middle, high);
    }

    if (tree->left == NULL and tree->right == NULL){
        free(tree);
        tree = NULL;
    }
}





void print(Tree* tree){

    if (tree == NULL) return;

    if (tree->key != -1) fila.push(tree->key);
    print(tree->left);
    print(tree->right);
    if (tree->key != -1){
        while(!fila.empty() && tree->key >= fila.top()){
            cout << fila.top() << " ";
            fila.pop();
        }
    }
}
