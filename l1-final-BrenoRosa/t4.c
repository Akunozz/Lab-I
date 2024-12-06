#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "janela.h"

#define NLIN 12      // Número de linhas do tabuleiro
#define NCOL 9       // Número de colunas do tabuleiro
#define TAM_CASA 50  // Tamanho de cada célula em pixels
#define TAM_CURSOR 5 // Raio do cursor do mouse
#define TAM_BOTAO_LARGURA 120 // Largura do botão
#define TAM_BOTAO_ALTURA 40   // Altura do botão
#define BOTAO_MARGEM 10       // Margem em relação à borda inferior

typedef struct {
    int linha1, coluna1;
    int linha2, coluna2;
    int selecionadas;
} celula_selecionada_t;

// Tem que chamar antes pra não dar erro
bool celula_ocupada(char tabuleiro[NLIN][NCOL], int linha, int coluna);
void preenche_tabuleiro(char tabuleiro[NLIN][NCOL]);

int ultima_coluna_ocupada(char tabuleiro[NLIN][NCOL], int linha) {
    for (int c = NCOL - 1; c >= 0; c--) {
        if (celula_ocupada(tabuleiro, linha, c)) return c;
    }
    return -1;
}

int primeira_coluna_ocupada(char tabuleiro[NLIN][NCOL], int linha) {
    for (int c = 0; c < NCOL; c++) {
        if (celula_ocupada(tabuleiro, linha, c)) return c;
    }
    return -1;
}

int primeira_linha_ocupada(char tabuleiro[NLIN][NCOL]) {
    for (int l = 0; l < NLIN; l++) {
        if (ultima_coluna_ocupada(tabuleiro, l) != -1) return l;
    }
    return -1;
}

bool verifica_vitoria(char tabuleiro[NLIN][NCOL]) {
    for (int i = 0; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            if (tabuleiro[i][j] != '.') {
                return false;
            }
        }
    }
    printf("Parabéns! Você venceu o jogo, o tabuleiro foi completamente esvaziado.\n");
    return true;
}

bool pode_fazer_repovoamento(char tabuleiro[NLIN][NCOL]) {
    int total_ocupadas = 0;
    for (int l = 0; l < NLIN; l++) {
        for (int c = 0; c < NCOL; c++) {
            if (celula_ocupada(tabuleiro, l, c)) {
                total_ocupadas++;
            }
        }
    }
    if (total_ocupadas * 2 > NLIN * NCOL) {
        printf("Não é possível fazer o repovoamento: espaço insuficiente!\n");
        return false;
    }
    return true;
}

bool primeira_celula_ocupada(char tabuleiro[NLIN][NCOL], int *linha, int *coluna) {
    for (int l = 0; l < NLIN; l++) {
        for (int c = 0; c < NCOL; c++) {
            if (celula_ocupada(tabuleiro, l, c)) {
                *linha = l;
                *coluna = c;
                return true;
            }
        }
    }
    return false;
}

bool clique_no_botao_reiniciar(rato_t rato) {
    ponto_t inicio = {BOTAO_MARGEM + 2 * (TAM_BOTAO_LARGURA + BOTAO_MARGEM),
                      NLIN * TAM_CASA + BOTAO_MARGEM};
    return rato.posicao.x >= inicio.x &&
           rato.posicao.x <= inicio.x + TAM_BOTAO_LARGURA &&
           rato.posicao.y >= inicio.y &&
           rato.posicao.y <= inicio.y + TAM_BOTAO_ALTURA;
}

bool clique_no_botao_repovoamento(rato_t rato) {
    ponto_t inicio = {BOTAO_MARGEM + TAM_BOTAO_LARGURA + BOTAO_MARGEM,
                      NLIN * TAM_CASA + BOTAO_MARGEM};
    return rato.posicao.x >= inicio.x &&
           rato.posicao.x <= inicio.x + TAM_BOTAO_LARGURA &&
           rato.posicao.y >= inicio.y &&
           rato.posicao.y <= inicio.y + TAM_BOTAO_ALTURA;
}

