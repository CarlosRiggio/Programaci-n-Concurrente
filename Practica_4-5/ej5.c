#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaforo_general;
sem_t semaforo_escribir[10];
sem_t semaforo_salir;

void *hilo(void *arg)
{
    int lector = *(int *)arg;
    while (1)
    {
        printf("[Lector %i] -> Esperando a intentar escribir...\n", lector + 1);
        sem_wait(&semaforo_escribir[lector]);
        printf("[Lector %i] -> Intentando escribir...\n", lector + 1);
        sem_wait(&semaforo_general);
        printf("[Lector %i] -> Escribiendo...\n", lector + 1); // sección crítica
        sem_wait(&semaforo_salir);
        printf("[Lector %i] -> Fin escritura\n", lector + 1);
        sem_post(&semaforo_general);
        pthread_exit(NULL);
    }
}

int main(int argc, char *argv[])
{
    int i;
    int N3 = atoi(argv[1]);
    // int N2 = atoi(argv[2]);
    pthread_t numero_de_hilos[N3];
    int lectores[N3];
    int salir_del_bucle = 0;

    sem_init(&semaforo_general, 0, 1); // solo dejamos entrar a 1 escritor

    for (i = 0; i < N3; i++)
    {
        lectores[i] = i;
        pthread_create(&numero_de_hilos[i], NULL, hilo, &lectores[i]);
        sem_init(&semaforo_escribir[i], 0, 0);
    }

    sem_init(&semaforo_salir, 0, 0);

    while (salir_del_bucle == 0)
    {
        sleep(1);
        printf("1-Intentar escribir\n");
        printf("2-Finalizar escribir\n");
        printf("3-Salir\n");
        printf("Selecciones una opción (1-3): ");
        int seleccion_del_usuario;
        int opcion_del_usuario;

        scanf("%i", &seleccion_del_usuario);

        switch (seleccion_del_usuario)
        {

        case 1:
            printf("Introduzca el número del lector (de 1 a %i): ", N3);
            scanf("%i", &opcion_del_usuario);
            sem_post(&semaforo_escribir[opcion_del_usuario - 1]);
            break;
        case 2:
            printf("Introduzca el número del lector (de 1 a %i): ", N3);
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
