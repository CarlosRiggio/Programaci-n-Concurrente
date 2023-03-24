#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{

    int aux = 1;
    int num_proc = atoi(argv[1]);
    key_t key = ftok(".", 'b');
    size_t size_int = 4;

    int id = shmget(key, size_int, IPC_CREAT | 0666); // creamos o accedemos a la dirección de memoria
    int *memoria_compartida = (int *)shmat(id, NULL, 0);

    *memoria_compartida = 2; // uso este valor para la opción en la q ambas puertas esten abiertas
 
    while(1)
    {
        if(aux){
            printf("Caminando por mi habitación\n");
        }
        
        getchar();
        printf("Intentando acceder al pasillo de entrada a la Sección Crítica...\n");
            if (num_proc == *memoria_compartida || *memoria_compartida == 2) // puerta abierta
            {
                printf("Dentro del pasillo\n");
                getchar();
                *memoria_compartida = num_proc;
                printf("He accioando el pulsador\n");
                printf("Dentro de mi Sección Crítica\n");
                getchar();
                printf("He salido de mi Sección Crítica\n");
                getchar();
                printf("He accionado el pulsador\n");
                aux=1;
                *memoria_compartida = 2;

                continue;
            }
            else
            {
                printf("Puerta cerrada\n");
                aux=0;

            }
    }
    return 0;
}