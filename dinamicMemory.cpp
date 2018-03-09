/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: March 06, 2018
*/

#include <bits/stdc++.h>

using namespace std;
#define N (1 << 24)

int MIDDLE, JUMP;
int last, total;

struct Tree{
    int key;
    bool active;
    struct Tree *left, *right;
};

Tree* newNode(int x);
Tree* search(Tree* tree, int x, int middle, int jump);

Tree* makeHead(Tree* node, int x);
void insert(Tree *&tree, int x);

int replace(Tree *&tree, bool left, int middle, int jump);
void remove(Tree *&tree, int &x, int middle, int jump);

void print(Tree *tree, int i, int middle, int jump);
void check_is_correct(Tree *tree);
void check(Tree *tree);



int main(){

    Tree* tree = NULL;
    int x, neg;

    for (int j = 0; j < 2; j++){
        printf("Inserting...\n");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            neg = rand() % 2;
            if (neg) x = -x;
            //printf("Inserting %d\n", x);
            insert(tree, x);
            //print(tree, 0, MIDDLE, JUMP/2);
            //check_is_correct(tree);
        }

        printf("Removing...\n");
        for (int i = 0; i < N; i++){
            x = rand() % N;
            neg = rand() % 2;
            if (neg) x = -x;
            //printf("Removing %d\n", x);
            remove(tree, x, MIDDLE, JUMP / 2);
            //print(tree, 0, MIDDLE, JUMP/2);
            //check_is_correct(tree);
        }
    }
    //check_is_correct(tree);
    //print(tree, 0, MIDDLE, JUMP/2);
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
    tree->active = true;
    tree->left = tree->right = NULL;
    total++;
    return tree;
}

Tree* search(Tree* tree, int x, int middle, int jump){

    while(tree != NULL){
        if (tree->key == x && tree->active) return tree;

        if (x < middle){
            tree = tree->left;
            middle -= jump;
        }else if (x > middle){
            tree = tree->right;
            middle += jump;
        }else break;
        jump /= 2;
    }

    return NULL;
}





Tree* makeHead(Tree* tree, int x){

    Tree* root;
    if (x <= MIDDLE - JUMP){
        do{
            root = newNode(x);
            root->right = tree;
            MIDDLE -= JUMP;
            JUMP *= 2;
            if (x <= MIDDLE - JUMP){
                root->active = false;
                tree = root;
            }else break;
        }while(1);
    }else{
        do{
            root = newNode(x);
            root->left = tree;
            MIDDLE += JUMP;
            JUMP *= 2;
            if (x >= MIDDLE + JUMP){
                root->active = false;
                tree = root;
            }else break;
        }while(1);
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
        //printf("middle: %d  jump: %d\n", middle, jump);
        while(tree->key != x){
            if (x < middle){
                if (!tree->active && tree->left == NULL){
                    tree->key = x;
                    break;
                }
                if (x > tree->key && tree->active) swap(x, tree->key);
                if (tree->left == NULL){
                    tree->left = newNode(x);
                    break;
                }
                tree = tree->left;
                middle -= jump;
                jump /= 2;
            }else if (x > middle){
                if (!tree->active && tree->right == NULL){
                    tree->key = x;
                    break;
                }
                if (x < tree->key && tree->active) swap(x, tree->key);
                if (tree->right == NULL){
                    tree->right = newNode(x);
                    break;
                }
                tree = tree->right;
                middle += jump;
                jump /= 2;
            }else swap(x, tree->key);
        }
    }
}





int replace(Tree *&tree, bool left, int middle, int jump){

    int x;
    if (left){
        if (tree->right == NULL){
            if (tree->active){
                x = tree->key;
                if (tree->left == NULL){
                    free(tree);
                    tree = NULL;
                    total--;
                    return x;
                }else tree->key = replace(tree->left, 1, middle - jump, jump / 2);
            }else x = replace(tree->left, 1, middle - jump, jump / 2);
        }else x = replace(tree->right, 1, middle + jump, jump / 2);
    }else{
        if (tree->left == NULL){
            if (tree->active){
                x = tree->key;
                if (tree->right == NULL){
                    free(tree);
                    tree = NULL;
                    total--;
                    return x;
                }else tree->key = replace(tree->right, 0, middle + jump, jump / 2);
            }else x = replace(tree->right, 0, middle + jump, jump / 2);
        }else x = replace(tree->left, 0, middle - jump, jump / 2);
    }

    if (!tree->active && tree->left == NULL && tree->right == NULL){
        free(tree);
        tree = NULL;
        total--;
    }

    return x;
}

void remove(Tree *&tree, int &x, int middle, int jump){

    if (tree == NULL) return;

    if (x == tree->key && tree->active){
        if (tree->left == NULL && tree->right == NULL){
            free(tree);
            tree = NULL;
            total--;
            return;
        }else{
            if (tree->right != NULL) tree->key = replace(tree->right, 0, middle + jump, jump / 2);
            else tree->key = replace(tree->left, 1, middle - jump, jump / 2);
        }
    }else if (x < middle) remove(tree->left, x, middle - jump, jump / 2);
    else if (x > middle) remove(tree->right, x, middle + jump, jump / 2);
    else return;

    if (!tree->active && tree->left == NULL && tree->right == NULL){
        free(tree);
        tree = NULL;
        total--;
    }
}




void print(Tree *tree, int i, int middle, int jump){

    if (tree == NULL) return;

    print(tree->left, i+1, middle - jump, jump / 2);
    //if (tree->active)
        cout << tree->key << " alt: " << i << ",  middle: " << middle << ",  jump: " << jump << endl;
    print(tree->right, i+1, middle + jump, jump / 2);
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
    if (tree->active){
        if (last >= tree->key){
            printf("Err between nodes %d and %d\n", last, tree->key);
            exit(0);
        }
        last = tree->key;
    }
    total--;
    check(tree->right);
}

