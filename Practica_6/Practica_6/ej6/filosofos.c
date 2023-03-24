#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct message
{
  long mtype;
  int tenedor;
};

struct ticket
{
  long mtype;
  int num;
};

int main(int argc, char *argv[])
{
  int numero_de_filosofo = atoi(argv[1]);
  // int id_de_cola_a_entrar = atoi(argv[2]);
  int id_cola_tenedores_1;
  int id_cola_tenedores_2;
  int id_cola_tenedores_3;
  int id_cola_tenedores_4;
  int id_cola_tenedores_5;
  int id_cola_tickets;

  key_t key;
  key = 1234;

  struct message tenedor_izq;
  struct message tenedor_dcha;
  struct ticket ticket;

  printf("Eres el Filósofo número: %i \n", numero_de_filosofo);
  // CON ESTO CONSEGUIMOS TENER EL ID DE TODAS LAS COLAS A PARTIR DELA PRIMERA
  printf("Introduce el ID del la cola de tenedores 1: ");
  scanf("%i", &id_cola_tenedores_1);

  id_cola_tenedores_2 = id_cola_tenedores_1 + 1;
  id_cola_tenedores_3 = id_cola_tenedores_2 + 1;
  id_cola_tenedores_4 = id_cola_tenedores_3 + 1;
  id_cola_tenedores_5 = id_cola_tenedores_4 + 1;
  id_cola_tickets = id_cola_tenedores_5 + 1;

  printf("ID cola de tenedores 1: %i\n", id_cola_tenedores_1);
  printf("ID cola de tenedores 2: %i\n", id_cola_tenedores_2);
  printf("ID cola de tenedores 3: %i\n", id_cola_tenedores_3);
  printf("ID cola de tenedores 4: %i\n", id_cola_tenedores_4);
  printf("ID cola de tenedores 5: %i\n", id_cola_tenedores_5);
  printf("ID cola de tickets: %i\n", id_cola_tickets);

  printf("Enter para coger 1 de los 4 tickets de entrada\n");
  getchar();
  printf("Esperando a coger tickets...\n");
  msgrcv(id_cola_tickets, (void *)&ticket, sizeof(ticket.num), 12, 0); // Bloqueante, hasta q no reciba el ticket no avanzo
  printf("Ticket cogido\n");
  printf("Enter para cogoer tenedores\n");
  getchar();
  printf("Cogiendo tenedor de la izquierda...\n");
  switch (numero_de_filosofo)
  {
  case 1:
    msgrcv(id_cola_tenedores_1, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), numero_de_filosofo, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo);
    break;
  case 2:
    msgrcv(id_cola_tenedores_2, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), numero_de_filosofo, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo);
    break;
  case 3:
    msgrcv(id_cola_tenedores_3, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), numero_de_filosofo, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo);
    break;
  case 4:
    msgrcv(id_cola_tenedores_4, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), numero_de_filosofo, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo);
    break;
  case 5:
    msgrcv(id_cola_tenedores_5, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), numero_de_filosofo, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo);
    break;
  }

  printf("Cogiendo tenedor de la derecha...\n");

  switch (numero_de_filosofo)
  {
  case 1:
    msgrcv(id_cola_tenedores_2, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), numero_de_filosofo + 1, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo + 1);
    break;
  case 2:
    msgrcv(id_cola_tenedores_3, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), numero_de_filosofo + 1, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo + 1);
    break;
  case 3:
    msgrcv(id_cola_tenedores_4, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), numero_de_filosofo + 1, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo + 1);
    break;
  case 4:
    msgrcv(id_cola_tenedores_5, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), numero_de_filosofo + 1, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo + 1);
    break;
  case 5:
    msgrcv(id_cola_tenedores_1, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), numero_de_filosofo - 4, 0);
    printf("Tenedor %i cogido\n", numero_de_filosofo - 4);
    break;
  }

  printf("Enter para comer\n");
  getchar();
  printf("Comiendo...\n");
  printf("Enter para dejar los tenedores\n");
  getchar();
  printf("Dejando el tenedor izquierdo...\n");
  switch (numero_de_filosofo)
  {
  case 1:
    msgsnd(id_cola_tenedores_1, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo);
    break;

  case 2:
    msgsnd(id_cola_tenedores_2, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo);
    break;

  case 3:
    msgsnd(id_cola_tenedores_3, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo);
    break;

  case 4:
    msgsnd(id_cola_tenedores_4, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo);
    break;

  case 5:
    msgsnd(id_cola_tenedores_5, (void *)&tenedor_izq, sizeof(tenedor_izq.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo);
    break;
  }

  printf("Dejando el tenedor derecho...\n");

  switch (numero_de_filosofo)
  {
  case 1:
    msgsnd(id_cola_tenedores_2, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo + 1);
    break;

  case 2:
    msgsnd(id_cola_tenedores_3, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo + 1);
    break;

  case 3:
    msgsnd(id_cola_tenedores_4, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo + 1);
    break;

  case 4:
    msgsnd(id_cola_tenedores_5, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo + 1);
    break;

  case 5:
    msgsnd(id_cola_tenedores_1, (void *)&tenedor_dcha, sizeof(tenedor_dcha.tenedor), 0);
    printf("Tenedor %i dejado\n", numero_de_filosofo - 4);
    break;
  }

  printf("Dejando el ticket...\n");
  // msgrcv(id_cola_tickets,(void *)&ticket,sizeof(ticket.num),12,0);
  msgsnd(id_cola_tickets, (void *)&ticket, sizeof(ticket.num), 0);
  printf("Ticket dejado\n");
  return 0;
}
