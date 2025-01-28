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
    float razao[MAXTAM];     // razao entre o beneficio e o peso
} tipoMochila;

// Transforma o vetor "Vet" em uma representacao binaria do valor "numero"
// EX: 5 => [1,0,1]
void contarBinario(int numero, int n, int Vet[]){
    for (int i = n - 1; i >= 0; i--)
    {
        int bit = (numero / (int)pow(2, i)) % 2; // 1/4 = 0 0 mod 2 = 0  0 mod 2
        // printf("%d ---\n", bit);
        Vet[n - 1 - i] = bit; // pego n -1 - i ; por exemplo de n=3  i= 2   => 3-1-2 = 0  , para contar em ordem crescente
    }
}

// calcula o peso de um subconjunto e retorna o peso total
float calcula_peso(tipoMochila mochila, int vetor[]){
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
float calcula_beneficio(tipoMochila mochila, int vetor[]){
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

// Traz a mochila de um arquivo, e guarda na struct que e usada de retorno
tipoMochila criar_mochila(char *StrEntrada){
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
            /* Le o restante da linha, ate chegar no final de linha '\n' */
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
void imprimirVet(int Vet[], int n){
    printf("\n");
    printf("####Vetor##### \n");

    for (int i = 0; i < n; i++)
    {
        printf("%d", Vet[i]);
    }
    printf("\n");
}

// Printa todos os itens e detalhes da mochila
void imprimirMochila(tipoMochila mochila){
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
void imprimirPotencia(tipoMochila mochila){
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
        char subString[mochila.N + 1]; // adiciona mais um pra ter o \0 q strings precisam
        for (int j = 0; j < mochila.N; j++)
        {
            subString[j] = subconjunto[j] ? '1' : '0'; // se tiver um 1, troca pelo caractere '1', se n, coloca '0'
        }
        subString[mochila.N] = '\0'; // coloca o final da string

        // agora printa a linha corretamente
        printf("\n | %-40s | %-18g | %-18g |", subString, calcula_peso(mochila, subconjunto), calcula_beneficio(mochila, subconjunto));
    }

    // printa o fim
    printf("\n ======================================================================================\n");
}

// Printa um subconjunto, mostrando o mapa de bits, o beneficio e o peso total dele.
void imprimirSubconjunto(tipoMochila mochila, int vetor[]){
    printf("\n ======================================================================================");
    printf("\n | Mapa de bits: ");
    for (int i = 0; i < mochila.N; i++){
        printf("%d ",vetor[i]);
    }
    printf("\n | Peso total:      %g", calcula_peso(mochila, vetor));
    printf("\n | Beneficio total: %g", calcula_beneficio(mochila, vetor));
    printf("\n ======================================================================================");
}

// Metodo de enumeracao
void enumeracao(tipoMochila mochila, int vetOtimo[]){
    vetOtimo[0];
    int vet_auxiliar[mochila.N];

    for (int i = 0; i < pow(2, mochila.N); i++){
        contarBinario(i, mochila.N, vet_auxiliar);

        if ((calcula_peso(mochila, vet_auxiliar) <= mochila.K) && (calcula_beneficio(mochila, vet_auxiliar) > calcula_beneficio(mochila, vetOtimo))){
            for (int j = 0; j < mochila.N; j++){
                vetOtimo[j] = vet_auxiliar[j];
            }
        }
    }
}

// Ordena os itens baseado nas suas razoes, e retorna seus indices em um vetor
void ordenaIndicesRazoes(tipoMochila mochila, int vetorIndices[]){
    //comeca zerado
    for (int i = 0; i < mochila.N; i++){
        vetorIndices[i] = 0;
    }

    // vetor para controlar se aquele item ja foi selecionado
    int selecionado[mochila.N];
    //comeca zerado
    for (int i = 0; i < mochila.N; i++){
        selecionado[i] = 0;
    }

    //contador de itens, pra saber quando ja ordenou todos.
    int cont = 0;

    do{
        // maior razao encontrada, e o indice dessa maior razao
        float maiorRazao = -1;
        int indiceMaiorRazao = -1;

        // percorre os itens e encontra o indice com maior razao q n foi selecionado
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

        // atribui o indice no vetor dos indices, e adiciona um no contador
        vetorIndices[cont] = indiceMaiorRazao;
        cont++;
    } while (cont < mochila.N);
}

//Metodo de heuristica
void heuristica(tipoMochila mochila, int vetOtimo[]){
    // vetor que vai guardar os indices dos itens ordenado pela razao
    int vetorIndices[mochila.N];
    ordenaIndicesRazoes(mochila, vetorIndices);

    // Acessando cada indice e somando beneficio e peso
    float somadorBeneficio = 0;
    float somadorPeso = 0;

    // Zerando vetor para evitar lixo de memoria
    for (int i = 0; i < mochila.N; i++)
        vetOtimo[i] = 0;

    // Se item couber na mochila, coloque-o
    for (int i = 0; i < mochila.N; i++){
        if (mochila.K >= (somadorPeso + mochila.peso[vetorIndices[i]]))
        {
            vetOtimo[vetorIndices[i]] = 1;
            somadorPeso += mochila.peso[vetorIndices[i]];
            somadorBeneficio += mochila.beneficio[vetorIndices[i]];
        }
    }

}

int main(int argc, char **argv){
    // ===================================================
    // ARGUMENTOS

    // confere se o num de argumentos ta certo
    if (argc != 4)
    {
        printf("\nErro de Sintaxe\n");
        printf("Usar: ./mochila.bin <metodo> <entrada> <saida>\n\n");
        return 1;
    }

    // se os argumentos estao certos, recebe eles
    char metodo = argv[1][0];   // metodo: E - Enumeracao & H - Heuristica // qlqr outra coisa pra usar os dois
    char *StrEntrada = argv[2]; // nome do arquivo de entrada
    char *StrSaida = argv[3];   // nome do arquivo de saida

    // ===================================================
    // MOCHILA

    tipoMochila Mochila = criar_mochila(StrEntrada); // mochila que vai guardar todos os detalhes
    int vetOtimo[Mochila.N];                         // mapa de bits do melhor subconjunto encontrado

    // ===================================================
    // PRINTS DE TESTE

    // mostra a mochila
    imprimirMochila(Mochila);

    // mostra os conjuntos
    imprimirPotencia(Mochila);

    // ===================================================
    // RESOLUCAO DO PROBLEMA

    if ( metodo == 'E' ){
        enumeracao(Mochila, vetOtimo);
        imprimirSubconjunto(Mochila, vetOtimo);
    } else if ( metodo == 'H' ){
        heuristica(Mochila, vetOtimo);
        imprimirSubconjunto(Mochila, vetOtimo);
    } else {
        enumeracao(Mochila, vetOtimo);
        imprimirSubconjunto(Mochila, vetOtimo);
        heuristica(Mochila, vetOtimo);
        imprimirSubconjunto(Mochila, vetOtimo);
    }

    return 0;
}
