#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_LEN 50

// Struct do no da arvore b
typedef struct no {
    int num_keys;
    char **keys;
    struct no **children;
    int is_leaf;
} no;

// Função para criar um novo nó 
no* createNode(int d, int is_leaf) {
    no *newNode = (no *)malloc(sizeof(no));
    newNode->is_leaf = is_leaf;
    newNode->num_keys = 0;
    
    // Aloca espaço para 2d + 1 chaves 
    newNode->keys = (char **)malloc((2 * d + 1) * sizeof(char *));
    for (int i = 0; i < 2 * d + 1; i++) {
        newNode->keys[i] = (char *)malloc(MAX_NOME_LEN * sizeof(char));
    }
    
    // Aloca espaço para 2d + 2 filhos
    newNode->children = (no **)malloc((2 * d + 2) * sizeof(no *));
    for(int i = 0; i < 2 * d + 2; i++) {
        newNode->children[i] = NULL;
    }
    
    return newNode;
}

// Função auxiliar recursiva de busca 
void searchTree(no *node, char *key, int level) {
    if (node == NULL) {
        printf("Chave '%s' nao encontrada na arvore.\n", key);
        return;
    }
    
    int i = 0;
    while (i < node->num_keys && strcmp(key, node->keys[i]) > 0) {
        i++;
    }
    
    // Verifica se a chave foi encontrada no nó atual
    if (i < node->num_keys && strcmp(key, node->keys[i]) == 0) {
        printf("Encontrado! A chave '%s' esta no nivel %d, indice %d do no.\n", key, level, i);
        return;
    }
    
    // Se for folha e não encontrou, a chave não existe
    if (node->is_leaf) {
        printf("Chave '%s' nao encontrada na arvore.\n", key);
        return;
    }
    
    // Chamada recursiva para o filho apropriado
    searchTree(node->children[i], key, level + 1);
}

// Função de inserção que realiza o split bottom-up quando o nó atinge > 2d chaves 
int insertNode(no *node, char *key, int d, char *up_key, no **new_child) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // Encontra a posição e insere ordenadamente
        while (i >= 0 && strcmp(key, node->keys[i]) < 0) {
            strcpy(node->keys[i + 1], node->keys[i]);
            i--;
        }
        strcpy(node->keys[i + 1], key);
        node->num_keys++;
    } else {
        // Desce na árvore para encontrar a folha adequada
        while (i >= 0 && strcmp(key, node->keys[i]) < 0) {
            i--;
        }
        i++;
        
        char promoted_key[MAX_NOME_LEN];
        no *promoted_child = NULL;
        
        if (insertNode(node->children[i], key, d, promoted_key, &promoted_child)) {
            // Se houve promoção de um nível inferior, insere a chave no nó atual
            int j = node->num_keys - 1;
            while (j >= i) {
                strcpy(node->keys[j + 1], node->keys[j]);
                node->children[j + 2] = node->children[j + 1];
                j--;
            }
            strcpy(node->keys[i], promoted_key);
            node->children[i + 1] = promoted_child;
            node->num_keys++;
        }
    }

    // Processo de divisão (Split) se estourar a capacidade máxima de 2d chaves 
    if (node->num_keys > 2 * d) {
        *new_child = createNode(d, node->is_leaf);
        (*new_child)->num_keys = d;
        node->num_keys = d;
        
        // A chave do meio sobe
        strcpy(up_key, node->keys[d]);
        
        // Move as chaves restantes para o novo nó
        for (int j = 0; j < d; j++) {
            strcpy((*new_child)->keys[j], node->keys[j + d + 1]);
        }
        
        // Move os ponteiros filhos se o nó não for folha
        if (!node->is_leaf) {
            for (int j = 0; j <= d; j++) {
                (*new_child)->children[j] = node->children[j + d + 1];
            }
        }
        return 1; // Retorna 1 informando que houve promoção
    }
    return 0;
}

// Funcao de inserção da árvore 
no* insert(no *root, char *key, int d) {
    if (root == NULL) {
        root = createNode(d, 1);
        strcpy(root->keys[0], key);
        root->num_keys = 1;
        return root;
    }

    char up_key[MAX_NOME_LEN];
    no *new_child = NULL;

    // Se a raiz dividiu, cria uma nova raiz fazendo a árvore crescer em altura
    if (insertNode(root, key, d, up_key, &new_child)) {
        no *new_root = createNode(d, 0);
        strcpy(new_root->keys[0], up_key);
        new_root->children[0] = root;
        new_root->children[1] = new_child;
        new_root->num_keys = 1;
        return new_root;
    }
    return root;
}

int main() {
    int d;
    no *root = NULL;

    // Solicita a ordem antes da construção da árvore 
    printf("Informe a ordem 'd' da arvore B (d >= 2): ");
    scanf("%d", &d);
    while (d < 2) {
        printf("Erro: d deve ser >= 2. Informe novamente: ");
        scanf("%d", &d);
    }

    // Leitura do arquivo de pokemons
    FILE *file = fopen("pokemon_names.txt", "r");
    if (!file) {
        printf("Erro ao tentar abrir o arquivo\n");
        return 1;
    }

    char buffer[MAX_NOME_LEN];
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove a quebra de linha no final da string
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) > 0) {
            root = insert(root, buffer, d);
        }
    }
    fclose(file);

    int option = 0;
    char key[MAX_NOME_LEN];

    while (1) { // Menu igual ao pdf
        printf("\n//=============================//\n");
        printf("//          ÁRVORE B           //\n");
        printf("//=============================//\n");
        printf("[1] Buscar\n");       
        printf("[2] Inserir\n");      
        printf("[9] Finalizar\n");    
        printf("\nEntre com a sua opcao: "); 
        
        switch (option) {
            case 1: //chama a funcao de buscar
                printf("\nDigite o pokemon para buscar: ");
                scanf(" %[^\n]", key); 
                searchTree(root, key, 0); 
                break;
            case 2: //insere o novo pokemon chamando a funcao
                printf("\nDigite o pokemon para inserir: ");
                scanf(" %[^\n]", key);
                root = insert(root, key, d);
                printf("Chave inserida!\n");
                break;
            case 9:
                printf("\nFinalizando o programa...\n");
                return 0;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    }
    return 0;
}