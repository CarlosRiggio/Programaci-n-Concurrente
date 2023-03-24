#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
    int aux = 1;
    int num_proc = atoi(argv[1]);
    int num_not_my_proc;
    key_t key = ftok(".", 'b');
    size_t size_int = 4;

    int id = shmget(key, size_int, IPC_CREAT | 0666); // creamos o accedemos a la dirección de memoria
    int *memoria_compartida = (int *)shmat(id, NULL, 0);

    //*memoria_compartida = 2; // uso este valor para la opción en la q ambas puertas esten abiertas
    
    if(num_proc==0){
        num_not_my_proc=1;
    }else{
        num_not_my_proc=0;
    }

        memoria_compartida[0]=1; // hacemos q ambas estén abiertas al principio 
        memoria_compartida[1]=1;

    while(1)
    {
        //if(aux){
            printf("Caminando por mi habitación\n");
            getchar();
            printf("Dentro del pasillo\n");
            getchar();
            printf("He accionado el pulsador");
        //}
        memoria_compartida[num_not_my_proc]=0; // inhabilitamos la puerta ajena
        getchar();
        printf("Intentando acceder a la Sección Crítica\n");

        if(memoria_compartida[num_proc]==1){
            printf("Dentro de mi Sección Crítica\n");
            getchar();
        }else{
            printf("Puerta cerrada. Saliendo del Pasillo...\n");
            
            getchar();
            printf("He accionado el pulsador\n");
            memoria_compartida[num_not_my_proc]=1;
            getchar();
            printf("He salido del Pasillo\n");
          //  aux=0;
            continue;
        }
        printf("He salido de mi Sección Crítica\n");
        getchar();
        //aux=1;
        
        printf("He accionado el pulsador\n");
        memoria_compartida[num_not_my_proc]=1; // habilitamos la puerta ajena
    
    }
        
    

    return 0;
}
