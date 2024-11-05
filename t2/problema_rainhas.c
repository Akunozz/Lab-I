#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

void imprime_tabuleiro(int n, int pos_linha, int pos_coluna, int rainha_linha, int rainha_coluna) {
    limpa_tela(); // Limpa a tela antes de imprimir o tabuleiro
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == pos_linha && j == pos_coluna) {
                altera_cor_fundo(255, 0, 0); // Vermelho para indicar a posição do cursor
            } else if (i == rainha_linha && j == rainha_coluna) {
                altera_cor_fundo(0, 0, 255); // Azul para indicar a posição da rainha
            } else {
                // Alterna entre cores para o padrão xadrez
                if ((i + j) % 2 == 0) {
                    altera_cor_fundo(220, 220, 220); // Branco
                } else {
                    altera_cor_fundo(0, 115, 0); // Verde
                }
            }

            // Verifica se a posição é a da rainha e imprime o símbolo correspondente
            if (i == rainha_linha && j == rainha_coluna) {
                printf(" ♛ ");
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
    int pos_linha = 0; // Posição inicial do cursor
    int pos_coluna = 0;
    int rainha_linha = -1; // Inicializa fora do tabuleiro
    int rainha_coluna = -1;
    char tecla;

    while (1) {
        // Chamada para imprime_tabuleiro com todos os argumentos necessários
        imprime_tabuleiro(n, pos_linha, pos_coluna, rainha_linha, rainha_coluna);
        atualiza_tela();

        // Captura a tecla e move o cursor após pressionar Enter
        printf("Use 'w', 'a', 's', 'd' para mover, 'r' para colocar a rainha, 'q' para sair: ");
        scanf(" %c", &tecla);

        if (tecla == 'w' && pos_linha > 0) {
            pos_linha--; // Move para cima
        } else if (tecla == 's' && pos_linha < n - 1) {
            pos_linha++; // Move para baixo
        } else if (tecla == 'a' && pos_coluna > 0) {
            pos_coluna--; // Move para a esquerda
        } else if (tecla == 'd' && pos_coluna < n - 1) {
            pos_coluna++; // Move para a direita
        } else if (tecla == 'r') {
            // Coloca a rainha na posição atual do cursor
            rainha_linha = pos_linha;
            rainha_coluna = pos_coluna;
        } else if (tecla == 'q') {
            break; // Sai do loop se 'q' for pressionado
        }
    }

    return 0;
}