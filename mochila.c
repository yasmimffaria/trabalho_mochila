#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXTAM 50 // define o maximo de itens

typedef struct tipoMochila
{
    int K;                   // peso maximo da mochila
    int N;                   // numero total de itens no universo
    char nome[MAXTAM][256];  // nome do item
    float peso[MAXTAM];      // peso do item
    float beneficio[MAXTAM]; // beneficio do item
    float razao[MAXTAM];     // razão entre o beneficio e o peso
} tipoMochila;

// Transforma o vetor "Vet" em uma representacao binaria do valor "numero"
// EX: 5 => [1,0,1]
void contarBinario(int numero, int n, int Vet[])
{
    for (int i = n - 1; i >= 0; i--)
    {
        int bit = (numero / (int)pow(2, i)) % 2; // 1/4 = 0 0 mod 2 = 0  0 mod 2
        // printf("%d ---\n", bit);
        Vet[n - 1 - i] = bit; // pego n -1 - i ; por exemplo de n=3  i= 2   => 3-1-2 = 0  , para contar em ordem crescente
    }
}

// calcula o peso de um subconjunto e retorna o peso total
float calcula_peso(tipoMochila mochila, int vetor[MAXTAM])
{
    float peso_total = 0;
    for (int i = 0; i < mochila.N; i++)
    {
        if (vetor[i] == 1)
        {
            peso_total += mochila.peso[i];
        }
    }
    return peso_total;
}

// calcula o beneficio de um subconjunto e retorna o beneficio total
float calcula_beneficio(tipoMochila mochila, int vetor[MAXTAM])
{
    float beneficio_total = 0;
    for (int i = 0; i < mochila.N; i++)
    {
        if (vetor[i] == 1)
        {
            beneficio_total += mochila.beneficio[i];
        }
    }
    return beneficio_total;
}

// Traz a mochila de um arquivo, e guarda na struct que é usada de retorno
tipoMochila criar_mochila(char *StrEntrada)
{
    // declaracao de variaveis
    char Controle;       // le o caracter e decide o que faz
    char auxFinalLinha;  // auxiliar para ir para próxima linha
    int cont = 0;        // contador de itens
    tipoMochila Mochila; // declaracao da mochila em si

    // arquivo de ponteiro pro txt
    FILE *arquivo = fopen(StrEntrada, "r");

    // testa pra ver se abriu o arquivo corretamente
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    // percorre o arquivo inteiro, olhando cada linha e trazendo ela pra seu devido lugar na memoria
    do
    {
        // olha a primeira letra
        fscanf(arquivo, "%c", &Controle);

        // ve oq tem que fazer dependendo pra letra lida
        switch (Controle)
        {
        // K - Tamanho limite da mochila
        case 'K':
            fscanf(arquivo, "%d", &Mochila.K);
            break;

        // N - Numero de items na lista
        case 'N':
            fscanf(arquivo, "%d", &Mochila.N);
            break;

        // I - Item: Nome Peso Beneficio
        case 'I':
            fscanf(arquivo, "%s %f %f", Mochila.nome[cont], &Mochila.peso[cont], &Mochila.beneficio[cont]);
            Mochila.razao[cont] = Mochila.beneficio[cont] / Mochila.peso[cont]; // calcula a razao
            cont++;
            break;

        // # - Ignorar linha
        case '#':
            /* Lê o restante da linha, até chegar no final de linha '\n' */
            do
            {
                fscanf(arquivo, "%c", &auxFinalLinha);
            } while (auxFinalLinha != '\n');
            break;

        // F - Fim do arquivo.
        case 'F':
            fclose(arquivo);
            break;
        }
    } while (Controle != 'F');

    // se no final, o contador contou um numero de itens diferente do valor que esta no N,
    //  para o programa, pois isso ta errado
    if (cont != Mochila.N)
    {
        printf("Numero de itens incompativel com o valor de N\n");
        exit(1);
    }

    // quando chegou no final, retorna a mochila, agora com todos os dados importados do txt
    return Mochila;
}

