#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mensaje
{
    long mtype;
    int tenedor;
}tenedor[5];



int main(int argc, char *argv[])
{
    int mqsid;
    int aux;
    key_t key;
    struct mensaje msg;
    int msgflg = IPC_CREAT | 0777;
    
    
    key=1234;

    mqsid = msgget(key, msgflg);
    printf("La cola de mensajes se ha creado con ID %d\n", mqsid);
    printf("La cola creaada tiene ID: %i\n",mqsid);

    for(aux=0;aux<5;aux++)
    {
        tenedor[aux].mtype=aux+1;
        tenedor[aux].tenedor=aux+1;
        msgsnd(mqsid, &tenedor[aux], sizeof(int), IPC_NOWAIT);

    }


    return 0;
}