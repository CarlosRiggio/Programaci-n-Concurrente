#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>   

int main(int argc, char* argv[]){
    int i;
    int status=0;
    int pid[3];

    for(i=0;i<3;i++){
        if((pid[i]=fork())==0){
            printf("Proceso hijo con PID %i creado\n",getpid());
            sleep(3); 
            printf("El proceso con PID %i ha finalizado\n", getpid());
            //execl("ej4_child",argv[1],(char*)0); 
            execl("./ej4_child", argv[1], (char*)0);

            exit(0); // con esto finalizamos el proceso hijo
        }
    }
    for(i=0;i<3;i++){
    //status=0;   
     if (waitpid(pid[i],&status,0) != -1) {
        if (WIFEXITED(status)) {
            printf("El proceso hijo con PID: %i terminó un código de salida %d\n",pid[i],WEXITSTATUS(status));
        }
    }
}

}