bool vizinhas(char tabuleiro[NLIN][NCOL], int l1, int c1, int l2, int c2) {
    if (l1 == l2) {
        int min = c1 < c2 ? c1 : c2;
        int max = c1 > c2 ? c1 : c2;
        for (int i = min + 1; i < max; i++) {
            if (celula_ocupada(tabuleiro, l1, i)) return false;
        }
        return true;
    }
    if (c1 == c2) {
        int min = l1 < l2 ? l1 : l2;
        int max = l1 > l2 ? l1 : l2;
        for (int i = min + 1; i < max; i++) {
            if (celula_ocupada(tabuleiro, i, c1)) return false;
        }
        return true;
    }
    if (abs(l1 - l2) == abs(c1 - c2)) {
        int dlinha = (l2 > l1) ? 1 : -1;
        int dcoluna = (c2 > c1) ? 1 : -1;
        for (int i = 1; i < abs(l2 - l1); i++) {
            if (celula_ocupada(tabuleiro, l1 + i * dlinha, c1 + i * dcoluna)) return false;
        }
        return true;
    }
    if (l2 == l1 + 1) {
        int ultima_col = ultima_coluna_ocupada(tabuleiro, l1);
        int primeira_col = primeira_coluna_ocupada(tabuleiro, l2);
        if (ultima_col == c1 && primeira_col == c2) return true;
    }
    int ultima_linha = NLIN - 1;
    int primeira_linha = primeira_linha_ocupada(tabuleiro);
    if (l1 == ultima_linha && l2 == primeira_linha) {
        int ultima_col = ultima_coluna_ocupada(tabuleiro, l1);
        int primeira_col = primeira_coluna_ocupada(tabuleiro, l2);
        if (ultima_col == c1 && primeira_col == c2) return true;
    }
    return false;
}

bool jogada_valida(char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada) {
    int l1 = selecionada->linha1, c1 = selecionada->coluna1;
    int l2 = selecionada->linha2, c2 = selecionada->coluna2;
    if (!vizinhas(tabuleiro, l1, c1, l2, c2)) return false;
    char n1 = tabuleiro[l1][c1];
    char n2 = tabuleiro[l2][c2];
    return (n1 == n2 || (n1 - '0' + n2 - '0' == 10));
}

bool celula_vazia(char tabuleiro[NLIN][NCOL], int linha, int coluna) {
    if (linha < 0 || linha >= NLIN || coluna < 0 || coluna >= NCOL) {
        return true;
    }
    return tabuleiro[linha][coluna] == '.';
}

bool celula_ocupada(char tabuleiro[NLIN][NCOL], int linha, int coluna) {
    if (linha < 0 || linha >= NLIN || coluna < 0 || coluna >= NCOL) {
        return false;
    }
    return tabuleiro[linha][coluna] >= '1' && tabuleiro[linha][coluna] <= '9';
}

bool clique_no_botao(rato_t rato) {
    ponto_t inicio = {BOTAO_MARGEM, NLIN * TAM_CASA + BOTAO_MARGEM};
    return rato.posicao.x >= inicio.x &&
           rato.posicao.x <= inicio.x + TAM_BOTAO_LARGURA &&
           rato.posicao.y >= inicio.y &&
           rato.posicao.y <= inicio.y + TAM_BOTAO_ALTURA;
}

cor_t determina_cor(char numero) {
    switch (numero) {
        case '1':
        case '9':
            return (cor_t){1.0, 0.8, 0.8, 1.0};
        case '2':
        case '8':
            return (cor_t){0.8, 1.0, 0.8, 1.0};
        case '3':
        case '7':
            return (cor_t){0.8, 0.8, 1.0, 1.0};
        case '4':
        case '6':
            return (cor_t){1.0, 1.0, 0.6, 1.0};
        case '5':
            return (cor_t){1.0, 0.6, 1.0, 1.0};
        default:
            return (cor_t){0.8, 0.8, 0.8, 1.0};
    }
}

void salvar_pontuacao_em_arquivo(int pontuacao) {
    FILE *arquivo = fopen("pontuacao.txt", "r");
    int pontuacoes[11];
    int total_pontuacoes = 0;
    if (arquivo != NULL) {
        while (fscanf(arquivo, "%d", &pontuacoes[total_pontuacoes]) == 1) {
            total_pontuacoes++;
            if (total_pontuacoes >= 10) break;
        }
        fclose(arquivo);
    }
    pontuacoes[total_pontuacoes++] = pontuacao;
    for (int i = 0; i < total_pontuacoes - 1; i++) {
        for (int j = i + 1; j < total_pontuacoes; j++) {
            if (pontuacoes[i] < pontuacoes[j]) {
                int temp = pontuacoes[i];
                pontuacoes[i] = pontuacoes[j];
                pontuacoes[j] = temp;
            }
        }
    }
    if (total_pontuacoes > 10) {
        total_pontuacoes = 10;
    }
    arquivo = fopen("pontuacao.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar as pontuações.\n");
        return;
    }
    for (int i = 0; i < total_pontuacoes; i++) {
        fprintf(arquivo, "%d\n", pontuacoes[i]);
    }
    fclose(arquivo);
    printf("Pontuação salva em 'pontuacao.txt'.\n");
}

