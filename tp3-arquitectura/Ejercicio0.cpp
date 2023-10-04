
#include <iostream>
#include <unistd.h>
#include "direccion_IP.cpp"
using namespace std;

int main (){
    pid_t proceso_id = getpid();
    char ip[20];
    obtener_IP(ip);
    char hostname[256];

    gethostname(hostname, sizeof(hostname));
    cout << "Hola Mundo! soy el proceso " << proceso_id << " de <TOTAL_PROCESOS> corriendo en la máquina "<< hostname << " IP= "<< ip << endl;
    return 0;
}