// Printa um Vetor
void imprimirVet(int Vet[], int n)
{
    printf("\n");
    printf("####Vetor##### \n");

    for (int i = 0; i < n; i++)
    {
        printf("%d", Vet[i]);
    }
    printf("\n");
}

// Printa todos os itens e detalhes da mochila
void imprimirMochila(tipoMochila mochila)
{
    // printa o header
    printf("\n ======================================================================================");
    printf("\n |                                   M O C H I L A                                    |");
    printf("\n |====================================================================================|");
    printf("\n | Numero de itens (N): %-61d |", mochila.N);
    printf("\n | Limite de peso  (K): %-61d |", mochila.K);
    printf("\n |====================================================================================|");
    printf("\n | ITEM                                  | PESO         | BENEFICIO    | RAZAO        |");
    printf("\n |=======================================|==============|==============|==============|");

    // printa os items em si
    for (int i = 0; i < mochila.N; i++)
    {
        printf("\n | %-37s | %-12g | %-12g | %-12g |", mochila.nome[i], mochila.peso[i], mochila.beneficio[i], mochila.razao[i]);
    }

    // printa o final
    printf("\n ======================================================================================\n");
}

// Printa todos os subconjuntos do conjunto potencia
void imprimirPotencia(tipoMochila mochila)
{
    // cria um vetor pra representar o subconjunto
    int subconjunto[mochila.N];

    // printa o header
    printf("\n ======================================================================================");
    printf("\n |                          C O N J U N T O   P O T E N C I A                         |");
    printf("\n |====================================================================================|");
    printf("\n | MAPA DE BITS                             | PESO TOTAL         | BENEFICIO TOTAL    |");
    printf("\n |==========================================|====================|====================|");

    // passa por todas as possiveis combinacoes
    for (int i = 0; i < pow(2, mochila.N); i++)
    {
        // atualiza o subconjunto
        contarBinario(i, mochila.N, subconjunto);

        // cria uma versao do mapa de bits que e na vdd uma string
        //  assim, e mais facil de printar
        int tam = sizeof(subconjunto) / sizeof(subconjunto[0]);
        char subString[tam + 1]; // adiciona mais um pra ter o \0 q strings precisam
        for (int j = 0; j < tam; j++)
        {
            subString[j] = subconjunto[j] ? '1' : '0'; // se tiver um 1, troca pelo caractere '1', se n, coloca '0'
        }
        subString[tam] = '\0'; // coloca o final da string

        // agora printa a linha corretamente
        printf("\n | %-40s | %-18g | %-18g |", subString, calcula_peso(mochila, subconjunto), calcula_beneficio(mochila, subconjunto));
    }

    // printa o fim
    printf("\n ======================================================================================\n");
}

void enumeracao(tipoMochila mochila, int vetOtimo[], float *peso_otimo, float *beneficio_otimo)
{
    *beneficio_otimo = 0;
    *peso_otimo = 0;
    vetOtimo[0];
    int vet_auxiliar[mochila.N];

    for (int i = 0; i < pow(2, mochila.N); i++)
    {
        contarBinario(i, mochila.N, vet_auxiliar);

        if (calcula_peso(mochila, vet_auxiliar) <= mochila.K)
        {
            if (calcula_beneficio(mochila, vet_auxiliar) > *beneficio_otimo)
            {
                for (int j = 0; j < mochila.N; j++)
                {
                    vetOtimo[j] = vet_auxiliar[j];
                }

                *beneficio_otimo = calcula_beneficio(mochila, vet_auxiliar);
                *peso_otimo = calcula_peso(mochila, vet_auxiliar);
            }
        }
    }

    printf("\n");
    printf("Este é o melhor subconjunto pelo metodo da Enumeracao");
    printf("\n");
    for (int i = 0; i < mochila.N; i++)
    {
        printf("%d ", vetOtimo[i]);
    }
    printf("\n");
    printf("Tendo o Peso : %g", *peso_otimo);
    printf("\n");
    printf("Tendo o beneficio :%g ", *beneficio_otimo);
}

