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
  int tipo_de_fumador;
  int id_cola;
  struct message msg;
  struct message respuesta;
  respuesta.mtype = 2;

  printf("Introduce el ID de la cola a entrar:");
  scanf("%i", &id_cola);

  printf("Fumadores:\n");
  printf("1-Tabaco\n");
  printf("2-Fósforo\n");
  printf("3-Papel\n");

  printf("Selecciona que tipo de fumador eres:");
  scanf("%i", &tipo_de_fumador);
  printf("Eres fumador del tipo= %i\n", tipo_de_fumador);
  printf("Enter para esperar a recibir los ingredientes que falta\n");
  getchar();

  while (1)

  {
    msgrcv(id_cola, (void *)&msg, 2 * sizeof(int), 1, 0);

    switch (tipo_de_fumador)
    {
    case 1: // FUMADOR CON TABACO
      if ((msg.ingrediente1 == 2 && msg.ingrediente2 == 3) || (msg.ingrediente1 == 3 && msg.ingrediente2 == 2))
      {

        printf("Ingrediente1=%i\n", msg.ingrediente1);
        printf("Ingrediente2=%i\n", msg.ingrediente2);

        printf("Fumando...\n");
        sleep(2);
        msgsnd(id_cola, (void *)&respuesta, 2 * sizeof(int), 0);
        printf("Dejó de fumar\n");
      }
      else
      {
        msgsnd(id_cola, (void *)&msg, 2 * sizeof(int), 0);
      }

      break;

    case 2: // FUMADOR CON FÓSFORO
      if ((msg.ingrediente1 == 1 && msg.ingrediente2 == 3) || (msg.ingrediente1 == 3 && msg.ingrediente2 == 1))
      {
        printf("Ingrediente1=%i\n", msg.ingrediente1);
        printf("Ingrediente2=%i\n", msg.ingrediente2);

        printf("Fumando...\n");
        sleep(2);
        msgsnd(id_cola, (void *)&respuesta, 2 * sizeof(int), 0);
        printf("Dejó de fumar\n");
      }
      else
      {
        msgsnd(id_cola, (void *)&msg, 2 * sizeof(int), 0);
      }

      break;

    case 3: // FUMADOR CON PÁPEL
      if ((msg.ingrediente1 == 1 && msg.ingrediente2 == 2) || (msg.ingrediente1 == 2 && msg.ingrediente2 == 1))
      {

        printf("Ingrediente1=%i\n", msg.ingrediente1);
        printf("Ingrediente2=%i\n", msg.ingrediente2);

        printf("Fumando...\n");
        sleep(2);
        msgsnd(id_cola, (void *)&respuesta, 2 * sizeof(int), 0);
        printf("Dejó de fumar\n");
      }
      else
      {
        msgsnd(id_cola, (void *)&msg, 2 * sizeof(int), 0);
      }
      break;
    }
  }
  
  return 0;
}