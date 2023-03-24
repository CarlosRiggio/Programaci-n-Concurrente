#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char valor_leido_del_teclado;
int contador_de_caracteres;

typedef struct
{
    int posicion;
    char* argumentos;
}posicion_y_cadena;

void * thread(posicion_y_cadena *data){
    printf("Posición:%i\nArgumento:%s\n\n",data->posicion,data->argumentos);
    pthread_exit(NULL); // para cerrar el hilo una vez acabe

}

int main(int argc, char *argv[])
{
    int numero_de_hilos = argc-1;
    int crear_hilo;
    int aux=0;
    pthread_t hilos[numero_de_hilos];
    posicion_y_cadena pyc[numero_de_hilos];


    for(aux;aux<numero_de_hilos;aux++)
    {
        pyc[aux].posicion=aux;
        pyc[aux].argumentos= argv[aux+1];

        crear_hilo=pthread_create(&hilos[aux],NULL,(void*)thread,(void*)&pyc[aux]);

        if(crear_hilo!=0)
        {
        perror("No se pudo crear el hilo\n");
        exit(-1);
        }
        sleep(1); // este sleep es para q a los hilos les de tiempo a ejecutarse antes de q acabe el proceso
    }
    
    return 0;
}
