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
    int selecionadas; // 0: nenhuma, 1: primeira selecionada, 2: duas selecionadas
} celula_selecionada_t;

// Protótipos de funções
bool celula_ocupada(char tabuleiro[NLIN][NCOL], int linha, int coluna);
void processa_clique(rato_t rato, char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada, int *pontuacao);
bool clique_no_botao(rato_t rato);
void desenha_botao();
void preenche_tabuleiro(char tabuleiro[NLIN][NCOL]);
bool vizinhas(char tabuleiro[NLIN][NCOL], int l1, int c1, int l2, int c2);
bool jogada_valida(char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada);
void realiza_jogada(char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada);
int ultima_coluna_ocupada(char tabuleiro[NLIN][NCOL], int linha);
int primeira_coluna_ocupada(char tabuleiro[NLIN][NCOL], int linha);
int primeira_linha_ocupada(char tabuleiro[NLIN][NCOL]);

// Retorna a última coluna ocupada de uma linha ou -1 se não houver
int ultima_coluna_ocupada(char tabuleiro[NLIN][NCOL], int linha) {
    for (int c = NCOL - 1; c >= 0; c--) {
        if (celula_ocupada(tabuleiro, linha, c)) return c;
    }
    return -1;
}

// Retorna a primeira coluna ocupada de uma linha ou -1 se não houver
int primeira_coluna_ocupada(char tabuleiro[NLIN][NCOL], int linha) {
    for (int c = 0; c < NCOL; c++) {
        if (celula_ocupada(tabuleiro, linha, c)) return c;
    }
    return -1;
}

// Retorna a primeira linha que possui casas ocupadas ou -1 se todas estiverem vazias
int primeira_linha_ocupada(char tabuleiro[NLIN][NCOL]) {
    for (int l = 0; l < NLIN; l++) {
        if (ultima_coluna_ocupada(tabuleiro, l) != -1) return l;
    }
    return -1;
}

#include <stdbool.h>
#include <stdio.h>

// Função para verificar se o tabuleiro está cheio (sem células vazias)
// Função para verificar se o tabuleiro está vazio (vencedor)

// Função para verificar se o tabuleiro foi completamente esvaziado
bool verifica_vitoria(char tabuleiro[NLIN][NCOL]) {
    // Percorre todo o tabuleiro
    for (int i = 0; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            // Se encontrar qualquer célula ocupada, retorna falso
            if (tabuleiro[i][j] != '.') {
                return false;
            }
        }
    }

    // Se nenhuma célula ocupada for encontrada, vitória!
    printf("Parabéns! Você venceu o jogo, o tabuleiro foi completamente esvaziado.\n");
    return true;
}

bool pode_fazer_repovoamento(char tabuleiro[NLIN][NCOL]) {
    int total_ocupadas = 0;

    // Contabiliza o número de células ocupadas no tabuleiro
    for (int l = 0; l < NLIN; l++) {
        for (int c = 0; c < NCOL; c++) {
            if (celula_ocupada(tabuleiro, l, c)) {
                total_ocupadas++;
            }
        }
    }

    // Verifica se há espaço suficiente para o repovoamento
    if (total_ocupadas * 2 > NLIN * NCOL) {
        printf("Não é possível fazer o repovoamento: espaço insuficiente!\n");
        return false; // O jogador perde
    }

    return true; // Repovoamento possível
}


// Função para encontrar a primeira célula ocupada no tabuleiro
// Retorna true se encontrar uma célula ocupada, e atualiza as variáveis linha e coluna
bool primeira_celula_ocupada(char tabuleiro[NLIN][NCOL], int *linha, int *coluna) {
    for (int l = 0; l < NLIN; l++) {
        for (int c = 0; c < NCOL; c++) {
            if (celula_ocupada(tabuleiro, l, c)) {
                *linha = l;    // Atualiza a linha
                *coluna = c;   // Atualiza a coluna
                return true;   // Encontra a primeira célula ocupada
            }
        }
    }
    return false; // Nenhuma célula ocupada encontrada
}

