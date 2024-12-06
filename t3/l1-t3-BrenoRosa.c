//l1-t3-Breno Rosa
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "janela.h"

#define TAMANHO_QUADRADO 50
#define MARGEM 20




void inicializa_tabuleiro(int n, char tab[]) {
    for (int i = 0; i < n * n; i++) {
        tab[i] = '-';
    }
}

bool verifica_ameacas(int n, char tab[]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int pos1 = i * n + j;
            if (tab[pos1] == 'Q') {
                for (int k = 0; k < n; k++) {
                    for (int l = 0; l < n; l++) {
                        int pos2 = k * n + l;
                        if (pos1 != pos2 && tab[pos2] == 'Q') {
                            if (i == k || j == l || abs(i - k) == abs(j - l)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool tabuleiro_completo(int n, char tab[]) {
    int rainhas = 0;
    for (int i = 0; i < n * n; i++) {
        if (tab[i] == 'Q') {
            rainhas++;
        }
    }
    return rainhas == n;
}

void desenha_tabuleiro(int n, char tab[], int linha_destacada, int coluna_destacada, bool sem_ameacas, bool completo) {
    int largura_tabuleiro = n * TAMANHO_QUADRADO;
    int altura_tabuleiro = n * TAMANHO_QUADRADO;
    int inicio_x = MARGEM;
    int inicio_y = MARGEM;

    cor_t cor_contorno = completo ? (cor_t){0, 0, 1, 1} :
                         (sem_ameacas ? (cor_t){0, 1, 0, 1} :
                                       (cor_t){1, 0, 0, 1});
    retangulo_t borda = {
        .inicio = {inicio_x - MARGEM / 2, inicio_y - MARGEM / 2},
        .tamanho = {largura_tabuleiro + MARGEM, altura_tabuleiro + MARGEM}
    };
    j_retangulo(borda, 5, cor_contorno, (cor_t){0, 0, 0, 1});
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int pos = i * n + j;
            cor_t cor_interna = ((i + j) % 2 == 0) ?
                                (cor_t){0.86, 0.86, 0.86, 1} :
                                (cor_t){0, 0.45, 0, 1};
            if (i == linha_destacada && j == coluna_destacada) {
                cor_interna = (cor_t){1, 1, 0, 1};
            }
            retangulo_t celula = {
                .inicio = {inicio_x + j * TAMANHO_QUADRADO, inicio_y + i * TAMANHO_QUADRADO},
                .tamanho = {TAMANHO_QUADRADO, TAMANHO_QUADRADO}
            };
            j_retangulo(celula, 1, (cor_t){0, 0, 0, 1}, cor_interna);
            if (tab[pos] == 'Q') {
                ponto_t centro = {
                    .x = celula.inicio.x + TAMANHO_QUADRADO / 2,
                    .y = celula.inicio.y + TAMANHO_QUADRADO / 2
                };
                j_texto(centro, TAMANHO_QUADRADO / 2, (cor_t){0, 0, 1, 1}, "♛");
            }
        }
    }
}


void desenha_desistir(int largura_tabuleiro, int altura_tabuleiro) {
    ponto_t inicio = {
        .x = MARGEM + (largura_tabuleiro - 150) / 2,
        .y = MARGEM + altura_tabuleiro + 20
    };
    retangulo_t botao = {
        .inicio = inicio,
        .tamanho = {150, 50}
    };
    j_retangulo(botao, 2, (cor_t){1, 0, 0, 1}, (cor_t){0.8, 0.8, 0.8, 1});
    ponto_t texto_pos = {
        .x = botao.inicio.x + botao.tamanho.largura / 2,
        .y = botao.inicio.y + botao.tamanho.altura / 2
    };
    j_texto(texto_pos, 20, (cor_t){1, 0, 0, 1}, "Desistir");
}



void desenha_tempo(double tempo_inicio, int largura_tabuleiro, int altura_tabuleiro) {
    double tempo_atual = j_relogio() - tempo_inicio;
    char texto[50];
    snprintf(texto, sizeof(texto), "Tempo: %.2f s", tempo_atual);
    ponto_t posicao = {
        .x = MARGEM + (largura_tabuleiro - 150) / 2,
        .y = MARGEM + altura_tabuleiro + 90
    };
    j_texto(posicao, 20, (cor_t){1, 1, 1, 1}, texto);
}

void desenha_mouse(rato_t rato) {
    circulo_t cursor = {
        .centro = rato.posicao,
        .raio = 5
    };
    j_circulo(cursor, 1, (cor_t){1, 0, 0, 1}, (cor_t){1, 0, 0, 0.5});
}

void desenha_janela(int n, char tab[], int linha_destacada, int coluna_destacada, bool sem_ameacas, bool completo, double tempo_inicio, rato_t rato) {
    int largura_tabuleiro = n * TAMANHO_QUADRADO;
    int altura_tabuleiro = n * TAMANHO_QUADRADO;
    desenha_tabuleiro(n, tab, linha_destacada, coluna_destacada, sem_ameacas, completo);
    desenha_desistir(largura_tabuleiro, altura_tabuleiro);
    desenha_tempo(tempo_inicio, largura_tabuleiro, altura_tabuleiro);
    desenha_mouse(rato);
}


int main() {
    int n;
    printf("Digite o tamanho do tabuleiro: ");
    scanf("%d", &n);
    char tab[n * n];
    inicializa_tabuleiro(n, tab);
    tamanho_t tamanho_janela = {
        .largura = n * TAMANHO_QUADRADO + 2 * MARGEM,
        .altura = n * TAMANHO_QUADRADO + 2 * MARGEM + 100
    };
    t_inicializa(tamanho_janela, "Jogo das Rainhas");
    double tempo_inicio = j_relogio();

while (1) {
    rato_t rato = j_rato();
    int largura_botao = 150;
    int altura_botao = 50;
    int largura_tabuleiro = n * TAMANHO_QUADRADO;
    int altura_tabuleiro = n * TAMANHO_QUADRADO;
    ponto_t botao_inicio = {
        .x = MARGEM + (largura_tabuleiro - largura_botao) / 2,
        .y = MARGEM + altura_tabuleiro + 20
    };
    if (rato.clicado[0] &&
        rato.posicao.x >= botao_inicio.x &&
        rato.posicao.x <= botao_inicio.x + largura_botao &&
        rato.posicao.y >= botao_inicio.y &&
        rato.posicao.y <= botao_inicio.y + altura_botao) {
        printf("Você desistiu do jogo.\n");
        break;
    }
    int coluna_destacada = rato.posicao.x / TAMANHO_QUADRADO;
    int linha_destacada = rato.posicao.y / TAMANHO_QUADRADO;
    if (linha_destacada >= 0 && linha_destacada < n && coluna_destacada >= 0 && coluna_destacada < n) {
        if (rato.clicado[0]) {
            int pos = linha_destacada * n + coluna_destacada;
            tab[pos] = (tab[pos] == 'Q') ? '-' : 'Q';
        }
    }
    bool sem_ameacas = verifica_ameacas(n, tab);
    bool completo = sem_ameacas && tabuleiro_completo(n, tab);
    if (completo) {
        double tempo_final = j_relogio();
        printf("Parabéns! Você venceu o jogo em %.2f segundos!\n", tempo_final - tempo_inicio);
        ponto_t posicao_vitoria = {
    .x = MARGEM + (largura_tabuleiro - 300) / 2,
    .y = MARGEM + altura_tabuleiro + 100
};
j_texto(posicao_vitoria, 15, (cor_t){0, 1, 0, 1}, "Parabéns! Pressione Enter para sair...");
        j_atualiza();
        while (1) {
            if (j_tem_tecla() && j_tecla() == '\n') {
                break;
            }
        }
        break;
    }
    desenha_janela(n, tab, linha_destacada, coluna_destacada, sem_ameacas, completo, tempo_inicio, rato);
    if (j_tem_tecla() && j_tecla() == 'q') {
        break;
    }
    j_atualiza();
}
    j_finaliza();
    return 0;
}