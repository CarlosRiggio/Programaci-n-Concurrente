#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
  int msqid;
  key_t key;
  int msgflg = IPC_CREAT | 0666; // Permisos de la cola de mensajes

  // Asignar la clave a la cola de mensajes
  key = 1234;

  // Crear la cola de mensajes
  if ((msqid = msgget(key, msgflg)) == -1) {
    perror("msgget");
    exit(1);
  } else {
    printf("La cola de mensajes se ha creado con ID %d\n", msqid);
  }

  return 0;
}
