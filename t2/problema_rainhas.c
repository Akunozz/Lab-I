#include <stdio.h>
#include <stdbool.h>

#define MAX_RAINHAS 50  // Limite fixo para a quantidade de rainhas

void atualiza_tela() {
    fflush(stdout);
}

void posiciona_cursor(int linha, int coluna) {
    printf("%c[%d;%dH", 27, linha, coluna);
}

void reset_cor() {
    printf("%c[0m", 27);
}

void altera_cor_fundo(int vermelho, int verde, int azul) {
    printf("%c[48;2;%d;%d;%dm", 27, vermelho, verde, azul);
}

void limpa_tela() {
    printf("%c[2J", 27);
    posiciona_cursor(1, 1);
}

int tamanho_tabuleiro() {
    int n;
    printf("Digite o tamanho do tabuleiro: ");
    scanf("%d", &n);
    return n;
}

// Armazena as posições das rainhas individualmente, até o limite de MAX_RAINHAS
int rainhas_linha[MAX_RAINHAS];
int rainhas_coluna[MAX_RAINHAS];
int num_rainhas = 0;  // Número atual de rainhas no tabuleiro

// Função para verificar se uma posição contém uma rainha
bool tem_rainha(int linha, int coluna) {
    for (int i = 0; i < num_rainhas; i++) {
        if (rainhas_linha[i] == linha && rainhas_coluna[i] == coluna) {
            return true;
        }
    }
    return false;
}

// Função para verificar se a posição é segura para uma nova rainha
bool posicao_segura(int linha, int coluna) {
    for (int i = 0; i < num_rainhas; i++) {
        int r_linha = rainhas_linha[i];
        int r_coluna = rainhas_coluna[i];
        
        // Verifica se está na mesma linha, coluna ou nas diagonais
        if (r_linha == linha || r_coluna == coluna || 
            (abs(r_linha - linha) == abs(r_coluna - coluna))) {
            return false;  // Posição não segura
        }
    }
    return true;  // Posição segura
}

// Função para adicionar ou remover uma rainha se a posição for segura
void alterna_rainha(int linha, int coluna) {
    for (int i = 0; i < num_rainhas; i++) {
        if (rainhas_linha[i] == linha && rainhas_coluna[i] == coluna) {
            // Remove a rainha se já estiver presente na posição
            for (int j = i; j < num_rainhas - 1; j++) {
                rainhas_linha[j] = rainhas_linha[j + 1];
                rainhas_coluna[j] = rainhas_coluna[j + 1];
            }
            num_rainhas--;
            return;
        }
    }

    // Adiciona uma nova rainha se a posição for segura
    if (posicao_segura(linha, coluna) && num_rainhas < MAX_RAINHAS) {
        rainhas_linha[num_rainhas] = linha;
        rainhas_coluna[num_rainhas] = coluna;
        num_rainhas++;
    } else {
        printf("Posição (%d, %d) não é segura para colocar uma rainha.\n", linha, coluna);
    }
}

void imprime_tabuleiro(int n, int pos_linha, int pos_coluna) {
    limpa_tela();  // Limpa a tela antes de imprimir o tabuleiro
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == pos_linha && j == pos_coluna) {
                altera_cor_fundo(255, 0, 0);  // Vermelho para indicar a posição do cursor
            } else if (tem_rainha(i, j)) {
                altera_cor_fundo(0, 0, 255);  // Azul para indicar a posição de uma rainha
            } else {
                if ((i + j) % 2 == 0) {
                    altera_cor_fundo(220, 220, 220);  // Branco
                } else {
                    altera_cor_fundo(0, 115, 0);  // Verde
                }
            }

            if (tem_rainha(i, j)) {
                printf(" ♛ ");  // Imprime a rainha
            } else {
                printf(" - ");
            }
            
            reset_cor();
        }
        printf("\n");
    }
}

int main() {
    int n = tamanho_tabuleiro();
    int pos_linha = 0;  // Posição inicial do cursor
    int pos_coluna = 0;
    char tecla;

    while (1) {
        imprime_tabuleiro(n, pos_linha, pos_coluna);
        atualiza_tela();

        printf("Use 'w', 'a', 's', 'd' para mover, 'r' para colocar/remover uma rainha, 'q' para sair: ");
        scanf(" %c", &tecla);

        if (tecla == 'w' && pos_linha > 0) {
            pos_linha--;  // Move para cima
        } else if (tecla == 's' && pos_linha < n - 1) {
            pos_linha++;  // Move para baixo
        } else if (tecla == 'a' && pos_coluna > 0) {
            pos_coluna--;  // Move para a esquerda
        } else if (tecla == 'd' && pos_coluna < n - 1) {
            pos_coluna++;  // Move para a direita
        } else if (tecla == 'r') {
            alterna_rainha(pos_linha, pos_coluna);  // Tenta adicionar ou remover uma rainha
        } else if (tecla == 'q') {
            break;  // Sai do loop se 'q' for pressionado
        }
    }

    return 0;
}
