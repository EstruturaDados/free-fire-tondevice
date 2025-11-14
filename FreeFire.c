#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_ITENS 20
#define TAM_NOME 30
#define TAM_TIPO 20

// Enum para critérios de ordenação
typedef enum {
    NOME,
    TIPO,
    PRIORIDADE
} CriterioOrdenacao;

// Struct para representar um item da mochila
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
    int prioridade; // 1 a 5 (1 = baixa, 5 = crítica)
} Item;

// Variáveis globais
Item mochila[MAX_ITENS];
int totalItens = 0;
bool ordenadoPorNome = false;
int comparacoes = 0;

// Protótipos das funções
void exibirMenu();
void adicionarItem();
void removerItem();
void listarItens();
void buscarSequencial();
void ordenarItens();
void buscarBinaria();
void insertionSort(Item vetor[], int tamanho, CriterioOrdenacao criterio);
void selectionSort(Item vetor[], int tamanho, CriterioOrdenacao criterio);
void bubbleSort(Item vetor[], int tamanho, CriterioOrdenacao criterio);
int compararItens(Item a, Item b, CriterioOrdenacao criterio);
void trocarItens(Item *a, Item *b);
void limparBuffer();

int main() {
    printf("=== 🎒 MOCHILA FREE FIRE - NIVEL MESTRE ===\n");
    printf("=== Sistema de Gerenciamento de Inventario ===\n\n");

    int opcao;
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1:
                adicionarItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarSequencial();
                break;
            case 5:
                ordenarItens();
                break;
            case 6:
                buscarBinaria();
                break;
            case 0:
                printf("Saindo do sistema... Ate a proxima missao! 🏝️\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        printf("\n");
    } while(opcao != 0);

    return 0;
}

void exibirMenu() {
    printf("=== MENU PRINCIPAL ===\n");
    printf("1. ➕ Adicionar item\n");
    printf("2. 🗑️  Remover item\n");
    printf("3. 📋 Listar todos os itens\n");
    printf("4. 🔍 Busca sequencial por nome\n");
    printf("5. 📊 Ordenar itens\n");
    printf("6. 🎯 Busca binaria por nome\n");
    printf("0. 🚪 Sair\n");
}

