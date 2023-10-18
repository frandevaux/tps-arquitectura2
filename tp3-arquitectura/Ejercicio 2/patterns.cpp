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



void multiProcessPM(string patterns[], string text, int start, int end, vector<tuple<int, int>> globalResults)
{
    vector<tuple<int, int>> patternResults;
    for (int i = start; i < end; i++)
    {
        int patternMatches = patternMatching(patterns[i], text);
        globalResults.push_back(make_tuple(i, patternMatches));
        MPI_Barrier(MPI_COMM_WORLD);
    }

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
    else {
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
    else {
        cout << "Unable to open file" << endl;
        return 0;
    }

    vector<tuple<int, int>> globalResults(32);
    multiProcessPM(patterns, textLine, rank * fraction_number, (rank + 1) * fraction_number, globalResults);


    if (rank == 0)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < 32; i++)
        {
            cout << "El patrón " << patterns[i] << " aparece " << get<1>(globalResults[i]) << " veces" << endl;
        }
    }
    // Close MPI
    if(MPI_SUCCESS != MPI_Finalize())
    {
        cout << "Error finalizing MPI" << endl;
        exit(1);
    }


    return 0;

}