void reiniciar_jogo(char tabuleiro[NLIN][NCOL], int *pontuacao) {
    preenche_tabuleiro(tabuleiro);
    *pontuacao = 0;
    printf("Jogo reiniciado\n");
}

void desenha_botao_reiniciar() {
    cor_t cor_fundo = {0.0, 1.0, 0.0, 1.0};
    cor_t cor_borda = {0.0, 0.0, 0.0, 1.0};  
    cor_t cor_texto = {1.0, 1.0, 1.0, 1.0}; 
    ponto_t inicio = {BOTAO_MARGEM + 2 * (TAM_BOTAO_LARGURA + BOTAO_MARGEM),
                      NLIN * TAM_CASA + BOTAO_MARGEM};
    retangulo_t botao = {inicio, {TAM_BOTAO_LARGURA, TAM_BOTAO_ALTURA}};
    j_retangulo(botao, 2.0, cor_borda, cor_fundo);
    ponto_t pos_texto = {inicio.x + 30, inicio.y + 25};
    j_texto(pos_texto, 15, cor_texto, "Reiniciar");
}

void somar_pontuacao(char tabuleiro[NLIN][NCOL], int *pontuacao) {
    int linhas_apagadas = 0;
    for (int l = 0; l < NLIN; l++) {
        bool linha_vazia = true;
        for (int c = 0; c < NCOL; c++) {
            if (tabuleiro[l][c] != '.') {
                linha_vazia = false;
                break;
            }
        }
        if (linha_vazia) {
            linhas_apagadas++;
        }
    }
    if (linhas_apagadas > 0) {
        *pontuacao += linhas_apagadas * 109;
        printf("%d linha(s) apagada(s). Pontuação atual: %d pontos\n", linhas_apagadas, *pontuacao);
    } else {
        printf("Nenhuma linha foi esvaziada.\n");
    }
}

void atualizar_pontuacao(int *pontuacao, int pontos) {
    *pontuacao += pontos;
}

void exibir_pontuacao(int pontuacao) {
    printf("Pontuação final: %d pontos\n", pontuacao);
}

void repovoar_tabuleiro(char tabuleiro[NLIN][NCOL]) {
    char numeros_ocupados[NLIN * NCOL];
    int total_ocupados = 0;
    for (int l = 0; l < NLIN; l++) {
        for (int c = 0; c < NCOL; c++) {
            if (celula_ocupada(tabuleiro, l, c)) {
                numeros_ocupados[total_ocupados++] = tabuleiro[l][c];
            }
        }
    }
    if (total_ocupados == 0) {
        printf("Nenhuma célula ocupada para repovoar.\n");
        return;
    }
    int novas_celulas = total_ocupados;
    int repovoadas = 0;       
    int linha_inicio, coluna_inicio;
    if (!primeira_celula_ocupada(tabuleiro, &linha_inicio, &coluna_inicio)) {
        printf("Nenhuma célula ocupada encontrada no tabuleiro.\n");
        return;
    }
    for (int l = linha_inicio; l >= 0 && repovoadas < novas_celulas; l--) {
        int inicio_coluna = (l == linha_inicio) ? coluna_inicio - 1 : NCOL - 1;
        for (int c = inicio_coluna; c >= 0 && repovoadas < novas_celulas; c--) {
            if (tabuleiro[l][c] == '.') {
                tabuleiro[l][c] = numeros_ocupados[(total_ocupados - 1 - repovoadas) % total_ocupados];
                repovoadas++;
            }
        }
    }
    printf("Tabuleiro repovoado com %d novas células.\n", repovoadas);
}

void desenha_botao_repovoamento() {
    cor_t cor_fundo = {0.0, 0.0, 1.0, 1.0};
    cor_t cor_borda = {0.0, 0.0, 0.0, 1.0};
    cor_t cor_texto = {1.0, 1.0, 1.0, 1.0};
    ponto_t inicio = {BOTAO_MARGEM + TAM_BOTAO_LARGURA + BOTAO_MARGEM,
                      NLIN * TAM_CASA + BOTAO_MARGEM};
    retangulo_t botao = {inicio, {TAM_BOTAO_LARGURA, TAM_BOTAO_ALTURA}};
    j_retangulo(botao, 2.0, cor_borda, cor_fundo);
    ponto_t pos_texto = {inicio.x + 10, inicio.y + 25};
    j_texto(pos_texto, 15, cor_texto, "Repovoamento");
}

