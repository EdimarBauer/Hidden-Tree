/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: November 23, 2017
*/

#include <stdlib.h>
#include <iostream>
#include <stdio.h>

using namespace std;
#define SIZE (1 << 25)

int bound = 2;
int before = -1;

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

int nextPow(int x){

    x |= (x>>1);
    x |= (x>>2);
    x |= (x>>4);
    x |= (x>>8);
    x |= (x>>16);

    return x+1;
}

int hasKeyUnderTree(Tree *node, int x, int middle, int height, bool f){

    do {
        if (x <= middle or (f and (node->left != NULL and node->left->key >= x))){
            node = node->left;
            middle -= height;
        }else{
            node = node->right;
            middle += height;
        }

        if (node == NULL) return x;

        height >>= 1;
    } while(node->key != x);

    return -1;
}

void propagationKey(Tree* node){

    while(node->left != NULL and node->left->key > node->key){
        swap(node->key, node->left->key);
        node = node->left;
    }
}

bool findKey(Tree* node, int x){

    while(node->key != x){
        if (node->left == NULL or (node->key < x and node->key != -1) )
            return false;

        node = node->left;
    }
    return true;
}

Tree* makeHead(Tree* node, int x, bool f){

    if (f){
        if (x < node->key) {
            if (findKey(node, x) == 1) return node;

            swap(x, node->key);
            propagationKey(node);
        }
        bound <<= 1;
    }else bound = nextPow(x);

    Tree* head = aloc(x);
    head->left = node;
    return head;
}

/*
    Função usada para atualizar a descida pra direita. Porém consumiu mais tempo. Questão em aberto!
    Melhor atimizar o tempo ou deixar o código mais legível??
*/
bool updateRight(Tree** node, int &x, int &middle, int height){

    if (x < (*node)->key)
        swap(x, (*node)->key);

    if ((*node)->right == NULL){
        (*node)->right = aloc(x);
        return true;
    }
    (*node) = (*node)->right;
    middle += height;

    return false;
}

Tree* insetionDegree(Tree* node, int x){

    if (node == NULL) return aloc(x);
    if (x >= bound or node->key >= bound) return makeHead(node, x, 1);

    Tree* head = node;
    int middle = bound>>1;
    int height = middle>>1;

    while(node->key != x){
        if (node->key == -1){
            node->key = hasKeyUnderTree(node, x, middle, height, true);
            break;
        }

        if (x == middle) {
            if (findKey(node, x)) break;
            swap(x, node->key);
            propagationKey(node);
        }

        if (x < middle){
            if (x > node->key)swap(x, node->key);

            if (node->left == NULL){
                node->left = aloc(x);
                break;
            }
            node = node->left;
            middle -= height;
        }else{
            if (x < node->key){
                if (findKey(node, x)) break;
                swap(x, node->key);
                propagationKey(node);
            }

            if (x >= (middle << 1)){
                insetionDegree(head, x);
                break;
            }

            if (node->right == NULL){
                node->right = aloc(x);
                break;
            }
            node = node->right;
            middle += height;
        }
        height >>= 1;
    }
    return head;
}

Tree* insertionInterval(Tree* node, int x){

    if (node == NULL){
        bound = nextPow(x);
        return aloc(x);
    }

    if (bound <= x) return makeHead(node, x, 0);

    Tree *head = node;
    int middle = bound >> 1;
    int height = middle >> 1;

    while(node->key != x){
        if (node->key == -1){
            node->key = hasKeyUnderTree(node, x, middle, height, false);
            break;
        }

        if (x == middle)
            swap(x, node->key);

        if (x < middle){
            if (node->left == NULL){
                if (x > node->key)
                    swap(x, node->key);
                node->left = aloc(x);
                break;
            }
            if (node->left->key < height){
                int limit = nextPow(node->left->key);
                if (x >= limit){
                    Tree *child = node->left;
                    node->left = aloc(x);
                    node->left->left = child;
                    break;
                }else{
                    middle = limit >> 1;
                    height = middle >> 1;
                    node = node->left;
                    continue;
                }
            }
            if (x > node->key)
                swap(x, node->key);
            node = node->left;
            middle -= height;
        }else if (updateRight(&node, x, middle, height)) break;

        height >>= 1;
    }

    return head;
}

Tree* insertionMemory(Tree* node, int x){

    if (node == NULL){
        bound = nextPow(x);
        return aloc(x);
    }

    if (bound <= x){
        Tree* head;
        do{
            head = aloc(-1);
            head->left = node;
            node = head;
            bound <<= 1;
        }while(bound <= x);

        head->key = x;
        return head;
    }

    Tree *head = node;
    int middle = bound >> 1;
    int height = middle >> 1;

    while(node->key != x){
        if (node->key == -1){
            node->key = hasKeyUnderTree(node, x, middle, height, false);
            break;
        }

        if (x == middle)
            swap(x, node->key);

        if (x < middle){
            if (x > node->key)
                swap(x, node->key);

            if (node->left == NULL){
                node->left = aloc(x);
                break;
            }
            node = node->left;
            middle -= height;
        }else{
            if (x < node->key)
                swap(x, node->key);

            if (node->right == NULL){
                node->right = aloc(x);
                break;
            }
            node = node->right;
            middle += height;
        }

        height >>= 1;
    }

    return head;
}

Tree* insertionTopDown(Tree* node, int x){

    if (node == NULL) return aloc(x);

    Tree* head = node;
    int middle = SIZE>>1;
    int height = middle>>1;

    while(node->key != x){
        if (node->key == -1){
            node->key = hasKeyUnderTree(node, x, middle, height, false);
            break;
        }

        if (x == middle) swap(x, node->key);

        if (x < middle){
            if (x > node->key) swap(x, node->key);

            if (node->left == NULL){
                node->left = aloc(x);
                break;
            }
            node = node->left;
            middle -= height;
        }else{
            if (x < node->key) swap(x, node->key);

            if (node->right == NULL){
                node->right = aloc(x);
                break;
            }
            node = node->right;
            middle += height;
        }
        height >>= 1;
    }

    return head;
}

void print(Tree *node){

    if (node == NULL) return;

    print(node->left);
    if (before >= node->key)
        printf("STOP. HAS ERR!! %d\n", before);
    //printf("%d ", node->key);
    before = node->key;
    print(node->right);
}

void printHeight(Tree *node, int alt){

    if (node == NULL) return;

    printHeight(node->left, alt+1);
    if (before >= node->key)
        printf("STOP. HAS ERR!! %d\n", before);
    //printf("%d:%d ", node->key, alt);
    before = node->key;
    printHeight(node->right, alt+1);
}

int main(){

    Tree* tree = NULL;
    int random;

    for (int i = 0; i < SIZE; i++){
        random = rand() % SIZE;
        //printf("%d\n", random);
        tree = insertionInterval(tree, random);
        //tree = insertionMemory(tree, random);
        //tree = insertionTopDown(tree, random);
        //tree = insetionDegree(tree, random);
    }

    print(tree);
}
