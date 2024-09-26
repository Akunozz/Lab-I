#include <stdio.h>

int calculo_imc(int p, int a){
    return 10000 * p / (a * a);
}

int main (){
    int peso, altura;
    int imc;
    printf("Digite a altura(em cm): ");
    scanf("%d", &altura);
    printf("Digite o peso(em kg): ");
    scanf("%d", &peso);
    imc = calculo_imc(peso, altura);
    printf("O imc eh: %d.%d\n", imc/10, imc%10);
}