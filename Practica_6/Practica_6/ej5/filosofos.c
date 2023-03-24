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

int main(int argc, char *argv[])
{
  int numero_de_filosofo = atoi(argv[1]);
  int id_de_cola_a_entrar = atoi(argv[2]);

  key_t key;
  key = 1234;

  struct message msg1;
  struct message msg2;

  printf("Eres el Filósofo número: %i \n", numero_de_filosofo);
  printf("Enter para coger 2 tenedores\n");
  getchar();
  msgrcv(id_de_cola_a_entrar, (void *)&msg1, sizeof(msg1.tenedor), numero_de_filosofo, 0);
  msgrcv(id_de_cola_a_entrar, (void *)&msg2, sizeof(msg2.tenedor), numero_de_filosofo + 1, 0);
  printf("Tenedores cogidos\n");
  printf("Enter para dejar los tenedores\n");
  getchar();
  msgsnd(id_de_cola_a_entrar, (void *)&msg1, sizeof(msg1.tenedor), 1);
  msgsnd(id_de_cola_a_entrar, (void *)&msg2, sizeof(msg1.tenedor), 1);
  printf("Tenedores dejados\n");

  return 0;
}
