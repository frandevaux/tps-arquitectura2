#include <iostream>
#include <vector>
#include <thread>

using namespace std;

const int n = 300;
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

void multiThread(const vector<vector<float>> &matrixA, const vector<vector<float>> &matrixB)
{
    // TODO
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

    cout << "Matrix A: " << endl;

    cout << " " << endl;

    // imprimirMatriz(matrixA);

    cout << " " << endl;

    cout << "Matrix B: " << endl;

    cout << " " << endl;
    // imprimirMatriz(matrixB);

    cout << " " << endl;
    // singleThread(matrixA, matrixB);
    multiThread(matrixA, matrixB);

    cout << "Matrix C: " << endl;

    // imprimirMatriz(matrixC);
    cout << " " << endl;
    cout << matrixC[0][0] << endl;
    cout << " " << endl;

    // calculate total of matrixC
    float total = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            total += matrixC[i][j];
        }
    }

    cout << total << endl;
    return 0;
}