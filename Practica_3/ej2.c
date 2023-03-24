#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    key_t key = ftok(".", 'a');
    size_t size_int = 4;
    int id = shmget(key,size_int,0666); // accedemos a la memoria q creamos en el ej1
    int *memoria_compartida = (int *)shmat(id, NULL, 0);// valor q está actualmente en la memoria.
    printf("Argumento en la zona de memoria compartida: %i\n", *memoria_compartida);

    return 0;
}