#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct message {
  long mtype;
  char mtext[1024];
};

int main(int argc, char *argv[])
{
  int msqid;
  key_t key;
  struct message msg;

  key = atoi(argv[1]);

  // Obtener el ID de la cola de mensajes
  if ((msqid = msgget(key, 0666)) == -1) {
    perror("msgget");
    exit(1);
  }

  // Preparar el mensaje
  msg.mtype = 1;
  strcpy(msg.mtext, "Hola, mundo!");

  // Enviar el mensaje a la cola de mensajes
  if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
    perror("msgsnd");
    exit(1);
  } else {
    printf("El mensaje se ha enviado correctamente a la cola de mensajes con ID %d\n", msqid);
  }

  return 0;
}
