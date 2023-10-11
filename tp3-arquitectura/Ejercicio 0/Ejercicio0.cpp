
#include <iostream>
#include <unistd.h>
#include "direccion_IP.cpp"
#include <mpi.h>

using namespace std;

int main (){

    if (MPI_Init(NULL, NULL) != MPI_SUCCESS)
    {
        cout << "Error initializing MPI" << endl;
        return 1;
    }

    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    int proceso_id = rank;
    char ip[20];
    obtener_IP(ip);
    char hostname[256];

    gethostname(hostname, sizeof(hostname));


    cout << "Hola Mundo! soy el proceso " << proceso_id << " de "<< size << " corriendo en la máquina "<< hostname << " IP= "<< ip << endl;

    if(MPI_SUCCESS != MPI_Finalize())
    {
        cout << "Error finalizing MPI" << endl;
        exit(1);
    }



    return 0;
}