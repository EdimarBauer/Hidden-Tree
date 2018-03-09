/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: March 07, 2018
*
* Updated: March 09, 2018
*
* Growth dinamic in both directions with jump technic and field jump to mark the quantity of missing nodes between the vertex.
*/

#include <bits/stdc++.h>

using namespace std;
#define N (1 << 24)

int MIDDLE, JUMP;
int last, total;

struct Tree{
    int key;
    char jump;
    struct Tree *left, *right;
};

Tree* newNode(int x);
Tree* search(Tree* tree, int x);

Tree* makeHead(Tree *tree, int x);
void insert(Tree *&tree, int x);

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
    tree->jump = 0;
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





Tree* makeHead(Tree* tree, int x){

    Tree* root;
    if (x <= MIDDLE - JUMP){
        root = newNode(x);
        root->right = tree;
        MIDDLE -= JUMP;
        JUMP *= 2;
        if (x <= MIDDLE - JUMP){
            int k = 0;
            do{
                k++;
                MIDDLE -= JUMP;
                JUMP *= 2;
            }while(x <= MIDDLE - JUMP);
            tree->jump = k;
        }
    }else{
        root = newNode(x);
        root->left = tree;
        MIDDLE += JUMP;
        JUMP *= 2;
        if (x >= MIDDLE + JUMP){
            int k = 0;
            do{
                k++;
                MIDDLE += JUMP;
                JUMP *= 2;
            }while(x >= MIDDLE + JUMP);
            tree->jump = k;
        }
    }

    return root;
}

void insert(Tree *&root, int x){

    if (root == NULL){
        root = newNode(x);
        MIDDLE = x;
        JUMP = 1;
    }else if (x <= MIDDLE - JUMP || x >= MIDDLE + JUMP) root = makeHead(root, x);
    else{
        Tree *tree = root;
        int middle = MIDDLE;
        int jump = JUMP / 2;
        while(tree->key != x){
            if (x < middle){
                if (x > tree->key) swap(x, tree->key);
                if (tree->left == NULL){
                    tree->left = newNode(x);
                    break;
                }
                if (tree->left->jump and x > tree->left->key){
                    //verify if x belongs the missing range
                    int k = -1;
                    Tree *child = tree->left;
                    do{
                        //count the quantity of nodes are missing until x place
                        if (++k == child->jump) break;
                        middle -= jump;
                        jump /= 2;
                    }while(x < middle);

                    if (x >= middle){
                        //needs to insert node between tree and tree->left
                        tree->left = newNode(x);
                        tree = tree->left;
                        tree->left = child;
                        child->jump--;
                        if (k){
                            //update the missing range between nodes
                            tree->jump = k;
                            child->jump -= k;
                        }
                        break;
                    }
                }
                tree = tree->left;
                middle -= jump;
                jump /= 2;
            }else if (x > middle){
                if (x < tree->key) swap(x, tree->key);

                if (tree->right == NULL){
                    tree->right = newNode(x);
                    break;
                }
                if (tree->right->jump and x < tree->right->key){
                    int k = -1;
                    Tree *child = tree->right;
                    do{
                        if (++k == child->jump) break;
                        middle += jump;
                        jump /= 2;
                    }while(x > middle);

                    if (x <= middle){
                        tree->right = newNode(x);
                        tree = tree->right;
                        tree->right = child;
                        child->jump--;
                        if (k){
                            tree->jump = k;
                            child->jump -= k;
                        }
                        break;
                    }
                }
                tree = tree->right;
                middle += jump;
                jump /= 2;
            }else swap(x, tree->key);
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

