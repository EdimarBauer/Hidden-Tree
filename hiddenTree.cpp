/*
* Título: Algoritmo Oculto, testes de desempenho
* Autor: Edimar Jacob Bauer
* Desenvolvido em: 20/11/2017
*
*   Este código tem por finalidade garantir a autoria da ideia a respeito da implementação da árvore Oculta, assim temporariamente chamada,
* antes mesmo da publicação do artigo a respeito das novas funcionalidades da Árvore Oculta como a propagação do valor na inserção. Este
* algoritmo foi ideado inicialmente pelo professor Doutor Saulo Jorge Beltrão de Queiroz que compartilhou sua ideia comigo e eu o ajudei a
* desenvolver o algoritmo, suas funções, otimizações, variações e desempenho. Mais detalhes poderão ser encontrados no artigo citado abaixo.
*
* Artigo referência: "The Hidden Binary Search Tree: A Balanced Rotation-Free Search Tree in the AVL RAM Model"
* Autor do Artigo: Saulo Jorge Beltrão de Queiroz
* Data da publicação: 19 de novembro de 2017
*
*
* Neste executável foram codificados e estudados os algoritmos de Árvore AVL, Árvore Oculta e Árvore Oculta com Propagação. Para cada um deles,
* foram retiradas amostras de desmpenho nas principais funções das árvores binárias: inserção, busca, travessia e exclusão. Os resultados
* iniciais apontaram a Árvore Oculta com Propagação tendo o melhor desempenho geral.
*
* A grosso modo, a árvore Oculta e a Árvore Oculta com Propagação trabalham com um número pré determinado de valores a serem inseridos,
* portanto sabe-se de ante-mão onde cada nó deverá ficar evitando a rotação e mantendo a árvore balanceada. Seu desempenho é melhor que a
* Árvore Rubro-Negra pois dispensa rotação e ainda dá possibilidades a novas funcionalidades como a exclusão preguiçosa aqui implementada.
*
* Alguns detalhes e códigos futuros ainda serão estudados posteriormente, como a distribuição de valor da variável "meio" em cada iteração,
* a possibilidade de se usar este algoritmo da Árvore Oculta com Propagação para substituir a Heap dinâmica e a implementação do código para
* que o tamanho da Árvore Oculta seja dinâmico com relação a chave, e podendo talvez otimizar espaço de armazenamento.
*/
#include <bits/stdc++.h>

#define INICIO 0
#define FIM (1 << 13)

using namespace std;

typedef struct no{
    int chave;
    struct no *esquerda;
    struct no *direita;
}ArvoreOculta;

typedef struct arvore{
    int chave;
    int alt_d;
    int alt_e;
    struct arvore *esq, *dir;
}Arvore;



ArvoreOculta* alocar(int x){

    ArvoreOculta *no = (ArvoreOculta*)malloc(sizeof(ArvoreOculta));
    if (no == NULL){
        puts("Falha na alocacao do no");
        exit(0);
    }
    no->chave = x;
    no->esquerda = NULL;
    no->direita = NULL;

    return no;
}

ArvoreOculta* buscaOculta(ArvoreOculta* no, int x){

    int a = INICIO;
    int z = FIM;
    int meio;
    while(1){
        if (no == NULL) return NULL;
        if (no->chave == x) return no;

        meio = (a+z)/2;
        if (x <= meio){
            no = no->esquerda;
            z = meio - 1;
        }else{
            no = no->direita;
            a = meio + 1;
        }
    }
}

int existeChaveAbaixo(ArvoreOculta* no, int x, int a, int z){

    int meio;

    while(no != NULL){
        meio = (a+z)/2;

        if (no->chave == x)
            return x;

        if (x <= meio){
            no = no->esquerda;
            z = meio-1;
        }else{
            no = no->direita;
            a = meio+1;
        }
    }

    return -1;
}

ArvoreOculta* inserir(ArvoreOculta *no, int x){

    ArvoreOculta *aux = no;

    if (no == NULL){
        no = alocar(x);
        aux = no;
    }else{
        int a = INICIO;
        int z = FIM;
        int meio;

        while (1){
            if (x == no->chave) break;
            if (no->chave == -1) {
                //no->chave = x;
                no->chave = existeChaveAbaixo(no, x, a, z);
                break;
            }

            meio = (a + z) / 2;
            if (x <= meio){
                if (no->esquerda == NULL){
                    no->esquerda = alocar(x);
                    break;
                }

                no = no->esquerda;
                z = meio - 1;
            }else{
                if (no->direita == NULL){
                    no->direita = alocar(x);
                    break;
                }

                no = no->direita;
                a = meio + 1;
            }
        }
    }

    return aux;
}

