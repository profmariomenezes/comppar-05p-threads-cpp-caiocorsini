#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;
using namespace chrono;

// Mutex eh necessario para nao haver erro de sincronizacao (condicao de corrida). Ateh em uma matriz 2x2 esses erros podem ocorrer
mutex mtx;

// Funcao de multiplicacao de matrizes
// vector<vector<int>>& eh para passar como referencia
// Calcula a primeira metade
void multiplicacaoMet1(vector<vector<int>>& matriz1, vector<vector<int>>& matriz2, vector<vector<int>>& matrizFinal, int n){
    for (int j = 0; j < n/2; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                lock_guard<mutex> lock(mtx);  // Interrompe na hora de atualizar o indice
                matrizFinal[j][i] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
}

// Calcula a segunda metade
void multiplicacaoMet2(vector<vector<int>>& matriz1, vector<vector<int>>& matriz2, vector<vector<int>>& matrizFinal, int n){
    for (int j = n/2; j < n; j++) {
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                lock_guard<mutex> lock(mtx);  // Interrompe na hora de atualizar o indice
                matrizFinal[j][i] += matriz1[i][k] * matriz2[k][j];
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

    //Input do usuario. Dimensao da matriz quadrada.
    printf("\nTamanho da matriz quadrada: ");
    std::cin >> n;  //Comando c++ para pegar input
    printf("Tamanho escolhido: %d\n", n);

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
cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/Computação paralela/Labs/Lab3 - MultiplicandoMatrizes/ex1MatrizesColunaLinha.cpp" ~/CompParalel/ && \
g++ ~/CompParalel/ex1MatrizesColunaLinha.cpp -o ~/CompParalel/ex1MatrizesColunaLinha && \
~/CompParalel/ex1MatrizesColunaLinha
*/

/*                              PARA COMPILAR E EXECUTAR (SEM OTIMIZACOES)
clear &&
cp "/mnt/c/Users/caiof/Documentos disco local/aaaComputacao_faculdade/Computação paralela/Labs/Lab3 - MultiplicandoMatrizes/ex1MatrizesColunaLinha.cpp" ~/CompParalel/ && \
g++ ~/CompParalel/ex1MatrizesColunaLinha.cpp -O0 -fno-tree-vectorize -fno-inline -o ~/CompParalel/ex1MatrizesColunaLinha && \
~/CompParalel/ex1MatrizesColunaLinha
*/