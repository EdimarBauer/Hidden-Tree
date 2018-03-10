/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: March 07, 2018
*
* Update: March 10, 2018
*/

#include <bits/stdc++.h>

using namespace std;
#define N (1 << 23)

int MIDDLE;
long long JUMP;
int last, total;

struct Tree{
    int key;
    struct Tree *left, *right;
};

Tree *headLower, *headUpper;

Tree* newNode(int x);
Tree* search(Tree *tree, int x);
void propagationKey(Tree *tree, bool left);

Tree* makeHead(Tree *tree, int x);
void insert(Tree *&root, int x);

int replace(Tree **tree, bool left);
void remove(Tree **tree, int x);

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
            if (rand()%2) x = -x;
            //printf("Inserting %d\n", x);
            insert(tree, x);
            //print(tree, 0);
            //check_is_correct(tree);
        }
        //check_is_correct(tree);
        printf("Removing...\n");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            if (rand()%2) x = -x;
            //printf("Removing %d\n", x);
            remove(&tree, x);
            //print(tree, 0);
            //check_is_correct(tree);
        }
        //check_is_correct(tree);
    }
    //check_is_correct(tree);
    //print(tree);
}



//#####################################################################################################
//#####################################################################################################





Tree* newNode(int x){

    Tree *node = (Tree*) malloc(sizeof(Tree));
    if (node == NULL){
        puts("Err of allocation");
        exit(0);
    }
    node->key = x;
    node->left = node->right = NULL;
    total++;
    return node;
}

Tree* search(Tree* tree, int x){

    while(tree != NULL){
        if (tree->key == x) return tree;

        if (x < tree->key) tree = tree->left;
        else tree = tree->right;
    }

    return NULL;
}






void propagationKey(Tree *tree, bool left){

    if (left){
        while(tree->left && tree->left->key > tree->key){
            swap(tree->key, tree->left->key);
            tree = tree->left;
        }
        if (tree->left && tree->left->right){
            Tree *aux = tree->left->right;
            while(aux->right) aux = aux->right;
            if (aux->key > tree->key){
                swap(tree->key, aux->key);
                propagationKey(aux, 1);
            }
        }
    }else{
        while(tree->right && tree->right->key < tree->key){
            swap(tree->key, tree->right->key);
            tree = tree->right;
        }
        if (tree->right && tree->right->left){
            Tree *aux = tree->right->left;
            while(aux->left) aux = aux->left;
            if (aux->key < tree->key){
                swap(tree->key, aux->key);
                propagationKey(aux, 0);
            }
        }
    }
}

Tree* makeHead(Tree* tree, int x){

    Tree* root;
    if (x <= MIDDLE - JUMP){
        if (headLower->key <= MIDDLE - JUMP){
            if (headLower->key <= x){
                if (search(headLower, x)) return tree;    //nothing to do
                swap(x, headLower->key);
                propagationKey(headLower, 0);
            }
        }
        root = newNode(x);
        root->right = tree;
        MIDDLE -= JUMP;
        JUMP *= 2;
        if (x <= MIDDLE - JUMP) headLower = root;
    }else{
        if (headUpper->key >= MIDDLE + JUMP){
            if (x <= headUpper->key){
                if (search(headUpper, x)) return tree;    //nothing to do
                swap(x, headUpper->key);
                propagationKey(headUpper, 1);
            }
        }
        root = newNode(x);
        root->left = tree;
        MIDDLE += JUMP;
        JUMP *= 2;
        if (x >= MIDDLE + JUMP ) headUpper = root;
    }

    return root;
}

void insert(Tree *&root, int x){

    if (root == NULL) {
        root = newNode(x);
        MIDDLE = x;
        JUMP = 1;
        headLower = headUpper = root;
    }else if (x >= MIDDLE + JUMP || x <= MIDDLE - JUMP ) root = makeHead(root, x);
    else{
        Tree *tree = root;
        int middle = MIDDLE;
        int jump = JUMP / 2;
        while(tree->key != x){
            if (x < middle){
                if (x > tree->key){
                    if (search(tree->right, x)) break;
                    swap(tree->key, x);
                    propagationKey(tree, 0);
                }
                if (x <= middle - (long long)jump*2 ){
                    insert(root, x);
                    break;
                }
                if (tree->left == NULL){
                    tree->left = newNode(x);
                    break;
                }
                tree = tree->left;
                //threat int over flow
                if ((long long)middle - jump < INT_MIN) {
                    int dif = middle - INT_MIN;
                    while(jump >= dif) jump /= 2;
                }
                middle -= jump;
                jump /= 2;
            }else if (x > middle){
                if (x < tree->key){
                    if (search(tree->left, x)) break;
                    swap(tree->key, x);
                    propagationKey(tree, 1);
                }
                if (x >= middle + (long long)jump*2 ){
                    insert(root, x);
                    break;
                }
                if (tree->right == NULL){
                    tree->right = newNode(x);
                    break;
                }
                tree = tree->right;
                if ((long long)middle + jump > INT_MAX) {
                    int dif = INT_MAX - middle;
                    while(jump >= dif) jump /= 2;
                }
                middle += jump;
                jump /= 2;
            }else{
                if (search(tree, x)) break;
                swap(tree->key, x);
                if (x > middle) propagationKey(tree, 1);
                else propagationKey(tree, 0);
            }
        }
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

void remove(Tree **tree, int x){

    while(*tree != NULL){
        if (x == (*tree)->key){
            if ((*tree)->left == NULL && (*tree)->right == NULL){
                free(*tree);
                *tree = NULL;
                total--;
            }else{
                if ((*tree)->right != NULL) (*tree)->key = replace(&((*tree)->right), 0);
                else (*tree)->key = replace(&((*tree)->left), 1);
                break;
            }
        }else if (x < (*tree)->key) tree = &((*tree)->left);
        else tree = &((*tree)->right);
    }
}





void print(Tree *tree, int i){

    if (tree == NULL) return;

    print(tree->left, i+1);
    cout << tree->key << " " << i << endl;
    print(tree->right, i+1);
}

void check_is_correct(Tree *tree){

    last = -INT_MAX;
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

