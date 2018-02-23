
/*
* Author: Edimar Jacob Bauer
* Email: edimarjb@gmail.com
* Date: February 22, 2018
*
*
* Consider the length of words proportional at quantity of elements that will be inserted
* Consider only the tiny alphabet for the examples
*
* The function "novaString" is only to generate words of type string, to test the structure set<string> and compare the time
* with this code
*
* The variable 'last' and 'total' serve to test the algorithm
*/

#include <bits/stdc++.h>
using namespace std;
#define N (1 << 20)
#define ALFA 32

char last[32];
int total;

struct Tree{
    struct Tree *left, *right;
    char *key;
};

string novaString();
char* newString(char t[], bool flag);
Tree* newNode(char s[]);
void mid(char s[], char low[], char high[]);
Tree* search(Tree* tree, char s[]);

void insert(Tree *&root, char s[], char low[], char high[]);

char* replace(Tree *&tree, bool left);
void remove(Tree *&tree, char s[]);

void print(Tree *tree, int height);
void check_is_correct(Tree *tree);
void check(Tree *tree);



int main(){

    Tree* tree = NULL;
    char low[16] = {"aaaaaaaaaaaaaa"};
    char high[16] = {"zzzzzzzzzzzzzz"};
    char lo[16], hi[16], t[16];
    char *s;
    int x;
    set<string> se;

    for (int j = 0; j < 2; j++){
        printf("Inserting...\n");
        for (int i = 0; i < N; i++){
            s = newString(t, 1);
            //se.insert(novaString());
            //cout << "Inserting: " << s << endl;
            strcpy(lo, low); strcpy(hi, high);
            insert(tree, s, lo, hi);
            //check_is_correct(tree);
        }

        printf("Removing...\n");
        for (int i = 0; i < N; i++){
            newString(t, 0);
            //se.erase(novaString());
            //cout << "Removing: " << t << endl;
            remove(tree, t);
            //check_is_correct(tree);
        }
    }
    //check_is_correct(tree);
    //print(tree, 0);
}



//#####################################################################################################
//#####################################################################################################


string novaString(){

    string s = "";
    int x;
    int lg = log2(N)/4;
    if (lg == 0)lg = 1;
    int i = 0;
    do{
        x = rand() % ALFA;
        if (x < 26){
            s += (char)(x + 'a');
            i++;
        }
    }while( (x < 26 || i == 0) && i < lg);

    return s;
}

char* newString(char t[], bool flag){

    char s[64], *r;
    int x, i = 0;
    int lg = log2(N)/4;
    if (lg == 0)lg = 1;
    do{
        x = rand() % ALFA;
        if (x < 26) s[i++] = (char)(x + 'a');
    }while( (x < 26 || i == 0) && i < lg);
    s[i] = '\0';
    if (flag){
        r = (char*)malloc(sizeof(char)*i);
        if (r == NULL){
            puts("Err of allocation in r");
            exit(0);
        }
        strcpy(r, s);
    }else strcpy(t, s);

    return r;
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

void mid(char s[], char low[], char high[]){

    int i, t = strlen(low), sum, carry = 0, dif;
    unsigned char c;
    for (i = 0; i < t; i++){
        sum = low[i] + high[i] + carry;
        c = (char)(sum / 2);
        if (sum%2){
            dif = 0;
            for (int j = i+1; j < t && dif == 0; j++)
                dif = ('z' - low[j]) - (high[j] - 'a');
            if (dif < 0){
                c++;
                carry = -26;
            }else carry = 26;
        }else carry = 0;
        s[i] = c;
    }
    s[i] = '\0';
}

Tree* search(Tree* tree, char s[]){

    while(tree != NULL){
        if (strcmp(s, tree->key) == 0) return tree;

        if (strcmp(s, tree->key) < 0) tree = tree->left;
        else tree = tree->right;
    }

    return NULL;
}




void insert(Tree *&root, char *s, char low[], char high[]){

    if (root == NULL){
        root = newNode(s);
        return;
    }

    Tree *tree = root;
    char middle[16];
    int comp;
    while(strcmp(tree->key, s) != 0){
        mid(middle, low, high);
        //printf("Low: %s  Mid: %s   High: %s\n", low, middle, high);
        comp = strcmp(s, middle);
        if (comp < 0){
            if (strcmp(s, tree->key) > 0) swap(tree->key, s);
            if (tree->left == NULL){
                tree->left = newNode(s);
                break;
            }
            tree = tree->left;
            strcpy(high, middle);
        }else if (comp > 0){
            if (strcmp(s, tree->key) < 0) swap(tree->key, s);
            if (tree->right == NULL){
                tree->right = newNode(s);
                break;
            }
            tree = tree->right;
            strcpy(low, middle);
        }else swap(tree->key, s);
    }
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




void print(Tree *tree, int height){

    if (tree == NULL) return;

    print(tree->left, height+1);
    printf("%s  height: %d\n", tree->key, height);
    print(tree->right, height+1);
}

void check_is_correct(Tree *tree){

    strcpy(last, "");
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
    strcpy(last, tree->key);
    total--;
    check(tree->right);
}

