/*
 * Trabalho Pratico - Arvore B
 * Estruturas de Dados - 2026/1
 * Prof. Dr. Anderson Bessa da Costa
 *
 * Implementacao baseada em:
 * Szwarcfiter, J. L. e Markenzon, L. (2010).
 * Estruturas de Dados e seus Algoritmos
 * Capitulo 5, Secao 5.5.
 */

#include "head.h"

/* ------------------------------------------------------------------ */
/* CRIACAO DE NO                                                        */
/* ------------------------------------------------------------------ */

/* Cria e retorna um novo no da arvore B de ordem d */
No* criaNo(int d, int eh_folha) {
    int i;
    No *novo = (No *)malloc(sizeof(No));

    novo->eh_folha  = eh_folha;
    novo->num_chaves = 0;

    /*
     * Aloca 2d+1 chaves (uma extra para suportar o overflow temporario
     * de 2d+1 chaves que ocorre logo antes da cisao de pagina).
     */
    novo->chaves = (char **)malloc((2 * d + 1) * sizeof(char *));
    for (i = 0; i < 2 * d + 1; i++) {
        novo->chaves[i] = (char *)malloc(MAX_NOME * sizeof(char));
        novo->chaves[i][0] = '\0';
    }

    /*
     * Aloca 2d+2 filhos (uma extra correspondente ao ponteiro extra
     * gerado durante o overflow temporario antes da cisao).
     */
    novo->filhos = (No **)malloc((2 * d + 2) * sizeof(No *));
    for (i = 0; i < 2 * d + 2; i++) {
        novo->filhos[i] = NULL;
    }

    return novo;
}

/* ------------------------------------------------------------------ */
/* BUSCA  (Algoritmo 5.3 do livro)                                     */
/* ------------------------------------------------------------------ */

/*
 * Busca a chave x a partir do no p.
 * Informa se encontrada e em qual posicao (nivel e indice no no).
 */
void buscaB(No *p, char *x, int nivel) {
    int i;

    if (p == NULL) {
        printf("Chave '%s' nao encontrada na arvore.\n", x);
        return;
    }

    /* percorre as chaves do no ate encontrar posicao adequada */
    i = 0;
    while (i < p->num_chaves && strcmp(x, p->chaves[i]) > 0) {
        i++;
    }

    /* chave encontrada neste no */
    if (i < p->num_chaves && strcmp(x, p->chaves[i]) == 0) {
        printf("Encontrado! Chave '%s' esta no nivel %d, posicao %d do no.\n",
               x, nivel, i + 1);
        return;
    }

    /* chegou a uma folha sem encontrar: chave inexistente */
    if (p->eh_folha) {
        printf("Chave '%s' nao encontrada na arvore.\n", x);
        return;
    }

    /* desce para o filho adequado (ponteiro p->filhos[i]) */
    buscaB(p->filhos[i], x, nivel + 1);
}

/* ------------------------------------------------------------------ */
/* INSERCAO  (Algoritmo 5.3 adaptado, cisao(que é split) descrita na Sec. 5.5.2)   */
/* ------------------------------------------------------------------ */

/*
 * Insere x na subarvore enraizada em p.
 * Caso haja split, retorna 1 e preenche chave_sobe e filho_novo
 * com a chave promovida e o novo no direito, respectivamente.
 * Retorna 0 se nao houve cisao.
 */
int insiraB(No *p, char *x, int d, char *chave_sobe, No **filho_novo) {
    int i, j;
    char prom[MAX_NOME];
    No  *filho_prom = NULL;

    /* --- inserção em folha --- */
    if (p->eh_folha) {
        /* desloca chaves maiores para abrir espaco */
        i = p->num_chaves - 1;
        while (i >= 0 && strcmp(x, p->chaves[i]) < 0) {
            strcpy(p->chaves[i + 1], p->chaves[i]);
            i--;
        }
        strcpy(p->chaves[i + 1], x);
        p->num_chaves++;

    } else {
        /* --- desce ate a folha correta --- */
        i = p->num_chaves - 1;
        while (i >= 0 && strcmp(x, p->chaves[i]) < 0) {
            i--;
        }
        i++;   /* indice do filho a descer */

        if (insiraB(p->filhos[i], x, d, prom, &filho_prom)) {
            /* houve split abaixo: incorpora chave promovida neste no */
            j = p->num_chaves - 1;
            while (j >= i) {
                strcpy(p->chaves[j + 1], p->chaves[j]);
                p->filhos[j + 2] = p->filhos[j + 1];
                j--;
            }
            strcpy(p->chaves[i], prom);
            p->filhos[i + 1] = filho_prom;
            p->num_chaves++;
        }
    }

    /* --- split se no ultrapassou 2d chaves --- */
    /*
     * Conforme Sec. 5.5.2: a pagina P fica com d entradas,
     * a chave central sd+1 sobe para o pai,
     * e um novo no Q recebe as d entradas restantes (sd+2 .. s2d).
     * Indices usados (base 0): chave do meio = d (posicao d+1 - 1).
     */
    if (p->num_chaves > 2 * d) {
        *filho_novo = criaNo(d, p->eh_folha);

        /* chave do meio sobe */
        strcpy(chave_sobe, p->chaves[d]);

        /* metade direita vai para o novo no */
        (*filho_novo)->num_chaves = d;
        for (j = 0; j < d; j++) {
            strcpy((*filho_novo)->chaves[j], p->chaves[d + 1 + j]);
            p->chaves[d + 1 + j][0] = '\0';
        }

        /* transfere ponteiros filhos para o novo no (se nao for folha) */
        if (!p->eh_folha) {
            for (j = 0; j <= d; j++) {
                (*filho_novo)->filhos[j] = p->filhos[d + 1 + j];
                p->filhos[d + 1 + j] = NULL;  /* limpa ponteiros do no original */
            }
        }

        /* no original fica com apenas d chaves */
        p->num_chaves = d;

        return 1;   /* houve split */
    }

    return 0;   /* sem cisao */
}

