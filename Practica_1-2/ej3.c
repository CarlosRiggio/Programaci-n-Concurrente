#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>   

int main(){
    int i;
    int status=0;
    int pid[3];

    for(i=0;i<3;i++){
        if((pid[i]=fork())==0){
            printf("Proceso hijo con PID %i creado\n",getpid());
            sleep(3); 
            printf("El proceso con PID %i ha finalizado\n", getpid());
            exit(i); // con esto finalizamos el proceso hijo
        }
    }
    for(i=0;i<3;i++){
     if (waitpid(pid[i],&status,0) != -1) {
        if (WIFEXITED(status)) {
            printf("El proceso hijo con PID: %i terminó un código de salida %d\n",pid[i],WEXITSTATUS(status));
        }
    }
}

}