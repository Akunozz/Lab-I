// t1 l124b Breno Rosa
#include <stdio.h>

// Função que desenha a figura 'a'
void desenha_figura_a(char c, int tamanho) {
    for (int i = 1; i <= tamanho; i++) {
        for (int j = 1; j <= i; j++) {
            printf("%c", c);
        }
        printf("\n");
    }
}

// Função que desenha a figura 'b'
void desenha_figura_b(char c, int tamanho) {
    for (int i = tamanho; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            printf("%c", c);
        }
        printf("\n");
    }
}

// Função que desenha a figura 'c'
void desenha_figura_c(char c, int tamanho) {
    for (int i = 1; i <= tamanho; i++) {
        for (int j = i; j < tamanho; j++) {
            printf(" ");
        }
        for (int k = 1; k <= i; k++) {
            printf("%c", c);
        }
        printf("\n");
    }
}

// Função que desenha a figura 'd'
void desenha_figura_d(char c, int tamanho) {
    for (int i = tamanho; i >= 1; i--) {
        for (int j = tamanho; j > i; j--) {
            printf(" ");
        }
        for (int k = 1; k <= i; k++) {
            printf("%c", c);
        }
        printf("\n");
    }
}

// Função que desenha a figura 'e'
void desenha_figura_e(char c, int tamanho) {
    for (int i = 1; i <= tamanho; i++) {
        for (int j = i; j < tamanho; j++) {
            printf(" ");
        }
        for (int k = 1; k <= (2 * i - 1); k++) {
            printf("%c", c);
        }
        printf("\n");
    }
}

// Função que desenha a figura 'f' 
void desenha_figura_f(char c, int tamanho) {
    for (int i = 1; i <= tamanho; i++) {
        for (int j = i; j < tamanho; j++) {
            printf(" ");
        }
        for (int k = 1; k <= (2 * i - 1); k++) {
            printf("%c", c);
        }
        printf("\n");
    }
    for (int i = tamanho - 1; i >= 1; i--) {
        for (int j = tamanho; j > i; j--) {
            printf(" ");
        }
        for (int k = 1; k <= (2 * i - 1); k++) {
            printf("%c", c);
        }
        printf("\n");
    }
}


// Função que desenha a figura 'g'
void desenha_figura_g(char c, int tamanho) {
    for (int i = 1; i <= tamanho; i++) {
        for (int j = i; j < tamanho; j++) {
            printf(" ");
        }
        printf("%c", c);
        if (i > 1) {
            for (int k = 1; k < (2 * i - 2); k++) {
                printf(" ");
            }
            printf("%c", c);
        }
        printf("\n");
    }
    for (int i = tamanho - 1; i >= 1; i--) {
        for (int j = tamanho; j > i; j--) {
            printf(" ");
        }
        printf("%c", c);
        if (i > 1) {
            for (int k = 1; k < (2 * i - 2); k++) {
                printf(" ");
            }
            printf("%c", c);
        }
        printf("\n");
    }
}

// Função que desenha a figura 'h'
void desenha_figura_h(char c, int tamanho) {
for (int j = 0; j < 2 * tamanho + 1; j++) {
        printf("%c", c);
    }
    printf("\n");
    for (int i = 1; i <= tamanho; i++) {
        printf("%c", c);
        for (int j = 1; j <= (tamanho - i); j++) {
            printf("%c", c);
        }
        for (int k = 1; k <= (2 * i - 1); k++) {
            if (k == 1 || k == (2 * i - 1)) {
                printf(" ");
            } else {
                printf("%c", c);
            }
        }
        for (int j = 1; j <= (tamanho - i); j++) {
            printf("%c", c);
        }
        printf("%c", c);
        printf("\n");
    }
    for (int i = tamanho - 1; i >= 1; i--) {
        printf("%c", c);

        for (int j = 1; j <= (tamanho - i); j++) {
            printf("%c", c);
        }
        for (int k = 1; k <= (2 * i - 1); k++) {
            if (k == 1 || k == (2 * i - 1)) {
                printf(" ");
            } else {
                printf("%c", c);
            }
        }
        for (int j = 1; j <= (tamanho - i); j++) {
            printf("%c", c);
        }
        printf("%c", c);
        printf("\n");
    }
    for (int j = 0; j < 2 * tamanho + 1; j++) {
        printf("%c", c);
    }
    printf("\n");
}


// Função que verifica ASCII (entre '!' e '~')
int caractere_imprimivel(char c) {
   return c >= 33 && c <= 126;
}

int main() {
    char opcao, caractere;
    int tamanho;
    while (1) {
        printf("Escolha uma figura de 'a' a 'h' (ou 'z' para sair): ");
        scanf(" %c", &opcao);
        if (opcao == 'z') {
            printf("Encerrando o programa.\n");
            break;
        }
        while (opcao < 'a' || opcao > 'h') {
            printf("Opcao invalida! Tente novamente.\n");
            printf("Escolha uma figura de 'a' a 'h' (ou 'z' para sair): ");
            scanf(" %c", &opcao);
            if (opcao == 'z') {
                printf("Encerrando o programa.\n");
                return 0;
            }
        }
        printf("Escolha um caractere: ");
        scanf(" %c", &caractere);
        while (!caractere_imprimivel(caractere)) {
            printf("Caractere invalido! Tente novamente.\n");
            printf("Escolha um caractere imprimivel: ");
            scanf(" %c", &caractere);
        }
        printf("Escolha o tamanho: ");
        scanf("%d", &tamanho);
        while (tamanho <= 0) {
            printf("Tamanho invalido! Tente novamente.\n");
            printf("Escolha o tamanho: ");
            scanf("%d", &tamanho);
        }
        switch (opcao) {
            case 'a':
                desenha_figura_a(caractere, tamanho);
                break;
            case 'b':
                desenha_figura_b(caractere, tamanho);
                break;
            case 'c':
                desenha_figura_c(caractere, tamanho);
                break;
            case 'd':
                desenha_figura_d(caractere, tamanho);
                break;
            case 'e':
                desenha_figura_e(caractere, tamanho);
                break;
            case 'f':
                desenha_figura_f(caractere, tamanho);
                break;
            case 'g':
                desenha_figura_g(caractere, tamanho);
                break;
            case 'h':
                desenha_figura_h(caractere, tamanho);
                break;
        }
        printf("\n");
    }
    return 0;
}