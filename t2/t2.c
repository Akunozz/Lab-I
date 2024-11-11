#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "terminal.h"

#define MAX_RAINHAS 50

void atualiza_tela() {
    t_atualiza();
}

void posiciona_cursor(int linha, int coluna) {
    t_lincol(linha, coluna);
}

void reset_cor() {
    t_cor_normal();
}

void altera_cor_fundo(int vermelho, int verde, int azul) {
    t_cor_fundo(vermelho, verde, azul);
}

void limpa_tela() {
    t_limpa();
    posiciona_cursor(1, 1);
}

int tamanho_tabuleiro() {
    int n;
    printf("O cursor indicado estara na cor vermelha\nPara mover o cursor use as telcas 'w,s,a,d'\nPara inserir a rainha precione a telca enter ou espaço \nPara finalizar o programa precione 'x'(finaliza com erro no terminal)\n");
    printf("Digite o tamanho do tabuleiro: ");
    scanf("%d", &n);
    return n;
}

int rainhas_linha[MAX_RAINHAS];
int rainhas_coluna[MAX_RAINHAS];
int num_rainhas = 0;

bool tem_rainha(int linha, int coluna) {
    for (int i = 0; i < num_rainhas; i++) {
        if (rainhas_linha[i] == linha && rainhas_coluna[i] == coluna) {
            return true;
        }
    }
    return false;
}

bool posicao_segura(int linha, int coluna) {
    for (int i = 0; i < num_rainhas; i++) {
        int r_linha = rainhas_linha[i];
        int r_coluna = rainhas_coluna[i];
        if (r_linha == linha || r_coluna == coluna || 
            (abs(r_linha - linha) == abs(r_coluna - coluna))) {
            return false;
        }
    }
    return true;
}

void alterna_rainha(int linha, int coluna) {
    for (int i = 0; i < num_rainhas; i++) {
        if (rainhas_linha[i] == linha && rainhas_coluna[i] == coluna) {
            for (int j = i; j < num_rainhas - 1; j++) {
                rainhas_linha[j] = rainhas_linha[j + 1];
                rainhas_coluna[j] = rainhas_coluna[j + 1];
            }
            num_rainhas--;
            return;
        }
    }

    if (posicao_segura(linha, coluna) && num_rainhas < MAX_RAINHAS) {
        rainhas_linha[num_rainhas] = linha;
        rainhas_coluna[num_rainhas] = coluna;
        num_rainhas++;
    } else {
        printf("Posição (%d, %d) não é segura para colocar uma rainha.\n", linha, coluna);
    }
}

bool tabuleiro_completo(int n) {
    if (num_rainhas != n) return false;

    for (int i = 0; i < num_rainhas; i++) {
        for (int j = i + 1; j < num_rainhas; j++) {
            if (rainhas_linha[i] == rainhas_linha[j] || rainhas_coluna[i] == rainhas_coluna[j] ||
                abs(rainhas_linha[i] - rainhas_linha[j]) == abs(rainhas_coluna[i] - rainhas_coluna[j])) {
                return false;
            }
        }
    }

    return true;
}

void imprime_tabuleiro(int n, int pos_linha, int pos_coluna) {
    limpa_tela();
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == pos_linha && j == pos_coluna) {
                altera_cor_fundo(255, 0, 0);
            } else if (tem_rainha(i, j)) {
                altera_cor_fundo(0, 0, 255);
            } else {
                if ((i + j) % 2 == 0) {
                    altera_cor_fundo(220, 220, 220);
                } else {
                    altera_cor_fundo(0, 115, 0);
                }
            }

            if (tem_rainha(i, j)) {
                printf(" ♛ ");
            } else {
                printf("   ");
            }
            
            reset_cor();
        }
        printf("\n");
    }
}

void finaliza(double tempo) {
    printf("Parabéns! Você completou o tabuleiro em %.2f segundos!\n", tempo);
}

int main() {
    int n = tamanho_tabuleiro();
    int pos_linha = 0;
    int pos_coluna = 0;
    char tecla;
    double tempo_inicio = t_relogio();

    t_inicializa();

    while (1) {
        imprime_tabuleiro(n, pos_linha, pos_coluna);
        atualiza_tela();
        tecla = t_tecla();

        if (tecla == 'w' && pos_linha > 0) {
            pos_linha--;
        } else if (tecla == 's' && pos_linha < n - 1) {
            pos_linha++;
        } else if (tecla == 'a' && pos_coluna > 0) {
            pos_coluna--;
        } else if (tecla == 'd' && pos_coluna < n - 1) {
            pos_coluna++;
        } else if (tecla == '\n' || tecla == ' ') {
            alterna_rainha(pos_linha, pos_coluna);
        } else if (tecla == 'x') {
            break;
        }

        if (tabuleiro_completo(n)) {
            double tempo_final = t_relogio();
            double tempo_total = tempo_final - tempo_inicio;
            t_finaliza();
            finaliza(tempo_total);
            break;
        }    
    }

    return 0;
}
