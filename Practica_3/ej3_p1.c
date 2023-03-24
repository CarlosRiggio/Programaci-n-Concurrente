#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    int in_char=0;

    int num = atoi(argv[1]);
    key_t key = ftok(".", 'a'); // creamos una clave en el directorio actual(eso significa el .) , usamos 1 para general la clave
    size_t size_int = 4;

    int id = shmget(key, size_int, IPC_CREAT | 0666); // crear memoria con la clave key, un tamaño de 4 e IPC_CREAT flag para crear una memoria. El 0666 indica q la memoria puede ser leida y escrita por cualquier usuario del sistema

    int *memoria_compartida = (int *)shmat(id, NULL, 0); // puntero al inicio de la zona de memoria compartida
    *memoria_compartida = num;                           // escribimos el numero en la zona de memoria compartida

    while(in_char!='x')
    {
        printf("Argumento en la zona de memoria compartida: %i\n", *memoria_compartida);
        printf("Presione 'X' para salir, sino el valor guardado en memoria se incrementará +1\n");
        in_char=getchar(); // para asignare el valor a in_char
        getchar(); // leer el caracter de nueva linea , sino coge el \n como otra linea y hace 2 veces el while
        (*memoria_compartida)++;
        
    }

    shmdt(memoria_compartida); // desvinculo la variable de ese hueco de memoria
    
    return 0;
}
