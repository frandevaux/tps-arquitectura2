#include <iostream>

using namespace std;

const int n = 3000;

// matrixC
float matrixC[n][n];

void calculateCell(int i, int j, float matrixA[n][n], float matrixB[n][n]) {
    float  result = 0;
    for (int k = 0; k < n; k++) {
        result += matrixA[i][k] * matrixB[k][j];
    } 
    matrixC[i][j] = result;
}

void singleThread(float matrixA[n][n], float matrixB[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            calculateCell(i, j, matrixA, matrixB);
        }
    }
} 

int main() {
    // init matrices randomly
    float matrixA[n][n];
    float matrixB[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; i < n; i++) {
            matrixA[i][j] = rand() % 100;
            matrixB[i][j] = rand() % 100;
        }
    }

    return 0;
}