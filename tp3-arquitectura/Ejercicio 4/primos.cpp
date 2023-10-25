#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <algorithm>
#include <sys/time.h>
#include <mpi.h>

using namespace std;

bool esPrimo(int numero)
{
    if (numero <= 1)
    {
        return false;
    }
    for (int i = 2; i * i <= numero; i++)
    {
        if (numero % i == 0)
        {
            return false;
        }
    }
    return true;
}

// Función para generar los primeros n números primos
vector<int> generarPrimos(int n)
{
    vector<int> primos;
    int numero = 2; // Empezamos con el primer número primo

    while (numero <= n)
    {
        if (esPrimo(numero))
        {
            primos.push_back(numero);
        }
        numero++;
    }

    return primos;
}

bool isPrimeMT(int i, vector<int> primosRaizN)
{
    for (int j = 0; j < primosRaizN.size(); j++)
    {

        if (i % primosRaizN[j] == 0)
        {

            return false;
        }
    }
    return true;
}

vector<int> calculatePrimesMP(int start, int end, vector<int> primosRaizN)
{
    vector<int> local_vector;

    for (int i = start; i < end; i++)
    {
        if (isPrimeMT(i, primosRaizN))
        {
            local_vector.push_back(i);
        }
    }
    return local_vector;
}

int main()
{

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

    const int n = 10000000;
    const int raizN = ceil(sqrt(n));
    int fraction_number = (n - raizN) / (size - 1);

    vector<int> primosTotal;
    vector<int> primosLocal;
    int local_primes_number = 0;
    int total_primes_number = 0;
    int globalCorrect = 0;
    int localCorrect = 0;

    if (rank == 0)
    {
        vector<int> primosRaizN = generarPrimos(raizN);
        // print rank and primosRaizN.size
        cout << "Proceso " << rank << " calculó " << primosRaizN.size() << " primos" << endl;

        // send to all processes primosRaizN
        primosLocal = primosRaizN;

        // send size of primosRaizN to all processes
        int primosRaizN_size = primosRaizN.size();
        local_primes_number = primosRaizN_size;

        for (int i = 1; i < size; i++)
        {
            MPI_Send(&primosRaizN_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // send primosRaizN to all processes

        for (int i = 1; i < size; i++)
        {
            MPI_Send(primosRaizN.data(), primosRaizN_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        vector<int> primosRaizN;
        int primosRaizN_size = 0;
        int correct = 0;

        // Recibe el tamaño de primosRaizN
        MPI_Recv(&primosRaizN_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Redimensiona el vector primosRaizN
        primosRaizN.resize(primosRaizN_size);

        // Recibe los datos en primosRaizN
        MPI_Recv(primosRaizN.data(), primosRaizN_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // calculate primes
        int start = raizN + (rank - 1) * fraction_number;
        int end = raizN + rank * fraction_number;
        primosLocal = calculatePrimesMP(start, end, primosRaizN);
        local_primes_number = primosLocal.size();
        // print
        cout << "Proceso " << rank << " calculó " << local_primes_number << " primos" << endl;
    }

    // send local primes number to root
    MPI_Reduce(&local_primes_number, &total_primes_number, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    // print total primes number

    vector<int> sizes(size);
    MPI_Gather(&local_primes_number, 1, MPI_INT, sizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the displacement for each process in the receive buffer
    vector<int> displs(size);
    displs[0] = 0;
    for (int i = 1; i < size; i++)
    {
        displs[i] = displs[i - 1] + sizes[i - 1];
    }

    // Calculate the total size of the global vector
    int total_size = displs[size - 1] + sizes[size - 1];

    // resize the global vector
    primosTotal.resize(total_size);

    // Gather the local vectors to the root process
    MPI_Gatherv(primosLocal.data(), local_primes_number, MPI_INT, primosTotal.data(), sizes.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
    // order the global vector
    sort(primosTotal.begin(), primosTotal.end());
    if (rank == 0)
    {
        cout << "Total de primos: " << total_primes_number << endl;
        // print last 10 primes
        cout << "Últimos 10 primos: ";
        for (int i = total_size - 10; i < total_size; i++)
        {
            cout << primosTotal[i] << " ";
        }
        // print execution time
        gettimeofday(&end, NULL);
        cout << "Tiempo ejecución: " << double(end.tv_sec - start.tv_sec) + double(end.tv_usec - start.tv_usec) / 1000000 << endl;
    }

    if (MPI_SUCCESS != MPI_Finalize())
    {
        cout << "Error finalizing MPI" << endl;
        exit(1);
    }

    return 0;
}
