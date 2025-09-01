// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

#define MAX_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

struct Territorio {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
};


// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// --- Função Principal (main) ---
int main() {
    // Declaração do vetor de territórios que armazenará os dados.
    struct Territorio territorios[MAX_TERRITORIOS];

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
            corValida = 1; // assume que a cor é válida inicialmente
            printf("Cor do exército (ex: Azul, Verde, Amarelo): ");
            scanf(" %[^\n]", territorios[i].cor);

            // Verificação: a cor já foi cadastrada anteriormente?
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

    // --- Exibição dos Territórios ---
    printf("\n====================================================\n");
    printf("      MAPA DO MUNDO - ESTADO ATUAL   \n");
    printf("====================================================\n");

    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\nTERRITÓRIO %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}

// --- Comentários para futuras funções ---
// (Mantenha apenas os comentários, sem duplicar código ou structs)

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
