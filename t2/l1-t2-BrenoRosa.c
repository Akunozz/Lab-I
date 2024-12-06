//l1-t2 Breno Rosa
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
    printf("O cursor indicado estara na cor vermelha\nPara mover o cursor use as telcas 'w,s,a,d'\nPara inserir a rainha precione a telca enter ou espaço \nPara finalizar o programa pressione 'x'\n");
    printf("Digite o tamanho do tabuleiro: ");
    scanf("%d", &n);
    return n;
}

int n_rainhas(int n, char tab[]) {
    int count = 0;
    for (int i = 0; i < n * n; i++) {
        if (tab[i] == 'X') {
            count++;
        }
    }
    return count;
}

void inicializa_tabuleiro(int n, char tab[]) {
    for (int i = 0; i < n * n; i++) {
        tab[i] = '.';
    }
}

bool posicao_segura(int n, char tab[], int linha, int coluna) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int pos = i * n + j;
            if (tab[pos] == 'X') {
                if (i == linha || j == coluna || abs(i - linha) == abs(j - coluna)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void alterna_rainha(int n, char tab[], int linha, int coluna) {
    int pos = linha * n + coluna;
    
    if (tab[pos] == 'X') {
        tab[pos] = '.';
    } else if (posicao_segura(n, tab, linha, coluna)) {
        tab[pos] = 'X';
    }
}

bool tabuleiro_completo(int n, char tab[]) {
    return n_rainhas(n, tab) == n;
}

void imprime_tabuleiro(int n, char tab[], int pos_linha, int pos_coluna) {
    limpa_tela();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int pos = i * n + j;

            if (i == pos_linha && j == pos_coluna) {
                altera_cor_fundo(255, 0, 0);
            } else if (tab[pos] == 'X') {
                altera_cor_fundo(0, 0, 255);
            } else {
                if ((i + j) % 2 == 0) {
                    altera_cor_fundo(220, 220, 220);
                } else {
                    altera_cor_fundo(0, 115, 0);
                }
            }
            if (tab[pos] == 'X') {
                printf(" ♛ ");
            } else {
                printf("   ");
            }
            reset_cor();
        }
        printf("\n");
    }
}

void finaliza(double tempo_inicio, double tempo_final) {
    printf("\nParabéns! Você completou o tabuleiro em %.2f segundos!\n", tempo_final - tempo_inicio);
}

int main() {
    int n = tamanho_tabuleiro();
    char tab[n * n];
    inicializa_tabuleiro(n, tab);
    int pos_linha = 0, pos_coluna = 0;
    char tecla;
    double tempo_inicio = t_relogio();
    t_inicializa();

    while (1) {
        imprime_tabuleiro(n, tab, pos_linha, pos_coluna);
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
        } 
        else if (tecla == '\n' || tecla == ' ') {
            alterna_rainha(n, tab, pos_linha, pos_coluna);
        } 
        else if (tecla == 'x') {
            break;
        }
        if (tabuleiro_completo(n, tab)) {
            double tempo_final = t_relogio();
            t_finaliza();
            finaliza(tempo_inicio, tempo_final);
            return 0;
        }
    }
    t_finaliza();
    return 0;
}
