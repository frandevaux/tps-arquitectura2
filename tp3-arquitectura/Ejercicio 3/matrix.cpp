#include <iostream>
#include <vector>
#include <thread>
#include <mpi.h>
#include <sys/time.h>

using namespace std;

const int n = 40;
const int t = 10;



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



vector<vector<float>> calculateRows(int start, int end, vector<vector<float>> matrixA, vector<vector<float>> matrixB) {
    int n = matrixA.size();
    vector<vector<float>> matrixC(end - start, vector<float>(n, 0.0));

    for (int i = start; i < end; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                matrixC[i - start][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    return matrixC;
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

int main()
{
    // init matrices with 0.1 and 0.2
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


    // cout << "Tamaño de las matrices: " << n << "x" << n << endl;

    // timeval time1, time2, time3;
    // gettimeofday(&time1, NULL);

    // cout << "\nCalculando multiplicación de matrices con un hilo" << endl;
    // singleThread(matrixA, matrixB);
    // gettimeofday(&time2, NULL);
    // cout << "Tiempo ejecución: " << double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec - time1.tv_usec) / 1000000 << endl;


    // Star MPI
    if (MPI_Init(NULL, NULL) != MPI_SUCCESS)
    {
        cout << "Error initializing MPI" << endl;
        return 1;
    }

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int fraction_number = n / size;
    
    // Solve rows
    int i = rank;

    vector<vector<float>> matrixC =  calculateRows(i * fraction_number, (i + 1) * fraction_number, matrixA, matrixB);
    // printResult(matrixC);

    // Initialize global matrix with the same dimensions as matrixC
    float* global_matrix = new float[n*n];

    // Gather
    MPI_Gather(&matrixC[0][0], fraction_number*n, MPI_FLOAT, global_matrix, fraction_number*n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Print result
    if (rank == 0){
        // Convert global matrix to vector
        float sum = 0.0;
        vector<vector<float>> global_matrix_vector(n, vector<float>(n));
        for (int i = 0; i < n*n; i++)
        {
            sum += global_matrix[i];
            global_matrix_vector[i/n][i%n] = global_matrix[i];
        }
        cout << "La suma de los elementos de la matriz es " << sum << endl;
        printResult(global_matrix_vector);
    }

    delete[] global_matrix;

    // Close MPI
    if(MPI_SUCCESS != MPI_Finalize())
    {
        cout << "Error finalizing MPI" << endl;
        exit(1);
    }

    return 0;
}