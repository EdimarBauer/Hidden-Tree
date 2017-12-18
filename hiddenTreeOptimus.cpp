/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: December 18, 2017
*
*Consider no lazy deletion and positive integer key
*/

#include <stdlib.h>
#include <iostream>

using namespace std;
#define SIZE (1 << 18)

int bound;

typedef struct node{
    unsigned int key;
    struct node* left;
    struct node* right;
}Tree;

Tree* aloc(unsigned int x){

    Tree *node = (Tree*) malloc(sizeof(Tree));
    if (node == NULL){
        puts("Err of alocation");
        exit(0);
    }

    node->key = x;
    node->left = node->right = NULL;

    return node;
}

//search the superior limit of x that is pow of 2
int nextPow(unsigned int x){

    x |= (x>>1);
    x |= (x>>2);
    x |= (x>>4);
    x |= (x>>8);
    x |= (x>>16);

    return x+1;
}

Tree* makeHead(Tree* node, unsigned int x){

    bound = nextPow(x);
    Tree* head = aloc(x);
    head->left = node;
    return head;
}

Tree* insertion(Tree* node, unsigned int x){

    if (node == NULL){
        bound = nextPow(x);
        return aloc(x);
    }

    if (bound <= x) return makeHead(node, x);

    Tree *head = node;
    unsigned int middle = bound >> 1;
    unsigned int height = middle >> 1;
    unsigned int limit;

    while(node->key != x){

        if (x < middle){
            if (x > node->key)
                swap(x, node->key);

            //base case
            if (node->left == NULL){
                node->left = aloc(x);
                break;
            }

            //if has a missing interval between the nodes
            if (node->left->key < height){
                limit = nextPow(node->left->key);
                //if x belongs the missing interval, inserts x between the nodes
                if (x >= limit){
                    Tree *child = node->left;
                    node->left = aloc(x);
                    node->left->left = child;
                    break;
                //else jump the next interval
                }else{
                    middle = limit >> 1;
                    height = middle >> 1;
                }
            //else down to the left
            }else{
                middle -= height;
                height >>= 1;
            }
            node = node->left;

        }else if (x > middle){
            if (x < node->key)
                swap(x, node->key);

            //base case
            if (node->right == NULL){
                node->right = aloc(x);
                break;
            }
            //down to the right
            node = node->right;
            middle += height;
            height >>= 1;

        //else keeps x on the node
        }else{
            swap(x, node->key);
        }
    }

    return head;
}

Tree* finding(Tree* node, unsigned int x){

    while(node != NULL){
        if (node->key == x) return node;

        if (x < node->key) node = node->left;
        else node = node->right;
    }

    return NULL;
}

int rightLess(Tree** node){

    while((*node)->left != NULL) node = &((*node)->left);

    int x = (*node)->key;
    Tree* del = *node;
    *node = (*node)->right;
    free(del);

    return x;
}

Tree* deletion(Tree* node, unsigned int x){

    if (node == NULL) return NULL;

    if (node->key == x){
        if (node->left == NULL){
            Tree* right = node->right;
            free(node);
            return right;
        }else if (node->right == NULL){
            int limit = nextPow(x);
            //if limit == bound so head will be deleted so bound should to be updated
            if (limit == bound) bound = nextPow(node->left->key);

            Tree* left = node->left;
            free(node);
            return left;
        }else node->key = rightLess(&(node->right));
    }else{
        if (x < node->key) node->left = deletion(node->left, x);
        else node->right = deletion(node->right, x);
    }

    return node;
}

void print(Tree *node){

    if (node == NULL) return;

    print(node->left);
    cout << node->key << " ";
    print(node->right);
}

int main(){

    Tree* tree = NULL;
    unsigned int random;

    for (int i = 0; i < SIZE; i++){
        random = rand() % SIZE;
        tree = insertion(tree, random);
    }

    for (int i = 0; i < SIZE; i++){
        random = rand() % SIZE;
        tree = deletion(tree, random);
    }

    print(tree);
    puts("");
}
