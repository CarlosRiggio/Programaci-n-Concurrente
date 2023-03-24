#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>   

int main(){
    int i;
    int status;

    for(i=0;i<3;i++){
        if(fork()==0){
            printf("Proceso hijo con PID %i creado\n",getpid());
            sleep(3); 
            printf("El proceso con PID %i ha finalizado\n", getpid());
            exit(i); // con esto finalizamos el proceso hijo
        }
    }
    for(i=0;i<3;i++){
        if (WIFEXITED(status)) {
        printf("El proceso hijo con PID: %i terminó un código de salida %d\n",wait(&status),WEXITSTATUS(status));
        }
    }
    return 0; 
}