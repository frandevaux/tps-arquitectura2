#include <iostream>
#include <cmath>
#include <sys/time.h>
#include <thread>

using namespace std;
long double result_multithread;

long double calculate_single_term(int n, int x)
{

    double div = 1.0 / (2.0 * n + 1.0);
    double pow_p1 = (x - 1.0) / (x + 1.0);
    double pow_p2 = 2.0 * n + 1.0;

    double term = div * pow(pow_p1, pow_p2);

    return term;
    ;
}

void calculate_term_thread(int x, int start, int end)
{
    long double thread_total = 0;
    for (int i = start; i < end; i++)
    {
        thread_total += calculate_single_term(i, x);
    }
    result_multithread += thread_total;
}

long double taylor_single_thread(int x)
{
    long double result_single_thread = 0;
    for (int i = 0; i <= 10000000; i++)
    {
        result_single_thread += calculate_single_term(i, x);
    }
    result_single_thread = 2 * result_single_thread;
    cout << "El resultado es:" << result_single_thread << endl;
    return 1;
};

long double taylor_multithread(int x, int t)
{

    int fraction_number = 10000000 / t;
    thread thread_array[t];
    int start = 0;
    int end = fraction_number;

    for (int i = 0; i < t; i++)
    {
        thread_array[i] = thread(calculate_term_thread, x, i * fraction_number, (i + 1) * fraction_number);
    }

    for (int i = 0; i < t; i++)
    {
        if (thread_array[i].joinable() == true)
        {
            thread_array[i].join();
        }
    }
    result_multithread = 2 * result_multithread;
    cout << "El resultado es:" << result_multithread << endl;
    return 1;
};

int main()
{
    int x;
    cout << "Ingrese un número entero positivo: ";
    cin >> x;
    int t;
    cout << "Ingrese la cantidad de hilos deseados: ";
    cin >> t;
    
    timeval time1, time2, time3;
    gettimeofday(&time1, NULL);

    cout << "\n Ejecutando Taylor con un hilo" << endl;
    taylor_single_thread(x);
    gettimeofday(&time2, NULL);
    cout << "Tiempo ejecución: " << double(time2.tv_sec - time1.tv_sec) + double(time2.tv_usec - time1.tv_usec) / 1000000 << endl;

    cout << "\n Ejecutando Taylor con " << t << " hilos" << endl;
    taylor_multithread(x, t);
    gettimeofday(&time3, NULL);
    cout << "Tiempo ejecución: " << double(time3.tv_sec - time2.tv_sec) + double(time3.tv_usec - time2.tv_usec) / 1000000 << endl;

    return 0;
}
