#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

#define TAM_BLOCO 16

using namespace std;
using namespace chrono;

// Mutex eh necessario para nao haver erro de sincronizacao (condicao de corrida). Ateh em uma matriz 2x2 esses erros podem ocorrer
mutex mtx;

// Funcao de multiplicacao de matrizes
// Dessa vez, baseada em blocagem
void multiplicacaoMet1(vector<vector<int>>& matriz1, vector<vector<int>>& matriz2, vector<vector<int>>& matrizFinal, int n){
    for (int i = 0; i < n/2; i+=TAM_BLOCO){
        for (int k=0; k<n ; k+= TAM_BLOCO){   
            for (int j = 0 ; j < n; j+=TAM_BLOCO){      
                for (int iInner = i; iInner<j+TAM_BLOCO; iInner++){     
                    for (int kInner = k ; kInner<k+TAM_BLOCO; kInner++){
                        for (int jInner = j ; jInner<j+TAM_BLOCO ; jInner++)
                            matrizFinal[iInner][jInner] += matriz1[iInner][kInner] * matriz2[kInner][jInner];
                    }
                }
            }
        }
    }
}

// Calcula a segunda metade
void multiplicacaoMet2(vector<vector<int>>& matriz1, vector<vector<int>>& matriz2, vector<vector<int>>& matrizFinal, int n){
    for (int i = n/2; i < n; i+=TAM_BLOCO){
        for (int k=0; k<n ; k+= TAM_BLOCO){   
            for (int j = 0 ; j < n; j+=TAM_BLOCO){      
                for (int iInner = i; iInner<j+TAM_BLOCO; iInner++){     
                    for (int kInner = k ; kInner<k+TAM_BLOCO; kInner++){
                        for (int jInner = j ; jInner<j+TAM_BLOCO ; jInner++)
                            matrizFinal[iInner][jInner] += matriz1[iInner][kInner] * matriz2[kInner][jInner];
                    }
                }
            }
        }
    }
}

void printMatrix(const vector<vector<int>>& matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int n;  //Armazena tamanho da matriz

    do{
        //Input do usuario. Dimensao da matriz quadrada.
        printf("\nTamanho da matriz quadrada: ");
        std::cin >> n;  //Comando c++ para pegar input
        printf("Tamanho escolhido: %d\n", n);
        if(n < TAM_BLOCO) printf("Tamanho tem que ser no minimo %d, pois eh o tamanho dos blocos.", TAM_BLOCO);
    }while(n < TAM_BLOCO);

    // std::vector<std::vector<int>> eh o tipo para formar a matriz dinamicamente
    std::vector<std::vector<int>>matriz1(n, std::vector<int>(n, 5));  //Crio a matriz quadrada 1. totalmente preenchida com 5s
    std::vector<std::vector<int>>matriz2(n, std::vector<int>(n, 7));  //Crio a matriz quadrada 2. totalmente preenchida com 7s
    std::vector<std::vector<int>>matrizFinal(n, std::vector<int>(n, 0));  //Crio a matriz resultado. totalmente preenchida com 0s

    auto inicioTemp = high_resolution_clock::now();  // Inicio da cronometragem

    // Multiplicação das matrizes. Abre as threads, uma para cada metade
    std::thread thread1(multiplicacaoMet1, std::ref(matriz1), std::ref(matriz2), std::ref(matrizFinal), n);
    std::thread thread2(multiplicacaoMet2, std::ref(matriz1), std::ref(matriz2), std::ref(matrizFinal), n);

    // Fechando as threads abertas
    thread1.join();
    thread2.join();

    auto finalTemp = high_resolution_clock::now();  // Final da cronometragem
    auto duracao = duration_cast<milliseconds>(finalTemp - inicioTemp);

    // Mostrar o resultado da multiplicação
    //printMatrix(matrizFinal, n);

    printf("\n\nDuracao (milissegundos): %ld\n", duracao.count());


    return 0;
}

/*                              PARA COMPILAR E EXECUTAR
clear &&
cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/Computação paralela/Labs/Lab3 - MultiplicandoMatrizes/ex2MatrizesBlocagem.cpp" ~/CompParalel/ && \
g++ ~/CompParalel/ex2MatrizesBlocagem.cpp -o ~/CompParalel/ex2MatrizesBlocagem && \
~/CompParalel/ex2MatrizesBlocagem
*/

/*                              PARA COMPILAR E EXECUTAR (SEM OTIMIZACOES)
clear &&
cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/Computação paralela/Labs/Lab3 - MultiplicandoMatrizes/ex2MatrizesBlocagem.cpp" ~/CompParalel/ && \
g++ ~/CompParalel/ex2MatrizesBlocagem.cpp -O0 -fno-tree-vectorize -fno-inline -o ~/CompParalel/ex2MatrizesBlocagem && \
~/CompParalel/ex2MatrizesBlocagem
*/