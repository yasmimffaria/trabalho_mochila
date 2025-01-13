#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct tipoMochila {
  int K; // peso máximo da mochila
  int N; // número total de itens no universo
  char *nome[50]; // nome do item
  float peso[50]; // peso do item
  float beneficio[50]; // benefício do item
} tipoMochila;

tipoMochila criar_mochila(char *StrEntrada) {
    char Controle; // lê o caracter e decide o que faz
    char auxFinalLinha;  // auxiliar para ir para próxima linha
    int cont = 0; // contador de itens

    tipoMochila Mochila;
    FILE *arquivo = fopen(StrEntrada, "r");

    if (arquivo == NULL) {
      printf("Erro ao abrir arquivo\n");
      exit(1);
    }

    do {
        fscanf(arquivo, "%c", &Controle);
        switch (Controle) {
          case 'K':
              fscanf(arquivo, "%d", &Mochila.K);
              break;
          case 'N':
              fscanf(arquivo, "%d", &Mochila.N);
              break;
          case 'I':
              // Aloca memória para a string do nome
              Mochila.nome[cont] = malloc(50 * sizeof(char));
              if (Mochila.nome[cont] == NULL) {
                  printf("Erro de memória\n");
                  exit(1);
              }
              fscanf(arquivo, "%s %f %f", Mochila.nome[cont], &Mochila.peso[cont], &Mochila.beneficio[cont]);
              cont++;
              break;
          case '#':
                /* Lê o restante da linha, até chegar no final de linha '\n' */
                do {
                    fscanf(arquivo, "%c", &auxFinalLinha);
                } while (auxFinalLinha != '\n');
                break;
          case 'F':
              fclose(arquivo);
              break;

        }
    } while (Controle != 'F');

    return Mochila;
}

int main(int argc, char **argv) {
    char* StrEntrada;
    tipoMochila Mochila;

    /* Testa se a quantidade de parâmetros informada está correta */
    if (argc != 2) {
        printf("\nErro de Sintaxe\n");
        printf("Usar: ./solver-mochila.bin <entrada>\n\n");
        return 1;
    }

    /* Obtém os parâmetros informados */
    StrEntrada = argv[1];
    Mochila = criar_mochila(StrEntrada);

    printf("K = %d\n", Mochila.K);
    printf("N = %d\n", Mochila.N);
    for (int i = 0; i < Mochila.N; i++) {
        printf("Item: %s P: %f B: %f \n", Mochila.nome[i], Mochila.peso[i], Mochila.beneficio[i]);
        free(Mochila.nome[i]); // Libera a memória alocada para o nome
    }

    return 0;
}