/*
 * Ponto de entrada da insercao.
 * Cria a raiz se a arvore estiver vazia.
 * Se a raiz for dividida, cria uma nova raiz e aumenta a altura.
 */
No* insira(No *raiz, char *x, int d) {
    char prom[MAX_NOME];
    No  *filho_prom = NULL;

    if (raiz == NULL) {
        raiz = criaNo(d, 1);
        strcpy(raiz->chaves[0], x);
        raiz->num_chaves = 1;
        return raiz;
    }

    if (insiraB(raiz, x, d, prom, &filho_prom)) {
        /* raiz foi dividida: nova raiz com uma unica chave */
        No *nova_raiz = criaNo(d, 0);
        strcpy(nova_raiz->chaves[0], prom);
        nova_raiz->filhos[0] = raiz;
        nova_raiz->filhos[1] = filho_prom;
        nova_raiz->num_chaves = 1;
        return nova_raiz;
    }

    return raiz;
}

/* ------------------------------------------------------------------ */
/* LIBERACAO DE MEMORIA                                                 */
/* ------------------------------------------------------------------ */

void liberaArvore(No *p, int d) {
    int i;
    if (p == NULL) return;

    if (!p->eh_folha) {
        for (i = 0; i <= p->num_chaves; i++) {
            liberaArvore(p->filhos[i], d);
        }
    }

    for (i = 0; i < 2 * d + 1; i++) {
        free(p->chaves[i]);
    }
    free(p->chaves);
    free(p->filhos);
    free(p);
}

/* ------------------------------------------------------------------ */
/* FUNCOES AUXILIARES                                                   */
/* ------------------------------------------------------------------ */

int ler_opcao() {
    int opcao;

    printf("Entre com a sua opcao: ");
    scanf("%d%*c", &opcao);

    return opcao;
}

void menu() {
    printf("\n// ----- // ----- // ARVORE B // ----- // ----- //\n"
           "[1] - Buscar\n"
           "[2] - Inserir\n"
           "[9] - Finalizar\n"
           "-------------------------------\n");
}

/* ------------------------------------------------------------------ */
/* MAIN                                                                 */
/* ------------------------------------------------------------------ */

int main(void) {
    int  d, opcao;
    No  *raiz = NULL;
    char buffer[MAX_NOME];
    char x[MAX_NOME];
    FILE *arq;

    /* solicita a ordem d antes de construir a arvore */
    printf("Informe a ordem 'd' da arvore B (d >= 2): ");
    scanf("%d%*c", &d);
    while (d < 2) {
        printf("Erro: d deve ser >= 2. Informe novamente: ");
        scanf("%d%*c", &d);
    }

    /* carrega pokemon_names.txt e constroi a arvore */
    arq = fopen("pokemon_names.txt", "r");
    if (!arq) {
        printf("Erro: nao foi possivel abrir 'pokemon_names.txt'.\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), arq)) {
        /* remove \r e \n do final */
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (strlen(buffer) > 0) {
            raiz = insira(raiz, buffer, d);
        }
    }
    fclose(arq);

    printf("Arvore B de ordem %d construida com sucesso!\n", d);

    /* loop principal do menu */
    while (1) {
        menu();
        opcao = ler_opcao();

        switch (opcao) {

            /* busca */
            case 1: {
                puts("Entre com a chave a ser buscada: ");
                scanf("%49[^\n]%*c", x);
                buscaB(raiz, x, 0);
            }
            break;

            /* insercao */
            case 2: {
                puts("Entre com a chave a ser inserida: ");
                scanf("%49[^\n]%*c", x);
                raiz = insira(raiz, x, d);
                printf("Chave '%s' inserida com sucesso!\n", x);
            }
            break;

            /* finalizar */
            case 9: {
                puts("Finalizando programa ..");
                liberaArvore(raiz, d);
                return 0;
            }
            break;

            default:
                puts("Opcao invalida! Tente novamente.");
            break;
        }
    }

    return 0;
}