void verificar_linhas_vazias(char tabuleiro[NLIN][NCOL], int *pontuacao) {
    for (int l = NLIN - 1; l >= NLIN / 2; l--) {
        bool linha_vazia = true;
        for (int c = 0; c < NCOL; c++) {
            if (tabuleiro[l][c] != '.') {
                linha_vazia = false;
                break;
            }
        }
        if (linha_vazia) {
            for (int i = l; i > 0; i--) {
                for (int c = 0; c < NCOL; c++) {
                    tabuleiro[i][c] = tabuleiro[i - 1][c];
                }
            }
            for (int c = 0; c < NCOL; c++) {
                tabuleiro[0][c] = '.';
            }
            printf("Linha %d foi esvaziada e movida para baixo.\n", l);
            *pontuacao += 109;
            printf("Pontuação atual: %d pontos\n", *pontuacao);
        }
    }
}

void destacar_celula(int linha, int coluna, char tabuleiro[NLIN][NCOL]) {
    ponto_t inicio = {coluna * TAM_CASA, linha * TAM_CASA};
    retangulo_t destaque = {inicio, {TAM_CASA, TAM_CASA}};
    cor_t cor_destaque = determina_cor(tabuleiro[linha][coluna]);
    cor_destaque.opacidade = 0.1;
    j_retangulo(destaque, 1.0, cor_destaque, cor_destaque);
}

void realizar_jogada(char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada, int *pontuacao) {
    if (selecionada->linha1 == selecionada->linha2 && selecionada->coluna1 == selecionada->coluna2) {
        printf("Você selecionou a mesma célula duas vezes. Jogada inválida!\n");
        selecionada->selecionadas = 0;
        return;
    }
    int l1 = selecionada->linha1, c1 = selecionada->coluna1;
    int l2 = selecionada->linha2, c2 = selecionada->coluna2;
    if (selecionada->selecionadas == 2 && vizinhas(tabuleiro, l1, c1, l2, c2)) {
        char num1 = tabuleiro[l1][c1];
        char num2 = tabuleiro[l2][c2];
        if (num1 == num2 || (num1 - '0' + num2 - '0' == 10)) {
            tabuleiro[l1][c1] = '.';
            tabuleiro[l2][c2] = '.';
            *pontuacao += 9;
            printf("Jogada válida realizada!\n");
            printf("Pontuação atual: %d pontos\n", *pontuacao);
            verificar_linhas_vazias(tabuleiro, pontuacao);
        } else {
            printf("Jogada inválida! As células não são iguais e não somam 10.\n");
        }
    }
    selecionada->selecionadas = 0;
}

void processa_clique(rato_t rato, char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada, int *pontuacao) {
    int coluna = rato.posicao.x / TAM_CASA;
    int linha = rato.posicao.y / TAM_CASA;
    if (linha >= 0 && linha < NLIN && coluna >= 0 && coluna < NCOL) {
        if (celula_ocupada(tabuleiro, linha, coluna)) {
            if (selecionada->selecionadas == 0) {
                selecionada->linha1 = linha;
                selecionada->coluna1 = coluna;
                selecionada->selecionadas = 1;
                printf("Primeira célula selecionada: (%d, %d)\n", linha, coluna);
            } else if (selecionada->selecionadas == 1) {
                if (selecionada->linha1 == linha && selecionada->coluna1 == coluna) {
                    printf("Você selecionou a mesma célula novamente. A seleção foi cancelada.\n");
                    selecionada->selecionadas = 0;
                } else {
                    selecionada->linha2 = linha;
                    selecionada->coluna2 = coluna;
                    selecionada->selecionadas = 2;
                    printf("Segunda célula selecionada: (%d, %d)\n", linha, coluna);
                }
            } else {
                selecionada->selecionadas = 0;
                printf("Seleção reiniciada.\n");
            }
        }
    }
    if (selecionada->selecionadas == 2) {
        realizar_jogada(tabuleiro, selecionada, pontuacao);
    }
}

void preenche_tabuleiro(char tabuleiro[NLIN][NCOL]) {
    for (int i = 0; i < NLIN / 2; i++) {
        for (int j = 0; j < NCOL; j++) {
            tabuleiro[i][j] = '.';
        }
    }
    for (int i = NLIN / 2; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            tabuleiro[i][j] = '1' + (rand() % 9);
        }
    }
}


