/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: December 18, 2017
*
* Updated: February 21, 2018
* Updated: April, 06, 2018
*
* Consider no lazy deletion and positive integer key
*
* The variable 'last' and 'total' serve to test the algorithm
*
*/

#include <bits/stdc++.h>

using namespace std;
#define N (1 << 23)

int bound;
int last, total;

struct Tree{
    int key;
    struct Tree *left, *right;
};

unsigned int random();
Tree* newNode(int x);
int nextPow(int x);
Tree* search(Tree* tree, int &x);

Tree* makeHead(Tree* node, int x);
void createMiddleNode(Tree *tree, int x);
void add(Tree* tree, int &x);
void insert(Tree *&tree, int &x);

int replace(Tree *&tree, bool left);
void remove(Tree *&root, int x);

void print(Tree *tree, int height);
void check_is_correct(Tree *tree);
void check(Tree *tree);



int main(){

    Tree* tree = NULL;
    int x;

    for (int j = 0; j < 1; j++){
        printf("Inserting...\n");
        for (int i = 0; i < N; i++){
            //x = rand() % N;
            x = random();
            insert(tree, x);
            //check_is_correct(tree);
        }

        printf("Removing...\n");
        for (int i = 0; i < 0; i++){
            x = rand() % N;
            remove(tree, x);
            //check_is_correct(tree);
        }
    }

    //print(tree);
}



//#####################################################################################################
//#####################################################################################################


unsigned int random(){

    unsigned int x = 0;
    for (int i = 0; i < 31; i++){
        x += (rand()%2) << i;
    }
    //printf("%u\n", x);
    return x;
}


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




Tree* makeHead(Tree* node, int x){
    bound = nextPow(x);
    Tree* head = newNode(x);
    head->left = node;
    return head;
}

void createMiddleNode(Tree *tree, int x){
    Tree *child = tree->left;
    tree->left = newNode(x);
    tree->left->left = child;
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
                tree->left = newNode(x);
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

void insert(Tree *&tree, int &x){

    if (tree == NULL){
        bound = nextPow(x);
        tree = newNode(x);
    }else if (bound <= x) tree = makeHead(tree, x);
    else add(tree, x);
}




int replace(Tree *&tree, bool left){

    int x;
    if (left){
        if (tree->right == NULL){
            x = tree->key;
            if (tree->left == NULL){
                free(tree);
                tree = NULL;
            }else tree->key = replace(tree->left, 1);
        }else x = replace(tree->right, 1);
    }else{
        if (tree->left == NULL){
            x = tree->key;
            if (tree->right == NULL){
                free(tree);
                tree = NULL;
            }else tree->key = replace(tree->right, 0);
        }else x = replace(tree->left, 0);
    }

    return x;
}

void remove(Tree *&root, int x){

    if (root == NULL) return;
    Tree **tree = &root;
    bool right = 0;

    while(*tree != NULL){
        if ((*tree)->key == x){
            if ((*tree)->left == NULL && (*tree)->right == NULL){
                free(*tree);
                *tree = NULL;
            }else{
                if ((*tree)->right != NULL) (*tree)->key = replace((*tree)->right, 0);
                else {
                    if (right == 0){
                        Tree *aux = *tree;
                        if (aux == root) bound = nextPow(aux->left->key);
                        *tree = (*tree)->left;
                        free(aux);
                    }else (*tree)->key = replace((*tree)->left, 1);
                }
            }
            total--;
            return;
        }
        if (x < (*tree)->key) tree = &((*tree)->left);
        else {
            right = 1;
            tree = &((*tree)->right);
        }
    }
}





void print(Tree *tree, int height){

    if (tree == NULL) return;

    print(tree->left, height+1);
    cout << tree->key << " " << height << endl;
    print(tree->right, height+1);
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

