#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXTAM 50 // define o maximo de itens 

typedef struct tipoMochila {
    int K;                   // peso maximo da mochila
    int N;                   // numero total de itens no universo
    char nome[MAXTAM][256];  // nome do item
    float peso[MAXTAM];      // peso do item
    float beneficio[MAXTAM]; // beneficio do item
    float razao[MAXTAM];     // razão entre o beneficio e o peso 
} tipoMochila;

//Função de teste para imprimir um vetor 
void imprimirVet(int Vet[], int n) {
    printf("\n");
    printf("####Vetor##### \n");

    for (int i = 0; i < n; i++) {
        printf("%d", Vet[i]);
    }
    printf("\n");

}


//Tranforma um numero x em um vetor binario onde  quantidade de posições é equivalente a quantidade de bits 
// do numero.
void contarBinario(int numero, int n, int* Vet) {
    for (int i = n - 1; i >= 0; i--) {
       int bit = (numero / (int)pow(2, i)) % 2; // 1/4 = 0 0 mod 2 = 0  0 mod 2
        //printf("%d ---\n", bit);
        Vet[n - 1 - i] = bit; //pego n -1 - i ; por exemplo de n=3  i= 2   => 3-1-2 = 0  , para contar em ordem crescente 

    }
    //chama a função que imprime o vetor
    imprimirVet(Vet, n);

}

//calcula o peso de um subconjunto e retorna o peso total 
float calcula_peso(tipoMochila mochila, int vetor[MAXTAM]){
    float peso_total=0;
    for  (int i =0; i<mochila.N; i++){

        if(vetor[i]==1){
            peso_total+= mochila.peso[i];
        }
    }
    
    return peso_total;
}

//calcula o beneficio de um subconjunto e retorna o beneficio total 
float calcula_beneficio(tipoMochila mochila, int vetor[MAXTAM]){
    float beneficio_total=0;
    for  (int i =0; i<mochila.N; i++){

        if(vetor[i]==1){
            beneficio_total+= mochila.beneficio[i];
        }
    }
    return beneficio_total;
}

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
    
    int n;
    //para teste o contador 
    printf("Digite o valor de n para mostrar as representações binárias de 0 até 2^n - 1: ");
    scanf("%d", &n);

    //se eu tiver 3 itens, vou ter 8 combinações possiveis
    int limite = pow(2,n); 

    //exibe todos os números de 0 até 2^n - 1 em binário, ou seja se tiver 4 itens vai pecorrer de 0 a 15
    int sub_conjunto[n];
    printf("\n");
    for (int i = 0; i < limite; i++) {
        
        contarBinario(i,n,sub_conjunto);

        printf("PESO: %.2f \n",calcula_peso(Mochila, sub_conjunto) );
        printf("BENEFICIO: %.2f \n",calcula_beneficio(Mochila, sub_conjunto) );
        printf("\n");
    }
    printf("\n");

    return 0;
}
