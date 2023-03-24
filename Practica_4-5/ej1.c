#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char valor_leido_del_teclado;
int contador_de_caracteres;

void * thread(void *arg){
    printf("Introducir un caracter (q para salir): ");
    while(valor_leido_del_teclado!='q'){
        valor_leido_del_teclado=getchar();
        getchar(); // para no utilizar el salto de linea q introducimos
        getchar(); // este para el salto de linea q va antes de numero de caracteres
        contador_de_caracteres++;
    }

    pthread_exit(NULL); // para cerrar el hilo una vez acabe

}

int main()
{
    pthread_t hilo; // variable para identificar al hilo
    int crear_hilo;

    crear_hilo=pthread_create(&hilo,NULL,thread,NULL);
    //&hilo: puntero a la variable hilo donde se almacenará su identificador
    //NULL: utilizamos los atributos predeterminados para el hilo
    //thread: puntero a la función q el hilo ejecutará
    //puntero a los argumentos q se le pasan a la funcion

    if(crear_hilo!=0){
        perror("No se pudo crear el hilo\n");
        exit(-1);
    }

    while(valor_leido_del_teclado!='q'){
        sleep(1);
        printf("\nNúmero de caracteres leídos: %i\n",contador_de_caracteres);
    }

    return 0;
}