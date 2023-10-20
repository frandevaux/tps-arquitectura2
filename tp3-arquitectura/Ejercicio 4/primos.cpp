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
            // cout << i << " es divisible por " << primosRaizN[j] << endl;
            return false;
        }
    }
    return true;
}

vector<int> calculatePrimesMP(int start, int end, vector<int> primosRaizN)
{
    vector<int> local_vector;
    cout << "Start: " << start << " End: " << end << endl;
    for (int i = start; i < end; i++)
    {
        if (isPrimeMT(i, primosRaizN))
        {
            // print
            cout << "Proceso " << MPI::COMM_WORLD.Get_rank() << " encontró primo: " << i << endl;
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

    const int n = 1000;
    const int raizN = ceil(sqrt(n));
    int fraction_number = (n - raizN) / (size - 1);

    vector<int> primosTotal;
    vector<int> primosLocal;
    int local_size = 0;
    int max_size = 0;
    int globalCorrect = 0;
    int localCorrect = 0;

    if (rank == 0)
    {
        vector<int> primosRaizN = generarPrimos(raizN);
        // send to all processes primosRaizN
        primosLocal = primosRaizN;
        primosTotal.resize(primosRaizN.size());
        // send size of primosRaizN to all processes
        int primosRaizN_size = primosRaizN.size();
        for (int i = 1; i < size; i++)
        {
            MPI_Send(&primosRaizN_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // send primosRaizN to all processes

        for (int i = 1; i < size; i++)
        {
            MPI_Send(primosRaizN.data(), primosRaizN_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // print number of primes
        cout << "Proceso " << rank << " calculó " << primosRaizN_size << " primos" << endl;
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

        cout << "Proceso " << rank << " recibió primosRaizN: ";
        for (int i = 0; i < primosRaizN.size(); i++)
        {
            cout << primosRaizN[i] << " ";
        }
        // calculate primes
        int start = raizN + (rank - 1) * fraction_number;
        int end = raizN + rank * fraction_number;
        primosLocal = calculatePrimesMP(start, end, primosRaizN);
        local_size = primosLocal.size();
        // print
        cout << "Proceso " << rank << " calculó " << local_size << " primos" << endl;
    }

    // calculate the max size from every primosLocal
    MPI_Reduce(&local_size, &max_size, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    // resize primosTotal
    primosTotal.resize(max_size * (size - 1) + primosTotal.size());
    // gather all the primosLocal in primosTotal
    MPI_Gather(&primosLocal, max_size, MPI_INT, &primosTotal, max_size, MPI_INT, 0, MPI_COMM_WORLD);

    // order primosTotal and print size of primosTotal
    if (rank == 0)
    {
        sort(primosTotal.begin(), primosTotal.end());
        cout << "Cantidad de primos: " << primosTotal.size() << endl;
    }

    if (MPI_SUCCESS != MPI_Finalize())
    {
        cout << "Error finalizing MPI" << endl;
        exit(1);
    }

    return 0;
}
