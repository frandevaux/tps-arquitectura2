#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <algorithm>

using namespace std;

const int n = 1000000;
const int raizN = ceil(sqrt(n));
const int t = 2;
vector<int> primosRaizN;
vector<int> primosTotal;

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

void calculatePrimesMT(int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if (isPrimeMT(i))
        {
            primosTotal.push_back(i);
        }
    }
}

void multiThread()
{
    primosRaizN = generarPrimos(raizN);

    for (int i = 0; i < primosRaizN.size(); i++)
    {
        primosTotal.push_back(primosRaizN[i]);
    }

    int fraction = (n - raizN) / t;

    thread thread_array[t];

    for (int i = 0; i < t; i++)
    {
        int start = raizN + i * fraction;
        int end = raizN + (i + 1) * fraction;
        thread_array[i] = thread(calculatePrimesMT, start, end);
    }

    for (int i = 0; i < t; i++)
    {
        if (thread_array[i].joinable() == true)
        {
            thread_array[i].join();
        }
    }

    // sort primosTotal
    sort(primosTotal.begin(), primosTotal.end());

    for (int i = primosTotal.size() - 1; i >= primosTotal.size() - 5; i--)
    {
        cout << primosTotal[i] << endl;
    }

    cout << primosTotal.size() << endl;
}

int main()
{
    multiThread();

    return 0;
}