void desenha_tabuleiro(char tabuleiro[NLIN][NCOL]) {
    cor_t cor_borda = {0.0, 0.0, 0.0, 1.0};
    cor_t cor_numero = {0.0, 0.0, 0.0, 1.0};
    for (int i = 0; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            ponto_t inicio = {j * TAM_CASA, i * TAM_CASA};
            retangulo_t casa = {inicio, {TAM_CASA, TAM_CASA}};
            cor_t cor_celula = determina_cor(tabuleiro[i][j]);
            j_retangulo(casa, 1.0, cor_borda, cor_celula);
            if (tabuleiro[i][j] != '.') {
                char texto[2] = {tabuleiro[i][j], '\0'};
                ponto_t pos_texto = {
                    inicio.x + TAM_CASA / 2 - 8,
                    inicio.y + TAM_CASA / 2
                };
                j_texto(pos_texto, TAM_CASA / 2, cor_numero, texto);
            }
        }
    }
}

void desenha_cursor(rato_t rato) {
    cor_t cor_cursor = {1.0, 0.0, 0.0, 1.0};
    circulo_t cursor = {rato.posicao, TAM_CURSOR};
    j_circulo(cursor, 0, cor_cursor, cor_cursor);
}

void desenha_botao() {
    cor_t cor_fundo = {1.0, 0.0, 0.0, 1.0};
    cor_t cor_borda = {0.0, 0.0, 0.0, 1.0}; 
    cor_t cor_texto = {1.0, 1.0, 1.0, 1.0};  
    ponto_t inicio = {BOTAO_MARGEM, NLIN * TAM_CASA + BOTAO_MARGEM};
    retangulo_t botao = {inicio, {TAM_BOTAO_LARGURA, TAM_BOTAO_ALTURA}};
    j_retangulo(botao, 2.0, cor_borda, cor_fundo);
    ponto_t pos_texto = {inicio.x + 30, inicio.y + 25};
    j_texto(pos_texto, 15, cor_texto, "Desistir");
}

int main() {
    srand(time(NULL));
    tamanho_t tamanho_janela = {NCOL * TAM_CASA, NLIN * TAM_CASA + TAM_BOTAO_ALTURA + BOTAO_MARGEM};
    t_inicializa(tamanho_janela, "Jogo de Tabuleiro");
    char tabuleiro[NLIN][NCOL];
    preenche_tabuleiro(tabuleiro);
    celula_selecionada_t selecionada = { .linha1 = -1, .coluna1 = -1, .linha2 = -1, .coluna2 = -1, .selecionadas = 0 };
    int pontuacao = 0;
    while (true) {
        rato_t rato = j_rato();
        if (rato.clicado[0]) {
            processa_clique(rato, tabuleiro, &selecionada, &pontuacao);  // Passando o ponteiro para pontuação
        }
        desenha_tabuleiro(tabuleiro);
        if (selecionada.selecionadas == 1) {
            destacar_celula(selecionada.linha1, selecionada.coluna1, tabuleiro);
        } else if (selecionada.selecionadas == 2) {
            destacar_celula(selecionada.linha2, selecionada.coluna2, tabuleiro);
        }
        if (selecionada.selecionadas == 2) {
            realizar_jogada(tabuleiro, &selecionada, &pontuacao);
        }
        desenha_cursor(rato);
        desenha_botao();
        desenha_botao_repovoamento();
        desenha_botao_reiniciar();
        j_atualiza();
        if (rato.clicado[0] && clique_no_botao(rato)) {
            printf("O jogador desistiu do jogo.\n");
            salvar_pontuacao_em_arquivo(pontuacao);
            break;
        }
        if (rato.clicado[0] && clique_no_botao_repovoamento(rato)) {
            if (!pode_fazer_repovoamento(tabuleiro)) {
                printf("Não foi possível realizar o repovoamento.\n");
                salvar_pontuacao_em_arquivo(pontuacao);
                break;
            } else {
                repovoar_tabuleiro(tabuleiro);
            }
        }
        if (verifica_vitoria(tabuleiro)) {
            salvar_pontuacao_em_arquivo(pontuacao);
            break;
        }
        if (rato.clicado[0] && clique_no_botao_reiniciar(rato)) {
            salvar_pontuacao_em_arquivo(pontuacao);
            reiniciar_jogo(tabuleiro, &pontuacao);
        }
    }
    j_finaliza();
    return 0;
}