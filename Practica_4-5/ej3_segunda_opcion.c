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

void * Jordi_Alba_hilo(void *parametro){
    printf("Jordi Alba ");
    pthread_exit(NULL); 
}

void * Thiago_Y_Silva_hilo(void *parametro){
    printf("Thiago Silva ");
    pthread_exit(NULL); 
}
int main()
{   
    pthread_t Ramos, Pique, Jordi_Alba, Thiago_Y_Silva;
    printf("De Gea ");
    pthread_create(&Ramos,NULL,Ramos_hilo,NULL);
    pthread_create(&Pique,NULL,Pique_hilo,NULL);
    pthread_create(&Jordi_Alba,NULL,Jordi_Alba_hilo,NULL);

    printf("Carvajal ");
    

    
    pthread_join(Jordi_Alba, NULL);
    pthread_create(&Thiago_Y_Silva,NULL,Thiago_Y_Silva_hilo,NULL);

    pthread_join(Ramos, NULL);
    pthread_join(Pique, NULL);

    printf("Busquets ");
    printf("Isco ");
    printf("Aspas ");
    
    pthread_join(Thiago_Y_Silva,NULL);
    printf("Morata\n");
    return 0;

}