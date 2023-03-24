#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

int contador_chld_terminados = 0;

void handler(int signal){
    printf("Proceso Hijo Finalizado \n");
    contador_chld_terminados = contador_chld_terminados +1;

}

int main(){
    int i;
    for(i=0;i<3;i++){
        if(fork()==0){
            printf("Proceso hijo con PID %i creado\n",getpid());
            sleep(3); //dormimos el proceso 3 segundos para q tarde en finalizar
            printf("El proceso con PID %i ha finalizado\n", getpid());
            return 0;
        }
    }
    
    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = SA_NODEFER; //la señal es entregada directamente 

    sigaction(SIGCHLD,&sig,NULL);//SIGCHLD es la señal q envían los procesos hijos cunado terminan

    while(contador_chld_terminados <3){
        pause();
    }

    return 0;
}