#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaforo_general;
sem_t semaforo_leer[10];
sem_t semaforo_salir;

void *hilo(void *arg)
{
    int lector = *(int *)arg;
    while (1)
    {
        printf("[Lector %i] -> Esperando a intentar leer...\n", lector + 1);
        sem_wait(&semaforo_leer[lector]);
        printf("[Lector %i] -> Intentando leer...\n", lector + 1);
        sem_wait(&semaforo_general);
        printf("[Lector %i] -> Leyendo...\n", lector + 1); // sección crítica
        sem_wait(&semaforo_salir);
        printf("[Lector %i] -> Fin lectura\n", lector + 1);
        sem_post(&semaforo_general);
        pthread_exit(NULL);
    }
}

int main(int argc, char *argv[])
{
    int i;
    int N1 = atoi(argv[1]);
    int N2 = atoi(argv[2]);
    pthread_t numero_de_hilos[N1];
    int lectores[N1];
    int salir_del_bucle = 0;

    if (N1 < N2)
    {
        printf("N2 debe ser menor que N1\n");
        return 0;
    }

    sem_init(&semaforo_general, 0, N2);

    for (i = 0; i < N1; i++)
    {
        lectores[i] = i;
        pthread_create(&numero_de_hilos[i], NULL, hilo, &lectores[i]);
        sem_init(&semaforo_leer[i], 0, 0);
    }

    sem_init(&semaforo_salir, 0, 0);

    while (salir_del_bucle == 0)
    {
        sleep(1);
        printf("1-Intentar leer\n");
        printf("2-Finalizar leer\n");
        printf("3-Salir\n");
        printf("Selecciones una opción (1-3): ");
        int seleccion_del_usuario;
        int opcion_del_usuario;

        scanf("%i", &seleccion_del_usuario);

        switch (seleccion_del_usuario)
        {

        case 1:
            printf("Introduzca el número del lector (de 1 a %i): ", N1);
            scanf("%i", &opcion_del_usuario);
            sem_post(&semaforo_leer[opcion_del_usuario - 1]);
            break;
        case 2:
            printf("Introduzca el número del lector (de 1 a %i): ", N1);
            scanf("%i", &opcion_del_usuario);
            sem_post(&semaforo_salir);
            break;
        case 3:
            salir_del_bucle = 1;
            break;

        default:
            break;
        }
    }
    return 0;
}
