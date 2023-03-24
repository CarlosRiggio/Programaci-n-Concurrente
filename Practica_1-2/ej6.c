#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int procesos=3;
    int status;
    int proceso4_acabado=0;
    int ja_done=0;
    int car_done=0;
    printf("De Gea ");

    //fflush(stdout); // para limpiar el buffer y que el proceso hijo no imprime tb De Gea
    
    printf("Ramos ");

    if (fork() == 0)  //proceso 2
    {
        execl("ej6_child", "Carvajal ", (char *)0);
        procesos--;
        sleep(2);
        exit(1);
    }
    
    if (fork() == 0)  // proceso 3
    {
        execl("ej6_child", "Piqué ", (char *)0);
        procesos--;
        sleep(4);
        exit(2);
    }
       if (fork() == 0)  // proceso jordi alba
    {
        execl("ej6_child", "Jordi Alba ", (char *)0);
        ja_done=1;
        procesos--;
        sleep(4);

        exit(4);
    }

    while(ja_done!=1)
    {
        int pid= wait(&status);
        if (WIFEXITED(status))
        {
            ja_done = 1;
        }
    }

    if (fork()==0) // prceso 4 
    {
        proceso4_acabado=1;
        execl("ej6_child", "Thiago Silva ", (char *)0);
        sleep(2);
        exit(3);
    }
//    }

    
 

    while(procesos!=0){
        
        int pid= wait(&status);
        if (WIFEXITED(status))
        {
            procesos--;  
        }
        
    }

    printf("Busquets ");

    sleep(2);
    //fflush(stdout);

    while(car_done!=1)
    {
        int pid= wait(&status);
        if (WIFEXITED(status))
        {
            car_done = 1;
        }
    }

    printf("Isco ");
    printf("Aspas ");
        

    while(proceso4_acabado!=1)
    {
        int pid= wait(&status);
        if (WIFEXITED(status))
        {
            printf(" Morata\n");
            return 0; 
        }

    }
    
    
    return 0;
}
