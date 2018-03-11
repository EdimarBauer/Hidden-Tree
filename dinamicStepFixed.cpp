/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: March 11, 2018
*
*/

#include <bits/stdc++.h>

using namespace std;
#define N (1 << 24)

int lower, bound, last, total;

struct Tree{
    int key;
    struct Tree *left, *right;
};
Tree *neg;

Tree* newNode(int x);
Tree* search(Tree *tree, int x);

void propagationKey(Tree *tree);
bool existKey(Tree *tree, int x);
void add(Tree *root, int x);
void insert(Tree *&root, int x);

int replace(Tree **tree, bool left);
void deletar(Tree **tree, int x);
void remove(Tree *&root, int x);

void print(Tree *tree);
void printar(Tree *tree, int i);
void check_is_correct(Tree *tree);
void check(Tree *tree);


int main(){

    Tree* tree = NULL;
    int x;

    for (int j = 0; j < 2; j++){
        puts("Inserting...");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            if (rand()%2) x = -x;
            //printf("Inserting %d\n", x);
            insert(tree, x);
            //print(tree);
            //check_is_correct(tree);
        }
        //check_is_correct(tree);
        puts("Removing...");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            if (rand()%2) x = -x;
            //printf("Removing %d\n", x);
            remove(tree, x);
            //print(tree);
            //check_is_correct(tree);
        }
        //check_is_correct(tree);
    }
    //print(tree);
    //puts("");
}

//###################################################################################################

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

Tree* search(Tree *tree, int x){

    if (x < 0) tree = neg;

    while(tree != NULL){
        if (tree->key == x) return tree;
        if (x < tree->key) tree = tree->left;
        else tree = tree->right;
    }
    return NULL;
}




void propagationKey(Tree *tree){

    if (tree->key < 0){
        while(tree->right != NULL and tree->right->key < tree->key){
            swap(tree->key, tree->right->key);
            tree = tree->right;
        }
    }else{
        while(tree->left != NULL and tree->left->key > tree->key){
            swap(tree->key, tree->left->key);
            tree = tree->left;
        }
    }
}

bool existKey(Tree *tree, int x){

    if (x < 0) while(tree && tree->key < x) tree = tree->right;
    else while(tree && tree->key > x) tree = tree->left;

    if (tree && tree->key == x) return true;
    return false;
}

void add(Tree *root, int x){

    int middle, jump;
    if (x < 0){
        middle = lower / 2;
        jump = -(middle / 2);
    }else{
        middle = bound / 2;
        if (bound == INT_MAX) middle++;
        jump = middle / 2;
    }
    bool left = 0, right = 0;
    Tree *tree = root;

    while(tree->key != x){
        if (x < middle){
            if (x > tree->key){
                if (x < 0 && left == 0 && existKey(tree->right, x)) break;
                swap(x, tree->key);
                if (left == 0) propagationKey(tree);
            }
            if (x < 0 && x <= middle*2){
                add(root, x);
                break;
            }

            if (tree->left == NULL){
                tree->left = newNode(x);
                break;
            }
            tree = tree->left;
            middle -= jump;
            jump >>= 1;
            left = 1;
        }else if (x > middle){
            if (x < tree->key){
                if (x >= 0 && right == 0 && existKey(tree->left, x)) break;
                swap(x, tree->key);
                if (right == 0) propagationKey(tree);
            }
            if (x >= 0 && x >= middle*2){
                add(root, x);
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
            if (x >= 0 && right == 0 && x < tree->key && existKey(tree->left, x)) break;
            else if (x < 0 && left == 0 && x > tree->key && existKey(tree->right, x)) break;
            swap(x, tree->key);
            propagationKey(tree);
        }
    }
}

void insert(Tree *&root, int x){

    if (x < 0){
        if (neg == NULL){
            neg = newNode(x);
            lower = -2;
        }else if ( (x <= lower && x != INT_MIN) || (neg->key <= lower && neg->key != INT_MIN) ){
            Tree *head = newNode(x);
            head->right = neg;
            neg = head;
            propagationKey(neg);
            lower *= 2;
        }else add(neg, x);
    }else{
        if (root == NULL){
            root = newNode(x);
            bound = 2;
        }else if (x >= bound || root->key >= bound){
            Tree *head = newNode(x);
            head->left = root;
            root = head;
            propagationKey(root);
            if (bound == (1 << 30)) bound = INT_MAX;
            else bound *= 2;
        }else add(root, x);
    }
}



int replace(Tree **tree, bool left){

    int x;
    if (left){
        while((*tree)->right != NULL) tree = &((*tree)->right);

        x = (*tree)->key;
        if ((*tree)->left == NULL){
            free(*tree);
            *tree = NULL;
            total--;
        }else (*tree)->key = replace(&((*tree)->left), 1);
    }else{
        while((*tree)->left != NULL) tree = &((*tree)->left);

        x = (*tree)->key;
        if ((*tree)->right == NULL){
            free(*tree);
            *tree = NULL;
            total--;
        }else (*tree)->key = replace(&((*tree)->right), 0);
    }

    return x;
}

void deletar(Tree **tree, int x){

    while((*tree) != NULL && (*tree)->key != x){
        if (x < (*tree)->key) tree = &((*tree)->left);
        else tree = &((*tree)->right);
    }

    if (*tree != NULL){
        if ((*tree)->left == NULL && (*tree)->right == NULL){
            free(*tree);
            *tree = NULL;
            total--;
        }else if ((*tree)->right != NULL) (*tree)->key = replace(&((*tree)->right), 0);
        else (*tree)->key = replace(&((*tree)->left), 1);
    }
}

void remove(Tree *&root, int x){

    if (x < 0){
        if (neg == NULL) return;
        if (neg->key == x && neg->right == NULL && neg->left == NULL){
            free(neg);
            neg = NULL;
            total--;
            return;
        }
        deletar(&neg, x);
    }else{
        if (root == NULL) return;
        if (root->key == x && root->left == NULL && root->right == NULL){
            free(root);
            root = NULL;
            total--;
            return;
        }
        deletar(&root, x);
    }
}





void print(Tree *tree){

    printar(neg, 0);
    printar(tree, 0);
}

void printar(Tree *tree, int i){

    if (tree == NULL) return;

    printar(tree->left, i+1);
    printf("%d  height: %d\n", tree->key, i);
    printar(tree->right, i+1);
}

void check_is_correct(Tree *tree){

    last = INT_MIN;
    int to = total;
    check(neg);
    check(tree);
    if (total != 0) printf("Diference of %d elements\n", total);
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

