#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sys/time.h>
#include <mpi.h>
#include <vector>
#include <tuple>

using namespace std;

int patternMatching(string pattern, string text)
{

    int n = text.length();
    int m = pattern.length();
    int count = 0;

    for (int i = 0; i <= n - m; i++)
    {
        int j;

        // Compara el patrón con el text en la posición actual
        for (j = 0; j < m; j++)
        {
            if (text[i + j] != pattern[j])
            {
                break; // Si hay una diferencia, sal del bucle interno
            }
        }
        if (j == m)
        {
            count += 1;
        }
    }

    return count;
}

vector<int> multiProcessPM(string patterns[], string text, int start, int end)
{
    vector<int> patternResults;
    for (int i = start; i < end; i++)
    {
        int patternMatches = patternMatching(patterns[i], text);
        patternResults.push_back(patternMatches);
    }
    return patternResults;
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

    int fraction_number = 32 / size;

    string line;
    ifstream patternFile("./patrones.txt");
    string patterns[32];
    if (patternFile.is_open())
    {
        int i = 0;
        while (getline(patternFile, line))
        {
            patterns[i] = line;
            i += 1;
        }
        patternFile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
        return 0;
    }

    string textLine;
    ifstream textFile("./texto.txt");

    if (textFile.is_open())
    {
        getline(textFile, textLine);
        textFile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
        return 0;
    }

    int localPattern = patternMatching(patterns[rank], textLine);

    // Send
    MPI_Send(&localPattern, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    // Receive
    if (rank == 0)
    {
        int Mpresult;
        for (int i = 0; i < size; i++)
        {

            MPI_Recv(&Mpresult, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cout << "El patron " << i << " se encontró " << Mpresult << " veces." << endl;
        }
    }

    // print execution time
    if (rank == 0)
    {
        gettimeofday(&end, NULL);
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