// Função para verificar se o clique está dentro do botão de reinício
bool clique_no_botao_reiniciar(rato_t rato) {
    ponto_t inicio = {BOTAO_MARGEM + 2 * (TAM_BOTAO_LARGURA + BOTAO_MARGEM), // Ao lado do botão de repovoamento
                      NLIN * TAM_CASA + BOTAO_MARGEM};

    // Verifica se o clique está dentro do botão
    return rato.posicao.x >= inicio.x &&
           rato.posicao.x <= inicio.x + TAM_BOTAO_LARGURA &&
           rato.posicao.y >= inicio.y &&
           rato.posicao.y <= inicio.y + TAM_BOTAO_ALTURA;
}

// Função para verificar se o clique está dentro do botão de repovoamento
bool clique_no_botao_repovoamento(rato_t rato) {
    ponto_t inicio = {BOTAO_MARGEM + TAM_BOTAO_LARGURA + BOTAO_MARGEM, // Ao lado do botão de desistência
                      NLIN * TAM_CASA + BOTAO_MARGEM};

    // Verifica se o clique está dentro do botão
    return rato.posicao.x >= inicio.x &&
           rato.posicao.x <= inicio.x + TAM_BOTAO_LARGURA &&
           rato.posicao.y >= inicio.y &&
           rato.posicao.y <= inicio.y + TAM_BOTAO_ALTURA;
}

// Verifica se duas células são vizinhas (regras estendidas)
bool vizinhas(char tabuleiro[NLIN][NCOL], int l1, int c1, int l2, int c2) {
    if (l1 == l2) { // Mesma linha
        int min = c1 < c2 ? c1 : c2;
        int max = c1 > c2 ? c1 : c2;
        for (int i = min + 1; i < max; i++) {
            if (celula_ocupada(tabuleiro, l1, i)) return false;
        }
        return true;
    }
    if (c1 == c2) { // Mesma coluna
        int min = l1 < l2 ? l1 : l2;
        int max = l1 > l2 ? l1 : l2;
        for (int i = min + 1; i < max; i++) {
            if (celula_ocupada(tabuleiro, i, c1)) return false;
        }
        return true;
    }
    if (abs(l1 - l2) == abs(c1 - c2)) { // Mesma diagonal
        int dlinha = (l2 > l1) ? 1 : -1;
        int dcoluna = (c2 > c1) ? 1 : -1;
        for (int i = 1; i < abs(l2 - l1); i++) {
            if (celula_ocupada(tabuleiro, l1 + i * dlinha, c1 + i * dcoluna)) return false;
        }
        return true;
    }

    // Última da linha + Primeira da próxima linha
    if (l2 == l1 + 1) {
        int ultima_col = ultima_coluna_ocupada(tabuleiro, l1);
        int primeira_col = primeira_coluna_ocupada(tabuleiro, l2);
        if (ultima_col == c1 && primeira_col == c2) return true;
    }

    // Última da última linha + Primeira da primeira linha com ocupação
    int ultima_linha = NLIN - 1;
    int primeira_linha = primeira_linha_ocupada(tabuleiro);
    if (l1 == ultima_linha && l2 == primeira_linha) {
        int ultima_col = ultima_coluna_ocupada(tabuleiro, l1);
        int primeira_col = primeira_coluna_ocupada(tabuleiro, l2);
        if (ultima_col == c1 && primeira_col == c2) return true;
    }

    return false;
}

// Verifica se a jogada é válida
bool jogada_valida(char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada) {
    int l1 = selecionada->linha1, c1 = selecionada->coluna1;
    int l2 = selecionada->linha2, c2 = selecionada->coluna2;

    if (!vizinhas(tabuleiro, l1, c1, l2, c2)) return false;

    char n1 = tabuleiro[l1][c1];
    char n2 = tabuleiro[l2][c2];

    return (n1 == n2 || (n1 - '0' + n2 - '0' == 10));
}

// Função para verificar se uma célula é vazia
bool celula_vazia(char tabuleiro[NLIN][NCOL], int linha, int coluna) {
    if (linha < 0 || linha >= NLIN || coluna < 0 || coluna >= NCOL) {
        return true; // Fora dos limites é considerado vazio
    }
    return tabuleiro[linha][coluna] == '.';
}

