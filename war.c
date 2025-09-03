// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS:
// - Implementar a funcionalidade de ataque entre territórios.
// - Alocar dinamicamente os territórios.
// - Criar uma simulação de batalha entre territórios utilizando dados aleatórios.
// - Atualizar os dados do território após o ataque.
// - Adicionar um laço principal para múltiplos turnos de ataque até que o jogador decida sair.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
#define MAX_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10

// --- Estrutura de Dados ---
struct Territorio {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
};

// --- Protótipos das Funções ---
struct Territorio* alocarMapa(int n);
void liberarMemoria(struct Territorio* mapa);
void exibirMapa(const struct Territorio* mapa, int n);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void faseDeAtaque(struct Territorio* mapa, int qtd);
void limparBufferEntrada();

// --- Função Principal (main) ---
int main() {
    srand(time(NULL)); // inicializa a aleatoriedade para os ataques

    // Alocação dinâmica para armazenar os territórios
    struct Territorio* territorios = alocarMapa(MAX_TERRITORIOS);

    // --- Cadastro dos Territórios ---
    printf("=== Cadastro de Territórios ===\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\n--- Cadastro do território %d: ---\n", i + 1);

        // Entrada do nome do território
        printf("Nome do território (ex: America, Asia): ");
        scanf(" %[^\n]", territorios[i].nome);

        // Entrada da cor do exército com verificação de duplicidade
        int corValida;
        do {
            corValida = 1;
            printf("Cor do exército (ex: Azul, Verde, Amarelo): ");
            scanf(" %[^\n]", territorios[i].cor);

            for (int j = 0; j < i; j++) {
                if (strcmp(territorios[i].cor, territorios[j].cor) == 0) {
                    printf("Cor já utilizada no território %s. Digite outra cor.\n", territorios[j].nome);
                    corValida = 0; // inválida, precisa digitar novamente
                    break;
                }
            }
        } while (!corValida);

        // Entrada da quantidade de tropas
        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // --- Laço principal para o jogo (turnos de ataque) ---
    int opcao;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Realizar ataques\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                exibirMapa(territorios, MAX_TERRITORIOS);
                break;
            case 2:
                faseDeAtaque(territorios, MAX_TERRITORIOS);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    // Liberação de memória
    liberarMemoria(territorios);

    return 0;
}

// Funções de Gerenciamento de Memória e Exibição

// alocarMapa()
// Aloca dinamicamente memória para o vetor de territórios
struct Territorio* alocarMapa(int n) {
    struct Territorio* mapa = (struct Territorio*)calloc(n, sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    return mapa;
}

// liberarMemoria()
// Libera a memória previamente alocada
void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}

// exibirMapa()
// Mostra o estado atual dos territórios
void exibirMapa(const struct Territorio* mapa, int n) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    for (int i = 0; i < n; i++) {
        printf("\nTERRITÓRIO %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do exército: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}


// Funções de Lógica do Jogo (Ataque)

// atacar()
// Simula uma batalha entre dois territórios
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ERRO] Um território não pode atacar outro da mesma cor!\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("\n[ERRO] O atacante precisa ter pelo menos 2 tropas para atacar!\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n%s (Atacante) rolou: %d\n", atacante->nome, dadoAtacante);
    printf("%s (Defensor) rolou: %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf(">>> Vitória do atacante! %s conquista %s!\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor); // defensor muda de cor
        defensor->tropas = atacante->tropas / 2; // metade das tropas do atacante move
        atacante->tropas -= defensor->tropas;   // atacante perde tropas
    } else {
        printf(">>> Defesa bem-sucedida! %s perde uma tropa.\n", atacante->nome);
        atacante->tropas -= 1;
    }
}

// faseDeAtaque()
// Solicita ao jogador os territórios de origem e destino e simula o ataque
void faseDeAtaque(struct Territorio* mapa, int qtd) {
    char nomeOrigem[MAX_NOME], nomeDestino[MAX_NOME];
    struct Territorio *origem = NULL, *destino = NULL;

    int continuarAtaques = 1;
    while (continuarAtaques) {
        printf("\n=== FASE DE ATAQUE ===\n");

        // Território de origem
        printf("Digite o nome do território de ORIGEM: ");
        scanf(" %[^\n]", nomeOrigem);
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].nome, nomeOrigem) == 0) {
                origem = &mapa[i];
                break;
            }
        }
        if (origem == NULL) {
            printf("Território de origem não encontrado!\n");
            continue;
        }

        // Território de destino
        printf("Digite o nome do território de DESTINO: ");
        scanf(" %[^\n]", nomeDestino);
        for (int i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].nome, nomeDestino) == 0) {
                destino = &mapa[i];
                break;
            }
        }
        if (destino == NULL) {
            printf("Território de destino não encontrado!\n");
            continue;
        }

        // Chama a simulação do ataque
        atacar(origem, destino);

        // Pergunta se o jogador deseja continuar os ataques
        printf("Deseja realizar outro ataque? (1 - Sim, 0 - Não): ");
        scanf("%d", &continuarAtaques);
    }
}