void adicionarItem() {
    if(totalItens >= MAX_ITENS) {
        printf("❌ Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novoItem;
    
    printf("=== ADICIONAR NOVO ITEM ===\n");
    
    printf("Nome: ");
    fgets(novoItem.nome, TAM_NOME, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0';
    
    printf("Tipo (arma, municao, cura, ferramenta): ");
    fgets(novoItem.tipo, TAM_TIPO, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';
    
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    
    printf("Prioridade (1-5, onde 5 e mais critico): ");
    scanf("%d", &novoItem.prioridade);
    limparBuffer();
    
    // Validações
    if(novoItem.quantidade <= 0) {
        printf("❌ Quantidade deve ser positiva!\n");
        return;
    }
    
    if(novoItem.prioridade < 1 || novoItem.prioridade > 5) {
        printf("❌ Prioridade deve estar entre 1 e 5!\n");
        return;
    }
    
    mochila[totalItens] = novoItem;
    totalItens++;
    ordenadoPorNome = false; // A lista pode estar desordenada após adição
    
    printf("✅ Item '%s' adicionado com sucesso!\n", novoItem.nome);
}

void removerItem() {
    if(totalItens == 0) {
        printf("❌ Mochila vazia! Nao ha itens para remover.\n");
        return;
    }
    
    char nomeBusca[TAM_NOME];
    printf("=== REMOVER ITEM ===\n");
    printf("Nome do item a ser removido: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    for(int i = 0; i < totalItens; i++) {
        if(strcmp(mochila[i].nome, nomeBusca) == 0) {
            // Move todos os itens uma posição para frente
            for(int j = i; j < totalItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            totalItens--;
            printf("✅ Item '%s' removido com sucesso!\n", nomeBusca);
            return;
        }
    }
    
    printf("❌ Item '%s' nao encontrado na mochila.\n", nomeBusca);
}

void listarItens() {
    if(totalItens == 0) {
        printf("❌ Mochila vazia!\n");
        return;
    }
    
    printf("=== ITENS NA MOCHILA (%d/%d) ===\n", totalItens, MAX_ITENS);
    printf("-------------------------------------------------------------\n");
    printf("| %-20s | %-15s | %-8s | %-4s |\n", "NOME", "TIPO", "QUANTIDADE", "PRIO");
    printf("-------------------------------------------------------------\n");
    
    for(int i = 0; i < totalItens; i++) {
        printf("| %-20s | %-15s | %-10d | %-4d |\n", 
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("-------------------------------------------------------------\n");
}

void buscarSequencial() {
    if(totalItens == 0) {
        printf("❌ Mochila vazia! Nao ha itens para buscar.\n");
        return;
    }
    
    char nomeBusca[TAM_NOME];
    printf("=== BUSCA SEQUENCIAL ===\n");
    printf("Nome do item a buscar: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    comparacoes = 0;
    bool encontrado = false;
    
    for(int i = 0; i < totalItens; i++) {
        comparacoes++;
        if(strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n✅ ITEM ENCONTRADO!\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            printf("Prioridade: %d\n", mochila[i].prioridade);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            encontrado = true;
            break;
        }
    }
    
    if(!encontrado) {
        printf("❌ Item '%s' nao encontrado na mochila.\n", nomeBusca);
        printf("Comparacoes realizadas: %d\n", comparacoes);
    }
}

void ordenarItens() {
    if(totalItens == 0) {
        printf("❌ Mochila vazia! Nao ha itens para ordenar.\n");
        return;
    }
    
    int opcao;
    CriterioOrdenacao criterio;
    clock_t inicio, fim;
    double tempo;
    
    printf("=== ORDENAR ITENS ===\n");
    printf("1. 📝 Por nome (Bubble Sort)\n");
    printf("2. 🏷️  Por tipo (Insertion Sort)\n");
    printf("3. ⚡ Por prioridade (Selection Sort)\n");
    printf("Escolha o criterio: ");
    scanf("%d", &opcao);
    limparBuffer();
    
    switch(opcao) {
        case 1:
            criterio = NOME;
            inicio = clock();
            bubbleSort(mochila, totalItens, criterio);
            fim = clock();
            ordenadoPorNome = true;
            printf("✅ Itens ordenados por NOME usando Bubble Sort\n");
            break;
        case 2:
            criterio = TIPO;
            inicio = clock();
            insertionSort(mochila, totalItens, criterio);
            fim = clock();
            printf("✅ Itens ordenados por TIPO usando Insertion Sort\n");
            break;
        case 3:
            criterio = PRIORIDADE;
            inicio = clock();
            selectionSort(mochila, totalItens, criterio);
            fim = clock();
            printf("✅ Itens ordenados por PRIORIDADE usando Selection Sort\n");
            break;
        default:
            printf("❌ Opcao invalida!\n");
            return;
    }
    
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("📊 Metricas de desempenho:\n");
    printf("   - Comparacoes: %d\n", comparacoes);
    printf("   - Tempo de execucao: %.6f segundos\n", tempo);
    
    listarItens();
}

void buscarBinaria() {
    if(totalItens == 0) {
        printf("❌ Mochila vazia! Nao ha itens para buscar.\n");
        return;
    }
    
    if(!ordenadoPorNome) {
        printf("❌ A lista precisa estar ordenada por nome para usar busca binaria!\n");
        printf("   Use a opcao 'Ordenar itens' -> 'Por nome' primeiro.\n");
        return;
    }
    
    char nomeBusca[TAM_NOME];
    printf("=== BUSCA BINARIA ===\n");
    printf("Nome do item a buscar: ");
    fgets(nomeBusca, TAM_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    int inicio = 0;
    int fim = totalItens - 1;
    int meio;
    comparacoes = 0;
    bool encontrado = false;
    
    while(inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoes++;
        
        int comparacao = strcmp(mochila[meio].nome, nomeBusca);
        
        if(comparacao == 0) {
            printf("\n✅ ITEM ENCONTRADO (Busca Binaria)!\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            printf("Comparacoes realizadas: %d\n", comparacoes);
            encontrado = true;
            break;
        } else if(comparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    
    if(!encontrado) {
        printf("❌ Item '%s' nao encontrado na mochila.\n", nomeBusca);
        printf("Comparacoes realizadas: %d\n", comparacoes);
    }
}

// Função de comparação genérica
int compararItens(Item a, Item b, CriterioOrdenacao criterio) {
    comparacoes++;
    switch(criterio) {
        case NOME:
            return strcmp(a.nome, b.nome);
        case TIPO:
            return strcmp(a.tipo, b.tipo);
        case PRIORIDADE:
            return b.prioridade - a.prioridade; // Ordem decrescente para prioridade
        default:
            return 0;
    }
}

void trocarItens(Item *a, Item *b) {
    Item temp = *a;
    *a = *b;
    *b = temp;
}

// Algoritmos de ordenação
void bubbleSort(Item vetor[], int tamanho, CriterioOrdenacao criterio) {
    comparacoes = 0;
    for(int i = 0; i < tamanho - 1; i++) {
        for(int j = 0; j < tamanho - 1 - i; j++) {
            if(compararItens(vetor[j], vetor[j + 1], criterio) > 0) {
                trocarItens(&vetor[j], &vetor[j + 1]);
            }
        }
    }
}

void insertionSort(Item vetor[], int tamanho, CriterioOrdenacao criterio) {
    comparacoes = 0;
    for(int i = 1; i < tamanho; i++) {
        Item chave = vetor[i];
        int j = i - 1;
        
        while(j >= 0 && compararItens(vetor[j], chave, criterio) > 0) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = chave;
    }
}

void selectionSort(Item vetor[], int tamanho, CriterioOrdenacao criterio) {
    comparacoes = 0;
    for(int i = 0; i < tamanho - 1; i++) {
        int indiceMenor = i;
        for(int j = i + 1; j < tamanho; j++) {
            if(compararItens(vetor[j], vetor[indiceMenor], criterio) < 0) {
                indiceMenor = j;
            }
        }
        if(indiceMenor != i) {
            trocarItens(&vetor[i], &vetor[indiceMenor]);
        }
    }
}

void limparBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}