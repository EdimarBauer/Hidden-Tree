
/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: February 22, 2018
*
* Update: March 08, 2018
*
* Consider the length of words proportional at quantity of elements that will be inserted
* Consider only the tiny alphabet for the examples
*
* The function "newString" is only to generate words of type string
*
* The variable 'last', 'INIT', and 'total' serve to test the algorithm. The variable 'lg' represents the max length of the
* strings that will be inserted. The constant 'ALFA' represents the 26 letters of alphabet plus 6 numbers that represents
* final of string, that is used in function 'newString'.
*/

#include <bits/stdc++.h>
using namespace std;
#define N (1 << 20)
#define ALFA 32

char *last, *INIT, *MIDDLE, *LOW, *HIGH;
int total, lg;

struct Tree{
    char *key;
    Tree *left, *right;
};

void newString(char t[]);
Tree* newNode(char *s);
Tree* search(Tree *tree, char s[]);

void mid(char *s, char *low, char *high);
void insert(Tree *&root, char t[]);

char* replace(Tree *&tree, bool left);
void remove(Tree *&tree, char s[]);

void print(Tree *tree, int alt);
void check_is_correct(Tree *tree);
void check(Tree *tree);



int main(){

    Tree* tree = NULL;
    INIT = (char*)malloc(sizeof(char));
    INIT[0] = '\0';
    MIDDLE = (char*)malloc(sizeof(char)*32);
    LOW = (char*)malloc(sizeof(char)*32);
    HIGH = (char*)malloc(sizeof(char)*32);
    lg = log2(N)/4; if (lg == 0)lg = 1;
    char t[16];
    int x;
    set<string> se;

    for (int j = 0; j < 2; j++){
        printf("Inserting...\n");
        for (int i = 0; i < N; i++){
            newString(t);
            //se.insert(t);
            //strcpy(lo, low); strcpy(hi, high);
            insert(tree, t);
            //print(tree, 0);
            //check_is_correct(tree);
        }
        //print(tree, 0);
        printf("Removing...\n");
        for (int i = 0; i < N; i++){
            newString(t);
            //se.erase(t);
            //printf("Removing: %s\n", t);
            remove(tree, t);
            //print(tree, 0);
            //check_is_correct(tree);
        }
    }
    //check_is_correct(tree);
    //print(tree, 0);
}



//#####################################################################################################
//#####################################################################################################


void newString(char t[]){

    int x, i = 0;
    do{
        x = rand() % ALFA;
        if (x < 26) t[i++] = (char)(x + 'a');
    }while( (x < 26 || i == 0) && i < lg);
    t[i] = '\0';
}

Tree* newNode(char *s){

    Tree *tree = (Tree*) malloc(sizeof(Tree));
    if (tree == NULL){
        puts("Err of allocation");
        exit(0);
    }
    tree->key = s;
    tree->left = tree->right = NULL;
    total++;
    return tree;
}

Tree* search(Tree* tree, char s[]){

    while(tree != NULL){
        if (strcmp(s, tree->key) == 0) return tree;

        if (strcmp(s, tree->key) < 0) tree = tree->left;
        else tree = tree->right;
    }

    return NULL;
}




void mid(char *s, char *low, char *high){

    bool change = 0;
    int i, mi, ma, sum, dif, carry = 0;
    mi = strlen(low); ma = strlen(high);
    if (mi > ma){
        swap(mi, ma);
        swap(low, high);
        change = 1;
    }
    unsigned char c;
    for (i = 0; i < mi; i++){
        sum = low[i] + high[i] + carry;
        c = (char)(sum / 2);
        if (sum%2){
            dif = 0;
            for (int j = i+1; j < mi && dif == 0; j++)
                dif = 219 - low[j] - high[j];
            if (dif < 0){
                c++;
                carry = -26;
            }else carry = 26;
        }else carry = 0;
        s[i] = c;
    }
    if (ma > mi){
        if (change){
            if (carry){
                s[i-1]++;
                carry = -carry;
            }
            low[i] = 'z';
        }else low[i] = 'a';
        low[i+1] = '\0';
        s[i] = (high[i] + low[i] + carry) / 2;
        i++;
    }else if (carry) s[i++] = 'm';

    s[i] = '\0';
    if (change) swap(low, high);
}

void insert(Tree *&root, char t[]){

    char *s = (char*)malloc(sizeof(char)*strlen(t));
    strcpy(s, t);
    if (root == NULL){
        root = newNode(s);
        return;
    }

    Tree *tree = root;
    char *middle = MIDDLE;
    middle[0] = 'm'; middle[1] = 'z'; middle[2] = '\0';
    char *low = LOW;
    low[0] = 'a'; low[1] = '\0';
    char *high = HIGH;
    high[0] = 'z'; high[1] = '\0';
    int comp;
    while(strcmp(tree->key, s) != 0){
        //printf("Low: %s  Mid: %s   High: %s\n", low, middle, high);
        comp = strcmp(s, middle);
        if (comp < 0){
            if (strcmp(s, tree->key) > 0) swap(tree->key, s);
            if (tree->left == NULL){
                tree->left = newNode(s);
                return;
            }
            tree = tree->left;
            swap(high, middle);
            mid(middle, low, high);
        }else if (comp > 0){
            if (strcmp(s, tree->key) < 0) swap(tree->key, s);
            if (tree->right == NULL){
                tree->right = newNode(s);
                return;
            }
            tree = tree->right;
            swap(low, middle);
            mid(middle, low, high);
        }else swap(tree->key, s);
    }
    free(s);
}




char* replace(Tree *&tree, bool left){

    char *s;
    if (left){
        if (tree->right == NULL){
            s = tree->key;
            if (tree->left == NULL){
                free(tree);
                tree = NULL;
                total--;
            }else tree->key = replace(tree->left, 1);
        }else s = replace(tree->right, 1);
    }else{
        if (tree->left == NULL){
            s = tree->key;
            if (tree->right == NULL){
                free(tree);
                tree = NULL;
                total--;
            }else tree->key = replace(tree->right, 0);
        }else s = replace(tree->left, 0);
    }
    return s;
}

void remove(Tree *&tree, char t[]){

    if (tree == NULL) return;

    if (strcmp(t, tree->key) == 0){
        if (tree->left == NULL && tree->right == NULL){
            free(tree->key);
            free(tree);
            tree = NULL;
            total--;
        }else{
            free(tree->key);
            if (tree->right != NULL) tree->key = replace(tree->right, 0);
            else tree->key = replace(tree->left, 1);
        }
    }else if (strcmp(t, tree->key) < 0) remove(tree->left, t);
    else remove(tree->right, t);
}




void print(Tree *tree, int alt){

    if (tree == NULL) return;

    print(tree->left, alt+1);
    printf("%s  height: %d\n", tree->key, alt);
    print(tree->right, alt+1);
}

void check_is_correct(Tree *tree){

    last = INIT;
    int to = total;
    check(tree);
    if (total != 0) printf("Diference of %d elements\n", total);
    total = to;
}

void check(Tree *tree){

    if (tree == NULL) return;

    check(tree->left);
    if (strcmp(last, tree->key) >= 0){
        printf("Err between nodes %s and %s\n", last, tree->key);
        exit(0);
    }
    last = tree->key;
    total--;
    check(tree->right);
}

