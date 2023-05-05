#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int msqid;
  key_t key;
  int msgflg = IPC_CREAT | 0666; // Permisos de la cola de mensajes

  // Verificar si se proporciona el número correcto de argumentos
  if (argc != 2) {
    fprintf(stderr, "Se debe proporcionar un valor entero como argumento\n");
    exit(1);
  }

  int num_queues = atoi(argv[1]); // Convertir el valor de cadena a entero

  // Crear la cola de mensajes
  for (int i = 0; i < num_queues; i++) {
    // Asignar la clave a la cola de mensajes
    key = i + 1234;

    // Crear la cola de mensajes
    if ((msqid = msgget(key, msgflg)) == -1) {
      perror("msgget");
      exit(1);
    } else {
      printf("La cola de mensajes %d se ha creado con ID %d\n", i, msqid);
    }
  }

  return 0;
}