float heuristica(tipoMochila mochila, int vetOtimo[])
{
    int vetorIndices[mochila.N];
    // Iniciando vetor com zeros
    for (int i = 0; i < mochila.N; i++)
    {
        vetorIndices[i] = 0;
    }
    int cont = 0;

    // vetor para controlar se aquele item ja foi selecionado
    int selecionado[mochila.N];
    for (int i = 0; i < mochila.N; i++)
    {
        selecionado[i] = 0;
    }

    do
    {
        int indiceMaiorRazao = -1;
        float maiorRazao = -1;

        // encontra o indice com maior razao q n foi selecionado
        for (int i = 0; i < mochila.N; i++)
        {
            if (!selecionado[i] && mochila.razao[i] > maiorRazao)
            {
                maiorRazao = mochila.razao[i];
                indiceMaiorRazao = i;
            }
        }

        // coloca o indice encontrado como selecionado no vetor
        selecionado[indiceMaiorRazao] = 1;

        vetorIndices[cont] = indiceMaiorRazao;
        cont++;
    } while (cont < mochila.N);

    // Acessando cada indice e somando beneficio e peso
    float somadorBeneficio = 0;
    float somadorPeso = 0;
    int vetorHeuristica[mochila.N]; // Vetor para os itens da heuristica

    // Zerando vetor para evitar lixo de memoria
    for (int i = 0; i < mochila.N; i++)
        vetorHeuristica[i] = 0;

    // Se item couber na mochila, coloque-o
    for (int i = 0; i < mochila.N; i++)
    {
        if (mochila.K >= (somadorPeso + mochila.peso[vetorIndices[i]]))
        {
            vetorHeuristica[vetorIndices[i]] = 1;
            somadorPeso += mochila.peso[vetorIndices[i]];
            somadorBeneficio += mochila.beneficio[vetorIndices[i]];
        }
    }

    printf("\nEste e o melhor subconjunto pelo metodo da heuristica:\n");
    for (int j = 0; j < mochila.N; j++)
    {
        printf("%d ", vetorHeuristica[j]);
    }

    printf("\nPeso: %.0f", calcula_peso(mochila, vetorHeuristica));
    printf("\nBeneficio: %.1f", calcula_beneficio(mochila, vetorHeuristica));
    return calcula_beneficio(mochila, vetorHeuristica);
}

int main(int argc, char **argv)
{
    // definição de variaveis
    char *StrEntrada;
    tipoMochila Mochila;

    // confere se o num de argumentos ta certo
    if (argc != 2)
    {
        printf("\nErro de Sintaxe\n");
        printf("Usar: ./solver-mochila.bin <entrada>\n\n");
        return 1;
    }

    // se os argumentos estao certo, recebe eles
    StrEntrada = argv[1];

    // roda a funcao que pega a mochila do arquivo e coloca na variavel Mochila
    Mochila = criar_mochila(StrEntrada);

    //=======TESTE=======

    // mostra a mochila
    imprimirMochila(Mochila);

    // mostra os conjuntos
    //imprimirPotencia(Mochila);

    int vetOtimo[Mochila.N];
    float peso_otimo;
    float beneficio_otimo;
    enumeracao(Mochila, vetOtimo, &peso_otimo, &beneficio_otimo);

    printf("\n\n");

    // Retorna o beneficio da heuristica
    float beneficioHeuristica;
    beneficioHeuristica = heuristica(Mochila, vetOtimo);

    // Gap
    printf("\n\nGap(%%) = (%.1f - %.1f)/%.1f = %.0f%%", beneficio_otimo, beneficioHeuristica, beneficio_otimo, ((beneficio_otimo - beneficioHeuristica) / beneficio_otimo) * 100);

    //=====TESTE-FIM=====
    return 0;
}
