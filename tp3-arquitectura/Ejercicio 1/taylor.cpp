#include <iostream>
#include <cmath>
#include <sys/time.h>
#include <thread>
#include <mpi.h>

using namespace std;

long double calculate_single_term(int n, int x)
{

    double div = 1.0 / (2.0 * n + 1.0);
    double pow_p1 = (x - 1.0) / (x + 1.0);
    double pow_p2 = 2.0 * n + 1.0;

    double term = div * pow(pow_p1, pow_p2);

    return term;
}

long double calculate_term_process(int x, int start, int end)
{
    long double process_total = 0.0;
    for (int i = start; i < end; i++)
    {
        process_total += calculate_single_term(i, x);
    }

    return process_total;
}

int main()
{
    int x = 1500000;

    if (MPI_Init(NULL, NULL) != MPI_SUCCESS)
    {
        cout << "Error initializing MPI" << endl;
        return 1;
    }

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // start timer
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Start Taylor
    cout << "\n Ejecutando Taylor con " << size << " procesos" << endl;

    // Init vars
    int fraction_number = 10000000 / size;
    long double global_result = 0.0;

    // Calculate
    long double local_result = calculate_term_process(x, rank * fraction_number, (rank + 1) * fraction_number);

    // Sum all results
    MPI_Reduce(&local_result, &global_result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print Result and execution time
    if (rank == 0)
    {
        gettimeofday(&end, NULL);
        cout << "El resultado es:" << 2.0 * global_result << endl;
        cout << "Tiempo ejecución: " << double(end.tv_sec - start.tv_sec) + double(end.tv_usec - start.tv_usec) / 1000000 << endl;
    }

    // Close MPI
    if (MPI_SUCCESS != MPI_Finalize())
    {
        cout << "Error finalizing MPI" << endl;
        exit(1);
    }

    return 0;
}
