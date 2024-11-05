#include <stdio.h>
    int main (){
        int a, b ,c, media;
        printf("Digite 3 numeros: ");
        scanf("%d %d %d", &a, &b, &c);

        media = (a + b + c) / 3;
        printf("A media dos 3 numeros eh: %d", media);
        return 0;
    }