#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mensaje
{
    long mtype;
    char mtext[1024];
};

int main(int argc, char *argv[])
{
    int mqsid;
    key_t key;
    struct mensaje msg;

    mqsid = atoi(argv[1]);

    // if (msgrcv(mqsid, ))
    if (msgrcv(mqsid, &msg, sizeof(msg.mtext), strlen(msg.mtext) + 1,0) == -1) // el 0 significa modo bloqueante si pones NOWAIT es no bloqueante y así no te quedas esperando 
    {
        perror("msgrcv");
        exit(1);
    }
    else
        printf("message received: %s\n", msg.mtext);
}