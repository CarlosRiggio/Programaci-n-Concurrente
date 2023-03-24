#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    key_t key = ftok(".", 'a');
    size_t size_int = 4;
    int id = shmget(key, size_int, 0666);
    int *memoria_compartida = (int *)shmat(id, NULL, 0);

    shmdt(memoria_compartida);
    shmctl(id, IPC_RMID, NULL); // eliminamos la memoria compartida, tras ello no podemos volver a acceder a ella

    printf("La memoria compartida ha sido liberada.\n");

    return 0;
}