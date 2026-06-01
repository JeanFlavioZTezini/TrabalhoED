#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50

/* definicao de tipos */

typedef struct no {
    int num_chaves;
    char **chaves;
    struct no **filhos;
    int eh_folha;
} No;

/* funcoes principais */
No*  criaNo(int d, int eh_folha);
void buscaB(No *p, char *x, int nivel);
int  insiraB(No *p, char *x, int d, char *chave_sobe, No **filho_novo);
No*  insira(No *raiz, char *x, int d);
void liberaArvore(No *p, int d);

/* funcoes auxiliares */
int  ler_opcao();
void menu();