// Função para verificar se uma célula está ocupada
bool celula_ocupada(char tabuleiro[NLIN][NCOL], int linha, int coluna) {
    // Verifica se as coordenadas estão dentro dos limites do tabuleiro
    if (linha < 0 || linha >= NLIN || coluna < 0 || coluna >= NCOL) {
        return false; // Fora dos limites é considerado não ocupado
    }

    // Retorna true se a célula contiver um número (de '1' a '9')
    return tabuleiro[linha][coluna] >= '1' && tabuleiro[linha][coluna] <= '9';
}

// Função para verificar se o clique está dentro do botão de desistência
bool clique_no_botao(rato_t rato) {
    ponto_t inicio = {BOTAO_MARGEM, NLIN * TAM_CASA + BOTAO_MARGEM};

    // Verifica se o clique está dentro do botão
    return rato.posicao.x >= inicio.x &&
           rato.posicao.x <= inicio.x + TAM_BOTAO_LARGURA &&
           rato.posicao.y >= inicio.y &&
           rato.posicao.y <= inicio.y + TAM_BOTAO_ALTURA;
}

// Função para determinar a cor da célula com base no número
cor_t determina_cor(char numero) {
    switch (numero) {
        case '1':
        case '9':
            return (cor_t){1.0, 0.8, 0.8, 1.0}; // Rosa claro
        case '2':
        case '8':
            return (cor_t){0.8, 1.0, 0.8, 1.0}; // Verde claro
        case '3':
        case '7':
            return (cor_t){0.8, 0.8, 1.0, 1.0}; // Azul claro
        case '4':
        case '6':
            return (cor_t){1.0, 1.0, 0.6, 1.0}; // Amarelo claro
        case '5':
            return (cor_t){1.0, 0.6, 1.0, 1.0}; // Roxo claro
        default:
            return (cor_t){0.8, 0.8, 0.8, 1.0}; // Cinza claro para células vazias
    }
}

void salvar_pontuacao_em_arquivo(int pontuacao) {
    FILE *arquivo = fopen("pontuacao.txt", "a");  // Abre o arquivo no modo de adição

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar a pontuação.\n");
        return;
    }

    // Escreve a pontuação no arquivo
    fprintf(arquivo, "Pontuação do jogo: %d\n", pontuacao);

    fclose(arquivo);  // Fecha o arquivo
}

// Função para reiniciar o jogo
void reiniciar_jogo(char tabuleiro[NLIN][NCOL], int *pontuacao) {
    // Limpa o tabuleiro
    preenche_tabuleiro(tabuleiro);

    // Zera a pontuação
    *pontuacao = 0;

    printf("Jogo reiniciado. Pontuação zerada.\n");
}

// Função para desenhar o botão de reinício
void desenha_botao_reiniciar() {
    cor_t cor_fundo = {0.0, 1.0, 0.0, 1.0};  // Verde para o botão
    cor_t cor_borda = {0.0, 0.0, 0.0, 1.0};  // Preto para a borda
    cor_t cor_texto = {1.0, 1.0, 1.0, 1.0};  // Branco para o texto

    // Calcula a posição do botão de reinício
    ponto_t inicio = {BOTAO_MARGEM + 2 * (TAM_BOTAO_LARGURA + BOTAO_MARGEM), // Ao lado do botão de repovoamento
                      NLIN * TAM_CASA + BOTAO_MARGEM};
    retangulo_t botao = {inicio, {TAM_BOTAO_LARGURA, TAM_BOTAO_ALTURA}};

    // Desenha o botão
    j_retangulo(botao, 2.0, cor_borda, cor_fundo);

    // Centraliza o texto "Reiniciar" no botão
    ponto_t pos_texto = {inicio.x + 30, inicio.y + 25};
    j_texto(pos_texto, 20, cor_texto, "Reiniciar");
}