priority_queue<int, vector<int>, greater<int> > fila;

void imprimirOculta(ArvoreOculta *no){

    if (no == NULL) return;

    fila.push(no->chave);
    imprimirOculta(no->esquerda);
    imprimirOculta(no->direita);
    //printf("%d ", fila.top());
    fila.pop();
}

void zerar(ArvoreOculta *no){

    if (no == NULL) return;

    no->chave = -1;
    zerar(no->esquerda);
    zerar(no->direita);
}

int maiorEsquerda(ArvoreOculta** no){
    /*
    *  Usa-se ponteiro de ponteiro para os casos em que o maior da direita possui filho na esquerda; nesse caso, o filho da esquerda desse nó
    *  passa a ser o filho da direita do pai dele.
    */
    while((*no)->direita != NULL)
        no = &((*no)->direita);

    int x = (*no)->chave;

    ArvoreOculta* aux = *no;
    (*no) = (*no)->esquerda;
    free(aux);
    return x;
}

ArvoreOculta* desaloca(ArvoreOculta *no){

    ArvoreOculta *aux;
    if (no->esquerda == NULL){
        aux = no->direita;
        free(no);
    }else if (no->direita == NULL){
        aux = no->esquerda;
        free(no);
    }else{
        aux = no;
        no->chave = maiorEsquerda(&(no->esquerda) );
    }

    return aux;
}

ArvoreOculta* excluirOculta(ArvoreOculta* no, int x){

    //caso base, se não tiver nenhum elemento
    if (no == NULL) return NULL;
    //caso base, se a chave estiver na cabeca
    if (no->chave == x){
        return desaloca(no);
    }

    ArvoreOculta *aux = no;
    int a = INICIO;
    int z = FIM;
    int meio;
    while(1){
        meio = (a+z)/2;
        if (x <= meio){
            if (no->esquerda != NULL){
                //achou o elemento
                if (no->esquerda->chave == x){
                    no->esquerda = desaloca(no->esquerda);
                    break;
                }else{
                    no = no->esquerda;
                    z = meio-1;
                }
            }else break;
        }else{
            if (no->direita != NULL){
                //achou o elemento
                if (no->direita->chave == x){
                    no->direita = desaloca(no->direita);
                    break;
                }else{
                    no = no->direita;
                    a = meio+1;
                }
            }else break;
        }
    }

    return aux;
}

void exclusaoPreguicosa(ArvoreOculta* no, int x){

    int a = INICIO;
    int z = FIM;
    int meio;

    while(no != NULL){
        meio = (a+z)/2;

        if (no->chave == x){
            no->chave = -1;
            break;
        }
        if (x <= meio){
            no = no->esquerda;
            z = meio-1;
        }else{
            no = no->direita;
            a = meio+1;
        }
    }
}

