/*
* Create by: Edimar Jacob Bauer
* E-mail: edimarjb@gmail.com
* Data: 19/12/2017
*
* Update: February 21, 2018
* Update: March 03, 2018
* Update: March 11, 2018
*
* Consider unique key
*/
#include <bits/stdc++.h>

using namespace std;
#define N (1 << 24)

priority_queue<int, vector<int>, greater<int> > fila;
int total, last;

struct Tree{
    int key;
    bool active;
    struct Tree *left, *right;
};

Tree* newNode(int x);
bool findKey(Tree *tree, int x, int low, int high);

Tree* search(Tree *tree, int x, int low, int high);
void insert(Tree *&root, int x, int low, int high);
void remove(Tree *&tree, int x, int low, int high);

void check_is_correct(Tree *tree);
void check(Tree *tree, int low, int high);
void print(Tree *tree);
void printar(Tree *tree, int i);

int main(){

    Tree* tree = NULL;
    int x, low = -N, high = N;

    for (int j = 0; j < 2; j++){
        puts("Inserting...");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            if (rand()%2) x = -x;
            //printf("Inserting %d\n", x);
            insert(tree, x, low, high);
            //printar(tree, 0);
            //check_is_correct(tree);
        }
        //check_is_correct(tree);
        puts("Removing...");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            if (rand()%2) x = -x;
            //printf("Removing %d\n", x);
            remove(tree, x, low, high);
            //check_is_correct(tree);
        }
        //check_is_correct(tree);
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
    tree->active = 1;
    tree->left = tree->right = NULL;
    total++;
    return tree;
}

bool findKey(Tree* tree, int x, int low, int high){

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

        if (tree == NULL) return false; //means the element was not found, so it will be inserted
    }while(tree->key != x || !tree->active);

    return true;  //means the element already exists in the tree, nothing to do
}





Tree* search(Tree* tree, int x, int low, int high){

    int middle;
    while(tree != NULL){
        if (x == tree->key && tree->active) return tree;

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

void insert(Tree *&root, int x, int low, int high){

    if (root == NULL){
        root = newNode(x);
        return;
    }
    Tree *tree = root;
    int middle;

    while(tree->key != x){
        if (!tree->active){
            //look for the occurrence of x under the tree
            if (!findKey(tree, x, low, high)){
                tree->key = x;
                tree->active = 1;
            }
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

void remove(Tree *&tree, int x, int low, int high){

    if (tree == NULL) return;

    if (x == tree->key && tree->active) tree->active = 0;
    else{
        int middle = (low + high) / 2;
        if (x <= middle) remove(tree->left, x, low, middle);
        else remove(tree->right, x, middle, high);
    }

    if (!tree->active && tree->left == NULL && tree->right == NULL){
        free(tree);
        tree = NULL;
        total--;
    }
}





void print(Tree* tree){

    if (tree == NULL) return;

    if (tree->active) fila.push(tree->key);
    print(tree->left);
    print(tree->right);
    if (tree->active){
        while(!fila.empty() && tree->key >= fila.top()){
            cout << fila.top() << " ";
            fila.pop();
        }
    }
}

void printar(Tree *tree, int i){

    if (tree == NULL) return;

    printar(tree->left, i+1);
    if (tree->active) printf("%d  alt: %d\n", tree->key, i);
    else printf("%d  alt: %d   deleted\n", tree->key, i);
    printar(tree->right, i+1);
}

void check_is_correct(Tree *tree){

    last = INT_MIN;
    int to = total;
    check(tree, -N, N);
    if (total != 0){
        printf("Diference of %d elements\n", total);
        exit(0);
    }
    total = to;
}

void check(Tree *tree, int low, int high){

    if (tree == NULL) return;

    int middle = (low + high) / 2;
    check(tree->left, low, middle);
    if (tree->active){
        if (tree->key <= low || tree->key >= high){
            printf("Err. %d out of your interval. Low: %d   High: %d\n", tree->key, low, high);
            exit(0);
        }
    }
    total--;
    check(tree->right, middle, high);
}