void somar_pontuacao(char tabuleiro[NLIN][NCOL], int *pontuacao) {
    int linhas_apagadas = 0; // Contador de linhas apagadas

    // Verifica cada linha do tabuleiro
    for (int l = 0; l < NLIN; l++) {
        bool linha_vazia = true;

        // Verifica se a linha está vazia
        for (int c = 0; c < NCOL; c++) {
            if (tabuleiro[l][c] != '.') {
                linha_vazia = false;
                break;
            }
        }

        // Se a linha foi apagada, conta
        if (linha_vazia) {
            linhas_apagadas++;
        }
    }

    // Se alguma linha foi apagada, soma a pontuação
    if (linhas_apagadas > 0) {
        *pontuacao += linhas_apagadas * 109; // Cada linha apagada vale 109 pontos
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

    // Coleta os números ocupados no tabuleiro
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

    int novas_celulas = total_ocupados; // Duplicar o número de células ocupadas
    int repovoadas = 0;                 // Contador de células repovoadas

    int linha_inicio, coluna_inicio;
    if (!primeira_celula_ocupada(tabuleiro, &linha_inicio, &coluna_inicio)) {
        printf("Nenhuma célula ocupada encontrada no tabuleiro.\n");
        return;
    }

    // Percorre as linhas para preencher antes da primeira célula ocupada
    for (int l = linha_inicio; l >= 0 && repovoadas < novas_celulas; l--) {
        int inicio_coluna = (l == linha_inicio) ? coluna_inicio - 1 : NCOL - 1;

        for (int c = inicio_coluna; c >= 0 && repovoadas < novas_celulas; c--) {
            if (tabuleiro[l][c] == '.') {
                // Insere os números em ordem inversa
                tabuleiro[l][c] = numeros_ocupados[(total_ocupados - 1 - repovoadas) % total_ocupados];
                repovoadas++;
            }
        }
    }

    printf("Tabuleiro repovoado com %d novas células.\n", repovoadas);
}

// Função para desenhar o botão de repovoamento
void desenha_botao_repovoamento() {
    cor_t cor_fundo = {0.0, 0.0, 1.0, 1.0};  // Azul para o botão
    cor_t cor_borda = {0.0, 0.0, 0.0, 1.0};  // Preto para a borda
    cor_t cor_texto = {1.0, 1.0, 1.0, 1.0};  // Branco para o texto

    // Calcula a posição do botão de repovoamento
    ponto_t inicio = {BOTAO_MARGEM + TAM_BOTAO_LARGURA + BOTAO_MARGEM, // Ao lado do botão de desistência
                      NLIN * TAM_CASA + BOTAO_MARGEM};
    retangulo_t botao = {inicio, {TAM_BOTAO_LARGURA, TAM_BOTAO_ALTURA}};

    // Desenha o botão
    j_retangulo(botao, 2.0, cor_borda, cor_fundo);

    // Centraliza o texto "Repovoamento" no botão
    ponto_t pos_texto = {inicio.x + 10, inicio.y + 25};
    j_texto(pos_texto, 20, cor_texto, "Repovoamento");
}

// Função para verificar se alguma linha foi esvaziada e mover as linhas acima para baixo
void verificar_linhas_vazias(char tabuleiro[NLIN][NCOL], int *pontuacao) {
    for (int l = NLIN - 1; l >= NLIN / 2; l--) { // Começa da linha mais baixa
        bool linha_vazia = true;

        // Verifica se a linha l está vazia
        for (int c = 0; c < NCOL; c++) {
            if (tabuleiro[l][c] != '.') {
                linha_vazia = false;
                break;
            }
        }

        // Se a linha foi esvaziada, move as linhas acima para baixo
        if (linha_vazia) {
            // Move todas as linhas acima da linha l para baixo
            for (int i = l; i > 0; i--) {
                for (int c = 0; c < NCOL; c++) {
                    tabuleiro[i][c] = tabuleiro[i - 1][c];
                }
            }
            // Preenche a primeira linha com células vazias
            for (int c = 0; c < NCOL; c++) {
                tabuleiro[0][c] = '.';
            }

            printf("Linha %d foi esvaziada e movida para baixo.\n", l);
            *pontuacao += 109;  // Adiciona 109 pontos pela linha esvaziada
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

// Função para realizar a jogada
void realizar_jogada(char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada, int *pontuacao) {
    // Verifica se as células selecionadas são diferentes
    if (selecionada->linha1 == selecionada->linha2 && selecionada->coluna1 == selecionada->coluna2) {
        printf("Você selecionou a mesma célula duas vezes. Jogada inválida!\n");
        selecionada->selecionadas = 0; // Reinicia a seleção
        return; // Não realiza a jogada
    }

    int l1 = selecionada->linha1, c1 = selecionada->coluna1;
    int l2 = selecionada->linha2, c2 = selecionada->coluna2;

    // Verifica se as células selecionadas são vizinhas e se a jogada é válida
    if (selecionada->selecionadas == 2 && vizinhas(tabuleiro, l1, c1, l2, c2)) {
        char num1 = tabuleiro[l1][c1];
        char num2 = tabuleiro[l2][c2];
        
        // Verifica se os números são iguais ou somam 10
        if (num1 == num2 || (num1 - '0' + num2 - '0' == 10)) {
            // Realiza a jogada, esvaziando as células
            tabuleiro[l1][c1] = '.';
            tabuleiro[l2][c2] = '.';
            *pontuacao += 9; // Atualiza a pontuação

            printf("Jogada válida realizada!\n");
            printf("Pontuação atual: %d pontos\n", *pontuacao);

            // Verifica se alguma linha foi esvaziada e move as linhas
            verificar_linhas_vazias(tabuleiro, pontuacao);
        } else {
            printf("Jogada inválida! As células não são iguais e não somam 10.\n");
        }
    }
    selecionada->selecionadas = 0; // Reinicia a seleção
}

// Função para processar o clique do mouse
void processa_clique(rato_t rato, char tabuleiro[NLIN][NCOL], celula_selecionada_t *selecionada, int *pontuacao) {
    int coluna = rato.posicao.x / TAM_CASA;
    int linha = rato.posicao.y / TAM_CASA;

    if (linha >= 0 && linha < NLIN && coluna >= 0 && coluna < NCOL) {
        if (celula_ocupada(tabuleiro, linha, coluna)) {
            if (selecionada->selecionadas == 0) { // Primeira célula
                selecionada->linha1 = linha;
                selecionada->coluna1 = coluna;
                selecionada->selecionadas = 1;
                printf("Primeira célula selecionada: (%d, %d)\n", linha, coluna);
            } else if (selecionada->selecionadas == 1) { // Segunda célula
                // Verifica se a célula clicada é a mesma da primeira
                if (selecionada->linha1 == linha && selecionada->coluna1 == coluna) {
                    printf("Você selecionou a mesma célula novamente. A seleção foi cancelada.\n");
                    selecionada->selecionadas = 0; // Cancela a seleção
                } else {
                    selecionada->linha2 = linha;
                    selecionada->coluna2 = coluna;
                    selecionada->selecionadas = 2;
                    printf("Segunda célula selecionada: (%d, %d)\n", linha, coluna);
                }
            } else { // Reinicia a seleção
                selecionada->selecionadas = 0;
                printf("Seleção reiniciada.\n");
            }
        }
    }

    // Após o clique, realiza a jogada
    if (selecionada->selecionadas == 2) {
        realizar_jogada(tabuleiro, selecionada, pontuacao);
    }
}

void preenche_tabuleiro(char tabuleiro[NLIN][NCOL]) {
    // Preenche as primeiras 6 linhas com casas vazias ('.')
    for (int i = 0; i < NLIN / 2; i++) {
        for (int j = 0; j < NCOL; j++) {
            tabuleiro[i][j] = '.'; // Casa vazia
        }
    }

    // Preenche as últimas 6 linhas com números aleatórios (sem casas vazias)
    for (int i = NLIN / 2; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            tabuleiro[i][j] = '1' + (rand() % 9); // Números entre '1' e '9'
        }
    }
}

// Função para desenhar o tabuleiro
void desenha_tabuleiro(char tabuleiro[NLIN][NCOL]) {
    cor_t cor_borda = {0.0, 0.0, 0.0, 1.0};  // Preto para as bordas
    cor_t cor_numero = {0.0, 0.0, 0.0, 1.0}; // Preto para os números

    for (int i = 0; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            ponto_t inicio = {j * TAM_CASA, i * TAM_CASA};
            retangulo_t casa = {inicio, {TAM_CASA, TAM_CASA}};

            // Determina a cor da célula
            cor_t cor_celula = determina_cor(tabuleiro[i][j]);

            // Desenha a célula
            j_retangulo(casa, 1.0, cor_borda, cor_celula);

            // Desenha o conteúdo da célula (número ou vazio)
            if (tabuleiro[i][j] != '.') {
                char texto[2] = {tabuleiro[i][j], '\0'};

                // Centraliza o texto na célula
                ponto_t pos_texto = {
                    inicio.x + TAM_CASA / 2 - 8, // Ajuste horizontal (meio da célula)
                    inicio.y + TAM_CASA / 2      // Ajuste vertical (meio da célula)
                };

                j_texto(pos_texto, TAM_CASA / 2, cor_numero, texto);
            }
        }
    }
}

// Função para desenhar o cursor do mouse
void desenha_cursor(rato_t rato) {
    cor_t cor_cursor = {1.0, 0.0, 0.0, 1.0}; // Vermelho para o cursor
    circulo_t cursor = {rato.posicao, TAM_CURSOR};
    j_circulo(cursor, 0, cor_cursor, cor_cursor);
}

// Função para desenhar o botão de desistência
void desenha_botao() {
    cor_t cor_fundo = {1.0, 0.0, 0.0, 1.0};  // Vermelho para o botão
    cor_t cor_borda = {0.0, 0.0, 0.0, 1.0};  // Preto para a borda
    cor_t cor_texto = {1.0, 1.0, 1.0, 1.0};  // Branco para o texto

    // Calcula a posição do botão
    ponto_t inicio = {BOTAO_MARGEM, NLIN * TAM_CASA + BOTAO_MARGEM};
    retangulo_t botao = {inicio, {TAM_BOTAO_LARGURA, TAM_BOTAO_ALTURA}};

    // Desenha o botão
    j_retangulo(botao, 2.0, cor_borda, cor_fundo);

    // Centraliza o texto "Desistir" no botão
    ponto_t pos_texto = {inicio.x + 30, inicio.y + 25};
    j_texto(pos_texto, 20, cor_texto, "Desistir");
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

        // Destaca a célula selecionada
        if (selecionada.selecionadas == 1) {
            destacar_celula(selecionada.linha1, selecionada.coluna1, tabuleiro);
        } else if (selecionada.selecionadas == 2) {
            destacar_celula(selecionada.linha2, selecionada.coluna2, tabuleiro);
        }

        // Realiza a jogada se duas células forem selecionadas
        if (selecionada.selecionadas == 2) {
            realizar_jogada(tabuleiro, &selecionada, &pontuacao);
        }

        // Desenha o cursor, botões e atualiza a tela
        desenha_cursor(rato);
        desenha_botao();
        desenha_botao_repovoamento();
        desenha_botao_reiniciar(); // Desenha o botão de reinício
        j_atualiza();

        // Encerra o jogo ao clicar no botão de desistência
        if (rato.clicado[0] && clique_no_botao(rato)) {
            printf("O jogador desistiu do jogo.\n");
            break;
        }

        // Repovoa o tabuleiro ao clicar no botão de repovoamento
        if (rato.clicado[0] && clique_no_botao_repovoamento(rato)) {
            if (!pode_fazer_repovoamento(tabuleiro)) {
                printf("Não foi possível realizar o repovoamento.\n");
                break;  // Encerra o jogo se não for possível fazer o repovoamento
            } else {
                repovoar_tabuleiro(tabuleiro); // Executa o repovoamento
            }
        }

        // Verifica se o tabuleiro foi esvaziado e o jogador venceu
        if (verifica_vitoria(tabuleiro)) {
            salvar_pontuacao_em_arquivo(pontuacao);  // Salva a pontuação quando vencer
            break; // Sai do loop principal, encerrando o jogo
        }

        // Reinicia o jogo ao clicar no botão de reinício
        if (rato.clicado[0] && clique_no_botao_reiniciar(rato)) {
            reiniciar_jogo(tabuleiro, &pontuacao);  // Reinicia o jogo
        }
    }

    j_finaliza();
    return 0;
}