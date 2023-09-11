#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <algorithm>

using namespace std;

const int n = 10000000;
const int raizN = ceil(sqrt(n));
const int t = 10;
vector<int> primosRaizN;
vector<int> primosTotal[t+1];

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

void singleThread()
{
    vector<int> primerosPrimos = generarPrimos(n);

    for (int i = primerosPrimos.size() - 1; i >= primerosPrimos.size() - 5; i--)
    {
        cout << primerosPrimos[i] << endl;
    }

    cout << primerosPrimos.size() << endl;
}

bool isPrimeMT(int i)
{
    for (int numero : primosRaizN)
    {
        if (i % numero == 0)
        {
            return false;
        }
    }
    return true;
}

void calculatePrimesMT(int tNumber,int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if (isPrimeMT(i))
        {
            primosTotal[tNumber].push_back(i);
        }
    }
}

void multiThread()
{
    primosRaizN = generarPrimos(raizN);

    for (int i = 0; i < primosRaizN.size(); i++)
    {
        primosTotal[0].push_back(primosRaizN[i]);
    }

    int fraction = (n - raizN) / t;

    thread thread_array[t];

    for (int i = 0; i < t; i++)
    {
        int start = raizN + i * fraction;
        int end = raizN + (i + 1) * fraction;
        thread_array[i] = thread(calculatePrimesMT, i+1, start, end);
    }

    for (int i = 0; i < t; i++)
    {
        if (thread_array[i].joinable() == true)
        {
            thread_array[i].join();
        }
    }

    // Get All Prime appeding from all threads
    vector<int> allPrimeNumbers;
    for (int i = 0; i < t+1; i++)
    {
        for (int j = 0; j < primosTotal[i].size(); j++)
        {
            allPrimeNumbers.push_back(primosTotal[i][j]);
        }
    }

    // sort primosTotal
    sort(allPrimeNumbers.begin(), allPrimeNumbers.end());

    for (int i = allPrimeNumbers.size() - 1; i >= allPrimeNumbers.size() - 5; i--)
    {
        cout << allPrimeNumbers[i] << endl;
    }

    cout << "Número total: " <<allPrimeNumbers.size() << endl;
}

int main()
{
    multiThread();

    return 0;
}
