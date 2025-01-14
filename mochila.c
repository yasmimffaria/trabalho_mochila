#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct tipoMochila {
    int K;               // peso maximo da mochila
    int N;               // numero total de itens no universo
    char *nome[50];      // nome do item
    float peso[50];      // peso do item
    float beneficio[50]; // beneficio do item
} tipoMochila;

tipoMochila criar_mochila(char *StrEntrada) {
    // Funcao cria uma nova mochila, lendo do arquivo e trazendo todos os dados do txt para a memoria

    // declaracao de variaveis
    char Controle;       // le o caracter e decide o que faz
    char auxFinalLinha;  // auxiliar para ir para próxima linha
    int cont = 0;        // contador de itens
    tipoMochila Mochila; // declaracao da mochila em si

    // arquivo de ponteiro pro txt
    FILE *arquivo = fopen(StrEntrada, "r");

    //testa pra ver se abriu o arquivo corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    //percorre o arquivo inteiro, olhando cada linha e trazendo ela pra seu devido lugar na memoria
    do {
        //olha a primeira letra
        fscanf(arquivo, "%c", &Controle);

        //ve oq tem que fazer dependendo pra letra lida
        switch (Controle) {
            //K - Tamanho limite da mochila
            case 'K':
                fscanf(arquivo, "%d", &Mochila.K);
                break;

            //N - Numero de items na lista
            case 'N':
                fscanf(arquivo, "%d", &Mochila.N);
                break;

            //I - Item: Nome Peso Beneficio
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

            //# - Ignorar linha
            case '#':
                /* Lê o restante da linha, até chegar no final de linha '\n' */
                do {
                    fscanf(arquivo, "%c", &auxFinalLinha);
                } while (auxFinalLinha != '\n');
                break;

            //F - Fim do arquivo.
            case 'F':
                fclose(arquivo);
                break;
        }
    } while (Controle != 'F');

    //quando chegou no final, retorna a mochila, agora com todos os dados importados do txt
    return Mochila;
}

int main(int argc, char **argv) {
    // definição de variaveis
    char* StrEntrada;
    tipoMochila Mochila;

    // confere se o num de argumentos ta certo
    if (argc != 2) {
        printf("\nErro de Sintaxe\n");
        printf("Usar: ./solver-mochila.bin <entrada>\n\n");
        return 1;
    }

    //se os argumentos estao certo, recebe eles
    StrEntrada = argv[1];

    //roda a funcao que cria a mochila e coloca na variavel Mochila
    Mochila = criar_mochila(StrEntrada);

    //para teste
    //printa todos os itens da mochila
    printf("K = %d\n", Mochila.K);
    printf("N = %d\n", Mochila.N);
    for (int i = 0; i < Mochila.N; i++) {
        printf("Item: %s P: %.1f B: %.1f \n", Mochila.nome[i], Mochila.peso[i], Mochila.beneficio[i]);
    }

    //depois que terminou, libera a memoria alocada dos vetores.
    for (int i = 0; i < Mochila.N; i++) {
        free(Mochila.nome[i]);
    }

    return 0;
}