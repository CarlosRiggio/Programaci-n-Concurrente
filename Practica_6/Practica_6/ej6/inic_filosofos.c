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

struct ticket
{
    long mtype;
    int num;
}ticket[4];



int main(int argc, char *argv[])
{
    int id_cola_tenedor_1;
    int id_cola_tenedor_2;
    int id_cola_tenedor_3;
    int id_cola_tenedor_4;
    int id_cola_tenedor_5;

    int id_cola_tickets;

    int aux;
    int aux_ticket;
    key_t key_tenedor_1;
    key_t key_tenedor_2;
    key_t key_tenedor_3;
    key_t key_tenedor_4;
    key_t key_tenedor_5;

    key_t key_tickets;

    struct mensaje tenedor;
    struct ticket ticket_;

    int msgflg = IPC_CREAT | 0777;
    
    //LLAVES PARA CADA COLA
    key_tenedor_1=1;
    key_tenedor_2=2;
    key_tenedor_3=3;
    key_tenedor_4=4;
    key_tenedor_5=5;
    key_tickets= 4321;

    //ID DE LAS COLAS
    id_cola_tenedor_1 = msgget(key_tenedor_1, msgflg);
    printf("Se ha creado la cola del tenedor 1 con ID: %d\n", id_cola_tenedor_1);
    id_cola_tenedor_2 = msgget(key_tenedor_2, msgflg);
    printf("Se ha creado la cola del tenedor 2 con ID: %d\n", id_cola_tenedor_2);
    id_cola_tenedor_3 = msgget(key_tenedor_3, msgflg);
    printf("Se ha creado la cola del tenedor 3 con ID: %d\n", id_cola_tenedor_3);
    id_cola_tenedor_4 = msgget(key_tenedor_4, msgflg);
    printf("Se ha creado la cola del tenedor 4 con ID: %d\n", id_cola_tenedor_4);
    id_cola_tenedor_5 = msgget(key_tenedor_5, msgflg);
    printf("Se ha creado la cola del tenedor 5 con ID: %d\n", id_cola_tenedor_5);
    id_cola_tickets=msgget(key_tickets,msgflg);
    printf("Se ha creado la cola de tickets con ID: %d\n",id_cola_tickets);
    
    
    
    //INTRODUCIMOS LOS % VALORES A LA COLA DE TENEDORES 1
    //for(aux=0;aux<5;aux++)
    //{
        tenedor.mtype=1;
        tenedor.tenedor=1;
        msgsnd(id_cola_tenedor_1, &tenedor, sizeof(int), IPC_NOWAIT);

    //}

        //INTRODUCIMOS LOS % VALORES A LA COLA DE TENEDORES 2
    //for(aux=0;aux<5;aux++)
    //{
        tenedor.mtype=2;
        tenedor.tenedor=2;
        msgsnd(id_cola_tenedor_2, &tenedor, sizeof(int), IPC_NOWAIT);

    //}

        //INTRODUCIMOS LOS % VALORES A LA COLA DE TENEDORES 3
    //for(aux=0;aux<5;aux++)
    //{
        tenedor.mtype=3;
        tenedor.tenedor=3;
        msgsnd(id_cola_tenedor_3, &tenedor, sizeof(int), IPC_NOWAIT);

    //}

        //INTRODUCIMOS LOS % VALORES A LA COLA DE TENEDORES 4
    //for(aux=0;aux<5;aux++)
    //{
        tenedor.mtype=4;
        tenedor.tenedor=4;
        msgsnd(id_cola_tenedor_4, &tenedor, sizeof(int), IPC_NOWAIT);

    //}

        //INTRODUCIMOS LOS % VALORES A LA COLA DE TENEDORES 5
    //for(aux=0;aux<5;aux++)
    //{
        tenedor.mtype=5;
        tenedor.tenedor=5;
        msgsnd(id_cola_tenedor_5, &tenedor, sizeof(int), IPC_NOWAIT);

   // }
    
    //INTRODUCIMOS LOS 4 VALORES A LA COLA DE TICKETS
    for(aux_ticket=0;aux_ticket<4;aux_ticket++)
    {
        ticket[aux_ticket].mtype=12; // le ponemos valores fijos q sabemos q van a referir solo a los tickets
        ticket[aux_ticket].num=12;
        msgsnd(id_cola_tickets,&ticket[aux_ticket], sizeof(int), IPC_NOWAIT);
    }


    return 0;
}