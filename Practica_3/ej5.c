#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{

    int num_proc = atoi(argv[1]);
    key_t key = ftok(".", 'b');
    size_t size_int = 4;

    int id = shmget(key, size_int, IPC_CREAT | 0666); // creamos o accedemos a la dirección de memoria
    int *memoria_compartida = (int *)shmat(id, NULL, 0);

    while (1)
    {
        printf("Caminando por mi habitación\n");
        getchar();
        printf("Intentando entrar en mi sección crítica...\n");
        if (num_proc == *memoria_compartida) // puerta abierta
        {
            printf("Dentro de mi Sección Crítica\n");

            getchar();
            printf("He accioando el pulsador\n");
            if (*memoria_compartida == 0)
            {
                *memoria_compartida = 1;
            }
            else
            {
                *memoria_compartida = 0;
            }
            continue;
        }
        else
        {
            printf("Puerta cerrada\n");

            continue;
        }
    }

    return 0;
}