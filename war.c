// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO (com Missões)
// ============================================================================
//
// ALTERAÇÃO PEDIDA: Removida a verificação de cor duplicada no cadastro
// dos territórios — agora cores podem se repetir livremente.
//
// Recursos incluídos:
// - Missões estratégicas por jogador (armazenadas dinamicamente).
// - Atribuição aleatória de missão a cada jogador.
// - Verificação silenciosa de missão ao final da fase de ataques.
// - Cadastro de 2 até 5 jogadores (ajustável).
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
#define MAX_TERRITORIOS 5
#define MAX_NOME 30
#define MAX_COR  10
#define MAX_MISSAO 120
#define MAX_JOGADORES 5

// --- Estruturas de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int  tropas;
} Territorio;

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    char* missao;         // armazenada dinamicamente
    int   missaoMostrada; // 0/1: para exibir apenas uma vez
} Jogador;

// --- Protótipos das Funções ---
Territorio* alocarMapa(int n);
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int qtdJogadores);
void exibirMapa(const Territorio* mapa, int n);

void atacar(Territorio* atacante, Territorio* defensor);
void faseDeAtaque(Territorio* mapa, int qtd, Jogador* jogadores, int qtdJogadores);

// Missões
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(const char* missao, const Jogador* jogador);
int  verificarMissao(char* missao, Territorio* mapa, int tamanho);

// Utilidades
void limparBufferEntrada(void);

// --- Função Principal (main) ---
int main(void) {
    srand((unsigned)time(NULL)); // inicializa aleatoriedade

    // Alocação dinâmica para os territórios
    Territorio* territorios = alocarMapa(MAX_TERRITORIOS);

    // --- Cadastro dos Territórios ---
    printf("=== Cadastro de Territórios ===\n");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("\n--- Cadastro do território %d: ---\n", i + 1);

        // Nome
        printf("Nome do território (ex: America, Asia): ");
        scanf(" %[^\n]", territorios[i].nome);

        // Cor (AGORA PODE REPETIR)
        printf("Cor do exército (ex: Azul, Verde, Amarelo): ");
        scanf(" %[^\n]", territorios[i].cor);

        // Tropas
        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // --- Quantidade de jogadores (2 a MAX_JOGADORES) ---
    int qtdJogadores;
    do {
        printf("\nQuantidade de jogadores (2 a %d): ", MAX_JOGADORES);
        scanf("%d", &qtdJogadores);
        if (qtdJogadores < 2 || qtdJogadores > MAX_JOGADORES) {
            printf("Valor inválido. Tente novamente.\n");
        }
    } while (qtdJogadores < 2 || qtdJogadores > MAX_JOGADORES);

    // --- Cadastro de Jogadores ---
    Jogador* jogadores = (Jogador*)calloc(qtdJogadores, sizeof(Jogador));
    if (!jogadores) {
        printf("Erro ao alocar jogadores.\n");
        free(territorios);
        return 1;
    }

    printf("\n=== Cadastro de Jogadores (%d) ===\n", qtdJogadores);
    for (int i = 0; i < qtdJogadores; i++) {
        printf("\n--- Jogador %d ---\n", i + 1);
        printf("Nome do jogador: ");
        scanf(" %[^\n]", jogadores[i].nome);

        // Sugestão: escolher uma cor que também exista no mapa (não é obrigatório)
        printf("Sua cor (livre, pode repetir): ");
        scanf(" %[^\n]", jogadores[i].cor);

        jogadores[i].missao = (char*)malloc(MAX_MISSAO);
        if (!jogadores[i].missao) {
            printf("Erro ao alocar memória para missão do jogador.\n");
            liberarMemoria(territorios, jogadores, qtdJogadores);
            return 1;
        }
        jogadores[i].missaoMostrada = 0;
    }

    // --- Vetor de Missões (templates) ---
    char* missoesBase[] = {
        "Controlar pelo menos 3 territorios com a sua cor (%s).",
        "Somar pelo menos 20 tropas em territorios da sua cor (%s).",
        "Controlar 2 territorios consecutivos (indices adjacentes) com a sua cor (%s).",
        "Eliminar totalmente a cor Vermelho (nenhum territorio com 'Vermelho').",
        "Controlar todos os territorios com a sua cor (%s)."
    };
    const int TOTAL_MISSOES = (int)(sizeof(missoesBase)/sizeof(missoesBase[0]));

    // --- Atribuição aleatória de Missão por jogador ---
    for (int i = 0; i < qtdJogadores; i++) {
        // Copia um template aleatório
        atribuirMissao(jogadores[i].missao, missoesBase, TOTAL_MISSOES);
        // Formata embutindo a cor do jogador quando houver "%s"
        char buffer[MAX_MISSAO];
        snprintf(buffer, sizeof(buffer), "%s", jogadores[i].missao); // guarda o template
        snprintf(jogadores[i].missao, MAX_MISSAO, buffer, jogadores[i].cor);
    }

    // --- Exibe a missão de cada jogador apenas uma vez (no início) ---
    printf("\n=== Missoes dos Jogadores ===\n");
    for (int i = 0; i < qtdJogadores; i++) {
        exibirMissao(jogadores[i].missao, &jogadores[i]);
        jogadores[i].missaoMostrada = 1;
    }

    // --- Laço principal do jogo (turnos) ---
    int opcao;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Exibir mapa\n");
        printf("2 - Realizar ataques\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibirMapa(territorios, MAX_TERRITORIOS);
                break;
            case 2:
                // Fase de ataque com verificação silenciosa de vitória
                faseDeAtaque(territorios, MAX_TERRITORIOS, jogadores, qtdJogadores);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    // Liberação da memória
    liberarMemoria(territorios, jogadores, qtdJogadores);

    return 0;
}

// ============================================================================
// Funções de Gerenciamento de Memória e Exibição
// ============================================================================

Territorio* alocarMapa(int n) {
    Territorio* mapa = (Territorio*)calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa!\n");
        exit(1);
    }
    return mapa;
}

