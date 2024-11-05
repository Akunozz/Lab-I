#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Função para verificar se duas rainhas estão se ameaçando
bool ameaca(int p1, int p2) {
    int x1 = p1 / 4, y1 = p1 % 4; // Coordenadas da primeira rainha
    int x2 = p2 / 4, y2 = p2 % 4; // Coordenadas da segunda rainha
    // Verifica se estão na mesma linha, coluna ou diagonal
    return (x1 == x2) || (y1 == y2) || (abs(x1 - x2) == abs(y1 - y2));
}

// Função principal para verificar o tabuleiro
bool verificar_tabuleiro(const char *tabuleiro) {
    int posicoes_rainhas[4];
    int num_rainhas = 0;

    // Encontrar as posições das rainhas no vetor (representando um tabuleiro 4x4)
    for (int i = 0; i < 16; i++) {
        if (tabuleiro[i] == 'x') {
            if (num_rainhas < 4) {
                posicoes_rainhas[num_rainhas++] = i;
            } else {
                return false; // Mais de 4 rainhas encontradas
            }
        }
    }

    // Verificar se existem exatamente 4 rainhas
    if (num_rainhas != 4) {
        return false;
    }

    // Verificar se alguma rainha ameaça outra
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (ameaca(posicoes_rainhas[i], posicoes_rainhas[j])) {
                return false;
            }
        }
    }

    return true;
}

// Função para ler o tabuleiro do usuário e validar a entrada
bool ler_tabuleiro(char *tabuleiro) {
    printf("Digite o tabuleiro 4x4 (16 caracteres, use apenas '.' ou 'x'):\n");
    scanf("%16s", tabuleiro);

    // Verifica se a entrada tem exatamente 16 caracteres e apenas '.' ou 'x'
    if (strlen(tabuleiro) != 16) {
        printf("Entrada inválida! O tabuleiro deve ter exatamente 16 caracteres.\n");
        return false;
    }

    for (int i = 0; i < 16; i++) {
        if (tabuleiro[i] != '.' && tabuleiro[i] != 'x') {
            printf("Entrada inválida! Use apenas '.' ou 'x'.\n");
            return false;
        }
    }

    return true;
}

int main() {
    char tabuleiro[17]; // 16 caracteres + 1 para o terminador nulo

    // Ler e validar a entrada do usuário
    if (!ler_tabuleiro(tabuleiro)) {
        return 1; // Termina o programa se a entrada for inválida
    }

    // Verificar o tabuleiro
    if (verificar_tabuleiro(tabuleiro)) {
        printf("O tabuleiro é válido.\n");
    } else {
        printf("O tabuleiro não é válido.\n");
    }
return 0;
}
