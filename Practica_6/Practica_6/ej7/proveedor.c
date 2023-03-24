#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct message
{
    long mtype;
    int ingrediente1;
    int ingrediente2;
};

int main(int argc, char *argv[])
{
    int sumador = 0;

    int msgflg = IPC_CREAT | 0777;
    key_t key = 1;
    int id_cola;
    id_cola = msgget(key, msgflg);
    printf("Se ha generado la cola con ID: %i\n", id_cola);

    while (1)
    {
        struct message msg;

        msg.mtype = 1; // el tipo de mensaje va a ser siempre 1
                       // inicializar la semilla del generador de números aleatorios
        srand(sumador);
        msg.ingrediente1 = rand() % 3 + 1;
        do
        {
            srand(sumador);
            msg.ingrediente2 = rand() % 3 + 1;
            sumador++;

        }while (msg.ingrediente1 == msg.ingrediente2);

        // sleep(1);
        printf("Se ha generado con un Ingrediente 1 = %i\n", msg.ingrediente1);
        printf("Se ha generado con un Ingrediente 2 = %i\n", msg.ingrediente2);
        printf("\n");
        msgsnd(id_cola, &msg, 2*sizeof(int), IPC_NOWAIT);

        

        // ESPERAMOS A RECIBIR UNA SEÑAL DE Q EL FUMADOR ACABÓ DE FUMAR, SERÁ DEL TIPO 2
        msgrcv(id_cola, (void *)&msg, 2 * sizeof(int), 2, 0); // es bloqueante;

    }
}
