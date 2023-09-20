#include <iostream>
#include <vector>
#include <thread>
#include <sys/time.h>

using namespace std;

const int n = 1000;
const int t = 10;

// matrixC
vector<vector<float>> matrixC(n, vector<float>(n));

void imprimirMatriz(const vector<vector<float>> &matriz)
{
    for (const vector<float> &fila : matriz)
    {
        for (float elemento : fila)
        {
            cout << elemento << " ";
        }
        cout << endl; // Saltar a la siguiente línea para la siguiente fila
    }
}

void calculateCell(int i, int j, const vector<vector<float>> &matrixA, const vector<vector<float>> &matrixB)
{
    float result = 0;
    for (int k = 0; k < n; k++)
    {
        result += matrixA[i][k] * matrixB[k][j];
    }
    matrixC[i][j] = result;
}

void calculateRows(int start, int end, const vector<vector<float>> &matrixA, const vector<vector<float>> &matrixB)
{
    for (int i = start; i < end; i++)
    {
        for (int j = 0; j < n; j++)
        {
            calculateCell(i, j, matrixA, matrixB);
        }
    }
}

void printResult(const vector<vector<float>> &matrix) {
    // print the corners of the matrix
    cout << "|" << matrix [0][0] << "..." << matrix[0][n-1] << "|" << endl;
    cout << "|............|" << endl;
    cout << "|" << matrix [n-1][0] << "..." << matrix[n-1][n-1] << "|" << endl;


    // calculate the total
    float total = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            total += matrix[i][j];
        }
    }
    cout << "La suma de los elementos de la matriz es " <<total << endl;
}

void multiThread(const vector<vector<float>> &matrixA, const vector<vector<float>> &matrixB)
{
    int fraction_number = n / t;
    thread thread_array[t];

    for (int i = 0; i < t; i++)
    {
        thread_array[i] = thread(calculateRows, i * fraction_number, (i + 1) * fraction_number, matrixA, matrixB);
    }
    for (int i = 0; i < t; i++)
    {
        if (thread_array[i].joinable() == true)
        {
            thread_array[i].join();
        }
    }

    printResult(matrixC);
}

void singleThread(const vector<vector<float>> &matrixA, const vector<vector<float>> &matrixB)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            calculateCell(i, j, matrixA, matrixB);
        }
    }

    printResult(matrixC);
}



int main()
{
    // init matrices randomly
    vector<vector<float>> matrixA(n, vector<float>(n));
    vector<vector<float>> matrixB(n, vector<float>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrixA[i][j] = 0.1;
            matrixB[i][j] = 0.2;
        }
    }

    // cout << "Matrix A: " << endl;
    // cout << " " << endl;
    // imprimirMatriz(matrixA);

    // cout << "Matrix B: " << endl;
    // cout << " " << endl;
    // imprimirMatriz(matrixB);

    cout << "Tamaño de las matrices: " << n << "x" << n << endl;

    timeval time1, time2, time3;
    gettimeofday(&time1, NULL);

    cout << "\nCalculando multiplicación de matrices con un hilo" << endl;
    singleThread(matrixA, matrixB);
    gettimeofday(&time2, NULL);
    cout << "Tiempo ejecución: " << double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec - time1.tv_usec) / 1000000 << endl;

    cout << "\nCalculando multiplicación de matrices con 10 hilos" << endl;
    multiThread(matrixA, matrixB);
    gettimeofday(&time3, NULL);
    cout << "Tiempo ejecución: " << double(time3.tv_sec - time2.tv_sec) + double(time3.tv_usec - time2.tv_usec) / 1000000 << endl;


    return 0;
}