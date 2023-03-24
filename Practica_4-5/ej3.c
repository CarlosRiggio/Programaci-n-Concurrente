#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void * Ramos_hilo(void *parametro){
    printf("Ramos ");
    pthread_exit(NULL); // para cerrar el hilo una vez acabe
    
}

void * Pique_hilo(void *parametro){
    printf("Pique ");
    pthread_exit(NULL); 
}

void * Carvajal_hilo(void *parametro){
    printf("Carvajal ");
    pthread_exit(NULL); 
}

void * Thiago_Y_Silva_hilo(void *parametro){
    printf("Thiago Silva ");
    pthread_exit(NULL); 
}

int main()
{   
    pthread_t Ramos, Pique, Carvajal, Thiago_Y_Silva;
    printf("De Gea ");
    pthread_create(&Ramos,NULL,Ramos_hilo,NULL);
    pthread_create(&Pique,NULL,Pique_hilo,NULL);
    pthread_create(&Carvajal,NULL,Carvajal_hilo,NULL);

    printf("Jordi Alba ");
    pthread_create(&Thiago_Y_Silva,NULL,Thiago_Y_Silva_hilo,NULL);

    pthread_join(Ramos, NULL);
    pthread_join(Pique, NULL);
    pthread_join(Carvajal, NULL);

    printf("Busquets ");

    printf("Isco ");
    printf("Aspas ");

    printf("Morata\n");
    return 0;

}