#include <stdio.h>

int calculo_imc(int p, int a){
    return p / (a * a);
}

int main (){
    float peso, altura;
    float imc;
    printf("Digite a altura(em cm): ");
    scanf("%f", &altura);
    printf("Digite o peso(em kg): ");
    scanf("%f", &peso);
    imc = calculo_imc(peso, altura);
    printf("O imc eh: %f.2", imc);
}