void liberarMemoria(Territorio* mapa, Jogador* jogadores, int qtdJogadores) {
    if (jogadores) {
        for (int i = 0; i < qtdJogadores; i++) {
            free(jogadores[i].missao);
            jogadores[i].missao = NULL;
        }
        free(jogadores);
    }
    free(mapa);
}

void exibirMapa(const Territorio* mapa, int n) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    for (int i = 0; i < n; i++) {
        printf("\nTERRITÓRIO %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do exército: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

// ============================================================================
// Funções de Lógica do Jogo (Ataque)
// ============================================================================

void atacar(Territorio* atacante, Territorio* defensor) {
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
        atacante->tropas -= defensor->tropas;    // atacante perde tropas
    } else {
        printf(">>> Defesa bem-sucedida! %s perde uma tropa.\n", atacante->nome);
        atacante->tropas -= 1;
    }
}

void faseDeAtaque(Territorio* mapa, int qtd, Jogador* jogadores, int qtdJogadores) {
    char nomeOrigem[MAX_NOME], nomeDestino[MAX_NOME];
    Territorio *origem = NULL, *destino = NULL;

    int continuarAtaques = 1;
    while (continuarAtaques) {
        origem = destino = NULL;

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
            goto verificar_e_continuar;
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
            goto verificar_e_continuar;
        }

        // Validação simples de inimigos (cores diferentes)
        if (strcmp(origem->cor, destino->cor) == 0) {
            printf("[ERRO] Ataques devem ser contra territórios inimigos (cores diferentes).\n");
            goto verificar_e_continuar;
        }

        // Executa o ataque
        atacar(origem, destino);

verificar_e_continuar:
        // --- Verificação silenciosa das missões após o turno ---
        for (int j = 0; j < qtdJogadores; j++) {
            if (verificarMissao(jogadores[j].missao, mapa, qtd)) {
                printf("\n==============================\n");
                printf(">>> VITÓRIA! %s cumpriu a missão:\n", jogadores[j].nome);
                printf("    %s\n", jogadores[j].missao);
                printf("==============================\n");
                return; // encerra a fase de ataque imediatamente
            }
        }

        // Pergunta se o jogador deseja continuar os ataques
        printf("Deseja realizar outro ataque? (1 - Sim, 0 - Não): ");
        scanf("%d", &continuarAtaques);
    }
}

// ============================================================================
// Funções de Missões
// ============================================================================

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

void exibirMissao(const char* missao, const Jogador* jogador) {
    printf("- Jogador: %s | Cor: %s\n", jogador->nome, jogador->cor);
    printf("  Sua missão: %s\n", missao);
}

// --- Auxiliares para verificar missões ---
static int contarTerritoriosPorCor(const Territorio* mapa, int n, const char* cor) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) count++;
    }
    return count;
}

static int somarTropasPorCor(const Territorio* mapa, int n, const char* cor) {
    int soma = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) soma += mapa[i].tropas;
    }
    return soma;
}

// Extrai a cor do jogador quando a descrição contiver "sua cor (XYZ)"
static int extrairCorDaMissao(const char* missao, char* outCor, size_t tamOut) {
    const char* abre = strchr(missao, '(');
    const char* fecha = strchr(missao, ')');
    if (!abre || !fecha || fecha <= abre + 1) return 0;
    size_t len = (size_t)(fecha - (abre + 1));
    if (len >= tamOut) len = tamOut - 1;
    strncpy(outCor, abre + 1, len);
    outCor[len] = '\0';
    return 1;
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    char corJogador[MAX_COR];
    int temCor = extrairCorDaMissao(missao, corJogador, sizeof(corJogador));

    if (strstr(missao, "Controlar pelo menos 3 territorios")) {
        if (!temCor) return 0;
        return contarTerritoriosPorCor(mapa, tamanho, corJogador) >= 3;
    }

    if (strstr(missao, "Somar pelo menos 20 tropas")) {
        if (!temCor) return 0;
        return somarTropasPorCor(mapa, tamanho, corJogador) >= 20;
    }

    if (strstr(missao, "Controlar 2 territorios consecutivos")) {
        if (!temCor) return 0;
        for (int i = 0; i < tamanho - 1; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 &&
                strcmp(mapa[i + 1].cor, corJogador) == 0) {
                return 1;
            }
        }
        return 0;
    }

    if (strstr(missao, "Eliminar totalmente a cor Vermelho")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) return 0;
        }
        return 1;
    }

    if (strstr(missao, "Controlar todos os territorios")) {
        if (!temCor) return 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) != 0) return 0;
        }
        return 1;
    }

    return 0; // missão desconhecida
}

// ============================================================================
// Utilidades
// ============================================================================

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descarta */ }
}
