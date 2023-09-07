#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sys/time.h>

using namespace std;

int patternResults[32];

void patternMatching(int patternIndex, string pattern, string text)
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

    patternResults[patternIndex] = count;
}

void singleThreadPM(string patterns[], string text)
{
    for (int i = 0; i < 32; i++)
    {
        patternMatching(i, patterns[i], text);
        cout << "El patron " << i << " se encontró " << patternResults[i] << " veces." << endl;
    }
}

void multiThreadPM(string patterns[], string text)
{

    thread thread_array[32];

    for (int i = 0; i < 32; i++)
    {
        thread_array[i] = thread(patternMatching, i, patterns[i], text);
    }

    for (int i = 0; i < 32; i++)
    {
        if (thread_array[i].joinable() == true)
        {
            thread_array[i].join();
        }
    }
    for (int i = 0; i < 32; i++)
    {

        cout << "El patron " << i << " se encontró " << patternResults[i] << " veces." << endl;
    }
}
int main()
{
    string line;
    ifstream patternFile("patrones.txt");
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
        cout << "Unable to open file";

    string textLine;
    ifstream textFile("texto.txt");

    if (textFile.is_open())
    {
        getline(textFile, textLine);
        textFile.close();
    }
    else
        cout << "Unable to open file";

    timeval time1, time2;
    gettimeofday(&time1, NULL);
    multiThreadPM(patterns, textLine);
    gettimeofday(&time2, NULL);
    cout << "Tiempo ejecución: " << double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec - time1.tv_usec) / 1000000 << endl;

    return 0;
}
