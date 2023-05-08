#include <stdio.h>
#include <pthread.h>

#define NUM_HILOS 3

void *funcion_hilo(void *arg) {
    pthread_t id_hilo = pthread_self();
    printf("ID del hilo: %lu\n", id_hilo);
    return NULL;
}

void lanzar_hilos() {
    pthread_t hilos[NUM_HILOS];

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_create(&hilos[i], NULL, funcion_hilo, NULL);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }
}

int main() {
    lanzar_hilos();
    return 0;
}
