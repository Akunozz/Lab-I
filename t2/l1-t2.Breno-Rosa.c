/* l1-t2 Breno Rosa*/
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

  int n_rainhas(int n, char tab[]) {
/*
    Faça uma função que recebe um tabuleiro contendo rainhas e retorna um inteiro:
0 se o tabuleiro tem menos de n rainhas e não tem ameaça entre as rainhas existentes;
1 se existir ameaça entre quaisquer rainhas no tabuleiro;
2 se existirem n rainhas no tabuleiro, e não existir ameaças entre elas.
O tabuleiro está em um vetor de char, com cada caractere podendo ser '.' ou 'X' (posição livre ou ocupada por uma rainha); os primeiros n caracteres representam a primeira linha, os n caracteres seguintes a segunda e assim por diante.

A função deve suportar tabuleiro de qualquer tamanho. Para isso, recebe 2 argumentos: um inteiro com o tamanho do tabuleiro e um vetor de char que codifica o tabuleiro.
    return 1;
  }
*/


void desenha_tabuleiro(int n, char tab[], int lin, int col)
{
/*  t_cor_normal();
  t_limpa();
  //...
  t_atualiza();
*/
    return 1;
}

bool trata_teclado(int n, int tab[], int *ref_lin, int *ref_col)
{
/*Faça uma função para realizar o processamento da entrada de uma tecla, da seguinte forma:
se não houver tecla na entrada, não faz nada e retorna false;
se a tecla for 'w', 'a', 's', 'd', incrementa ou decrementa a linha ou coluna do cursor, sem permitir que tenham valores fora de 1 a n e retorna false;
se a tecla for enter ou espaço, altera o tabuleiro na posição do cursor, colocando uma rainha se não houver e retirando se houver e retorna false;
se a tecla for 'x', retorna true;
se a tecla for qualquer outra, não faz nada e retorna false.
*/
  if (!t_tem_tecla()) return false;
  //...
}

/*
Faça um programa, usando as funções anteriores, para permitir que o usuário preencha um tabuleiro. O programa deve encerrar quando o tabuleiro estiver corretamente preenchido ou quando a função da parte III retornar true. No primeiro caso, o programa deve felicitar o usuário e informar o tempo que ele levou para preencher o tabuleiro.

O tamanho do tabuleiro deve ser perguntado ao usuário no início da execução do programa.

A estrutura do programa deve ser algo como:

  inicializa variáveis e terminal
  repete {
    trata_teclado
    desenha_tabuleiro
    verifica se deve terminar
  }
  parabeniza o usuário se for o caso
  finaliza o terminal
*/


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