ArvoreOculta* exclusaoParcialmentePreguicosa(ArvoreOculta* no, int x, int a, int z){

    if (no == NULL) return NULL;

    if (no->chave == x){
        if (no->esquerda == NULL and no->direita == NULL){
            free(no);
            return NULL;
        }

        no->chave = -1;
        return no;
    }

    int meio = (a+z)/2;
    if (x <= meio){
        no->esquerda = exclusaoParcialmentePreguicosa(no->esquerda, x, a, meio-1);
    }else{
        no->direita = exclusaoParcialmentePreguicosa(no->direita, x, meio+1, z);
    }

    if (no->esquerda == NULL and no->direita == NULL){
        free(no);
        return NULL;
    }

    return no;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ArvoreOculta* inserirPropagando(ArvoreOculta *no, int x){

    ArvoreOculta *aux = no;

    if (no == NULL){
        no = alocar(x);
        aux = no;
    }else{
        int a = INICIO;
        int z = FIM;
        int meio, y;

        while (1){
            if (x == no->chave) break;
            if (no->chave == -1){
                no->chave = x;
                break;
            }

            meio = (a + z) / 2;
            if (x <= meio){
                if (no->chave < x){
                    swap(x, no->chave);
                }
                if (no->esquerda == NULL){
                    no->esquerda = alocar(x);
                    break;
                }

                no = no->esquerda;
                z = meio - 1;
            }else{
                if (no->chave > x){
                    swap(x, no->chave);
                }
                if (no->direita == NULL){
                    no->direita = alocar(x);
                    break;
                }

                no = no->direita;
                a = meio + 1;
            }
        }
    }

    return aux;
}

ArvoreOculta* buscaProp(ArvoreOculta *no, int x){

    //como a propagação deixa a árvore em ordem, podemos buscar o elemento verificando apenas a chave
    while(1){
        if (no == NULL) return NULL;
        if (no->chave == x) return no;

        if (no->chave < x)
            no = no->direita;
        else
            no = no->esquerda;
    }
}

void imprimirProp(ArvoreOculta *no){

    if (no == NULL) return;

    imprimirProp(no->esquerda);
    //printf("%d ", no->chave);
    imprimirProp(no->direita);

}

ArvoreOculta* excluirProp(ArvoreOculta* no, int x){

    //caso base, se não tiver nenhum elemento
    if (no == NULL) return NULL;
    //caso base, se a chave estiver na cabeca
    if (no->chave == x){
        return desaloca(no);
    }

    ArvoreOculta *aux = no;
    while(1){
        if (x < no->chave){
            if (no->esquerda != NULL){
                //achou o elemento
                if (no->esquerda->chave == x){
                    no->esquerda = desaloca(no->esquerda);
                    break;
                }else{
                    no = no->esquerda;
                }
            }else break;
        }else{
            if (no->direita != NULL){
                //achou o elemento
                if (no->direita->chave == x){
                    no->direita = desaloca(no->direita);
                    break;
                }else{
                    no = no->direita;
                }
            }else break;
        }
    }

    return aux;
}




void imprimir(Arvore *arv);
void rotacao_esquerda(Arvore **arv);
void rotacao_direita(Arvore **arv);
void balancear(Arvore **arv);
int insereAVL(Arvore **arv, int k);
int maiorEsquerda(Arvore **arv, Arvore *pivo);
int remover(int k, Arvore **arv);
void zerarAVL(Arvore *no);
Arvore* buscaAVL(Arvore *arv, int x);

int main(){

    ArvoreOculta *arvoreOculta = NULL;
    Arvore *arvoreAVL = NULL;
    set<int> no;
    int random;

    //Inserção inicial
    for (int i = 0; i < FIM; i++){
        random = rand() % FIM;
        arvoreOculta = inserir(arvoreOculta, random);
        //arvoreOculta = inserirPropagando(arvoreOculta, random);
        //insereAVL(&arvoreAVL, random);
        //no.insert(random);
    }

    /*
    //Funções de inserção
    for (int i = 0; i < FIM; i++){
        zerar(arvoreOculta);
        //zerarAVL(arvoreAVL);
        for (int j = 0; j < FIM; j++){
            random = rand() % FIM;
            arvoreOculta = inserir(arvoreOculta, random);
            //arvoreOculta = inserirPropagando(arvoreOculta, random);
            //insereAVL(&arvoreAVL, random);
        }
    }
    */

    /*
    //Funções para Travessia em ordem
    for (int i = 0; i < FIM; i++){
        for (int j = 0; j < FIM; j++){
            random = rand() % FIM;
            imprimirProp(arvoreOculta);
            //imprimirOculta(arvoreOculta);
            //imprimir(arvoreAVL);
        }
    }
    */

    /*
    //Funções para busca
    for (int i = 0; i < FIM; i++){
        for (int j = 0; j < FIM; j++){
            random = rand() % FIM;
            buscaOculta(arvoreOculta, random);
            //buscaProp(arvoreOculta, random);
            //buscaAVL(arvoreAVL, random);
        }
    }
    */

    //Funções de excluir
    for (int i = 0; i < FIM; i++){
        for (int j = 0; j < FIM; j++){
            random = rand() % FIM;
            //arvoreOculta = excluirOculta(arvoreOculta, random);
            //exclusaoPreguicosa(arvoreOculta, random);
            arvoreOculta = exclusaoParcialmentePreguicosa(arvoreOculta, random, INICIO, FIM);
            //arvoreOculta = excluirProp(arvoreOculta, random);
            //remover(random, &arvoreAVL);
        }
        for (int j = 0; j < FIM; j++){
            random = rand() % FIM;
            arvoreOculta = inserir(arvoreOculta, random);
            //arvoreOculta = inserirPropagando(arvoreOculta, random);
            //insereAVL(&arvoreAVL, random);
        }
    }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Funções da AVL

void zerarAVL(Arvore *arv){

    if (arv == NULL) return;

    arv->chave = -1;
    arv->alt_d = 0;
    arv->alt_e = 0;
    zerarAVL(arv->esq);
    zerarAVL(arv->dir);

}

void imprimir(Arvore *arv){

    if (arv == NULL) return;

    imprimir(arv->esq);
    printf("Chave %d  alt_e: %d  alt_d: %d\n", arv->chave, arv->alt_e, arv->alt_d);
    imprimir(arv->dir);
}

Arvore* buscaAVL(Arvore *arv, int x){

    while(1){
        if (arv == NULL) return NULL;
        if (arv->chave == x) return arv;

        if (arv->chave > x)
            arv = arv->esq;
        else
            arv = arv->dir;
    }
}

void rotacao_esquerda(Arvore **arv){

    Arvore *aux = (*arv);

    (*arv) = aux->dir;
    aux->dir = (*arv)->esq;
    aux->alt_d = (*arv)->alt_e;

    (*arv)->esq = aux;
    (*arv)->alt_e = aux->alt_d > aux->alt_e ? aux->alt_d + 1 : aux->alt_e + 1;
}

void rotacao_direita(Arvore **arv){

    Arvore *aux = (*arv);

    (*arv) = aux->esq;
    aux->esq = (*arv)->dir;
    aux->alt_e = (*arv)->alt_d;

    (*arv)->dir = aux;
    (*arv)->alt_d = aux->alt_d > aux->alt_e ? aux->alt_d + 1 : aux->alt_e + 1;
}

void balancear(Arvore **arv){

    if ( ((*arv)->alt_d - (*arv)->alt_e) > 1){
        if ( (*arv)->dir->alt_e > (*arv)->dir->alt_d )
            rotacao_direita(&((*arv)->dir));

        rotacao_esquerda(arv);

    }else if ( ((*arv)->alt_d - (*arv)->alt_e) < -1){
        if ( (*arv)->esq->alt_d > (*arv)->esq->alt_e )
            rotacao_esquerda(&((*arv)->esq));

        rotacao_direita(arv);
    }
}

int insereAVL(Arvore **arv, int k){

    if ((*arv) == NULL){
        (*arv) = (Arvore *)malloc(sizeof(Arvore));
        (*arv)->chave = k;
        (*arv)->esq = NULL;
        (*arv)->dir = NULL;
        (*arv)->alt_d = 0;
        (*arv)->alt_e = 0;
        return 1;
    }else
        if ((*arv)->chave == -1)
            (*arv)->chave = k;
        else if (k > (*arv)->chave)
            (*arv)->alt_d = insereAVL(&((*arv)->dir), k);
        else if (k < (*arv)->chave)
            (*arv)->alt_e = insereAVL(&((*arv)->esq), k);
        else
            return ((*arv)->alt_e > (*arv)->alt_d) ? (*arv)->alt_e + 1 : (*arv)->alt_d + 1;

    balancear(arv);
    return ((*arv)->alt_e > (*arv)->alt_d) ? (*arv)->alt_e + 1 : (*arv)->alt_d + 1;
}

int maiorEsquerda(Arvore **arv, Arvore *pivo){

    if ((*arv)->dir == NULL){
        pivo->chave = (*arv)->chave;
        Arvore *aux = (*arv)->esq;
        free(*arv);
        *arv = aux;
        if (aux == NULL)
            return 0;
    }else{
        (*arv)->alt_d = maiorEsquerda(&((*arv)->dir), pivo);
    }

    balancear(arv);
    return ((*arv)->alt_e > (*arv)->alt_d) ? (*arv)->alt_e + 1 : (*arv)->alt_d + 1;
}

int remover(int k, Arvore **arv){

    if ((*arv) == NULL) return 0;

    if ((*arv)->chave == k){
        Arvore *aux;
        if ((*arv)->esq == NULL){
            aux = *arv;
            *arv = (*arv)->dir;
            free(aux);
            if (*arv == NULL)
                return 0;
        }else if ((*arv)->dir == NULL){
            aux = *arv;
            *arv = (*arv)->esq;
            free(aux);
            if (*arv == NULL)
                return 0;
        }else
            (*arv)->alt_e = maiorEsquerda(&((*arv)->esq), *arv);

    }else if ((*arv)->chave > k){
        (*arv)->alt_e = remover(k, &((*arv)->esq));
    }else{
        (*arv)->alt_d = remover(k, &((*arv)->dir));
    }

    balancear(arv);
    return ((*arv)->alt_e > (*arv)->alt_d) ? (*arv)->alt_e + 1 : (*arv)->alt_d + 1;
}
