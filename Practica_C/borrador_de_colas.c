#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int msqid;
  msqid = atoi(argv[1]);

  // Eliminar la cola de mensajes
  if (msgctl(msqid, IPC_RMID, NULL) == -1) {
    perror("msgctl");
    exit(1);
  } else {
    printf("La cola de mensajes con ID %d se ha eliminado\n", msqid);
  }

  return 0;
}