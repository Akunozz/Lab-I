  #include <stdio.h>

  int leval()
  {
    // recebe do usuário um número de 3 dígitos, e converte em seu valor
    int valor;
    int codigo;
    int centena, dezena, unidade;
    codigo = getchar();
    centena = codigo - '0';
    codigo = getchar();
    dezena = codigo - '0';
    codigo = getchar();
    getchar();
    unidade = codigo - '0';
    valor = centena * 100 + dezena * 10 + unidade;
    return valor;
  }

  void impval(int val)
  {
    int centena, dezena, unidade;
    unidade = val % 10;
    dezena  = val / 10 % 10;
    centena = val / 100 % 10;
    int codigo;
    codigo = centena + '0';
    putchar(codigo);
    codigo = dezena  + '0';
    putchar(codigo);
    codigo = unidade + '0';
    putchar(codigo);
  }

  int main()
  {
    int v1, dobro, quadrado;
    v1 = leval();

    dobro = v1 + v1;
    quadrado = v1 * v1;
    impval(dobro);
    impval(quadrado);
  }