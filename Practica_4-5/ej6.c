#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

// SEMAFOROS POR HILO
sem_t intentar_leer[50];
sem_t intentar_escribir[50];
sem_t acabar_leer[50];
sem_t acabar_escribir[50];

// SEMAFOROS DE SINCRONIZACION
sem_t sem_papel_lector_o_escritor_sync;
sem_t sem_cola_escritor_espera_sync;
sem_t sem_cola_lector_espera_sync;


// VARIABLES GLOBALES CON SUS CORRESPONDIENTES SEMÁFOROS
sem_t sem_comprobar_si_hay_algiuen_dentro;
int escritor_dentro = 0;
int lector_dentro = 0;
sem_t sem_numero_lectores_leyendo;
int numero_lectores_leyendo;
sem_t sem_numero_de_lectores_en_cola;
int numero_de_lectores_en_cola = 0;
sem_t sem_numero_de_escritores_en_cola;
int numero_de_escritores_en_cola = 0;

// HILO LECTOR
void *hilo_Lector(void *num_lector)
{
    while (1)
    {
        int numero_lector = *(int *)num_lector;
        printf("[Lector %i] -> Esperando a intentar leer...\n", numero_lector + 1);
        sem_wait(&intentar_leer[numero_lector]);
        printf("[Lector %i] -> Intentando leer...\n", numero_lector + 1);
        //----------------------------------------------------------------------------------------------
        sem_wait(&sem_comprobar_si_hay_algiuen_dentro); //+++ Lector solo entra en cola si hay un escritor dentro
            if (escritor_dentro == 1)
            {
                sem_post(&sem_comprobar_si_hay_algiuen_dentro);

                sem_wait(&sem_numero_de_lectores_en_cola);
                numero_de_lectores_en_cola++;
                sem_post(&sem_numero_de_lectores_en_cola);
                //
                sem_wait(&sem_cola_lector_espera_sync); // Esperamos a q el escritor nos avise a q ha salido
                // El escritor despierta al primer lector q quiere entrar
                sem_wait(&sem_numero_de_lectores_en_cola);
                    numero_de_lectores_en_cola--;
                    // Este lector comprueba si hay + lectores suspendidos, y si los hay los despiert
                    if (numero_de_lectores_en_cola > 0)
                    {
                        sem_post(&sem_cola_lector_espera_sync);
                    }
                sem_post(&sem_numero_de_lectores_en_cola); 
            }
        sem_post(&sem_comprobar_si_hay_algiuen_dentro); 

        // Tras esto el lector ya ha comprobado si hay escritor y ,de serlo necesario, esperar a q se vaya.
        // Estamos dentro de la sección crítica
        sem_wait(&sem_papel_lector_o_escritor_sync);//SC

            sem_wait(&sem_comprobar_si_hay_algiuen_dentro);
            lector_dentro = 1;
            sem_post(&sem_comprobar_si_hay_algiuen_dentro);

            sem_wait(&sem_numero_lectores_leyendo);
            numero_lectores_leyendo++;
            sem_post(&sem_numero_lectores_leyendo);

            printf("[Lector %i] -> Leyendo...\n", numero_lector + 1);

            sem_wait(&acabar_leer[numero_lector]);
            printf("[Lector %i] -> Fin lectura\n", numero_lector + 1);

            sem_wait(&sem_numero_lectores_leyendo);
            numero_lectores_leyendo--;
        //sem_post(&sem_papel_lector_o_escritor_sync);//SC

        if (numero_lectores_leyendo == 0)
        {
            sem_wait(&sem_comprobar_si_hay_algiuen_dentro);
            lector_dentro = 0; // Decimos q ya no hay lectores dentro

            sem_post(&sem_papel_lector_o_escritor_sync);//SC

            sem_post(&sem_comprobar_si_hay_algiuen_dentro);
            //--------------------//---------------------//
            

            sem_wait(&sem_numero_de_escritores_en_cola);
            if (numero_de_escritores_en_cola != 0)
            {
                sem_post(&sem_cola_escritor_espera_sync); // Si hay escritores para entrar, les dejamos pasar.
            }   
            sem_post(&sem_numero_de_escritores_en_cola);
            

        }else // si sigue habiendo lectores para leer
        {
            sem_post(&sem_papel_lector_o_escritor_sync);
        }
        sem_post(&sem_numero_lectores_leyendo);
    }

    pthread_exit((void *)NULL);
}

// HILO ESCRITOR
void *hilo_Escritor(void *num_escritor)
{
    while (1)
    {
        int numero_escritor = *(int *)num_escritor;

        printf("[Escritor %i] -> Esperando a intentar escribir...\n", numero_escritor + 1);
        sem_wait(&intentar_escribir[numero_escritor]);
        printf("[Escritor %i] -> Intentando escribir...\n", numero_escritor + 1);
        //--------------------------------------------------
        sem_wait(&sem_comprobar_si_hay_algiuen_dentro); // escritor solo entra si no hay ningun lector  ni ningun escritor dentro
        if ((escritor_dentro != 0) || (lector_dentro != 0))
        {
            sem_wait(&sem_numero_de_escritores_en_cola);
            numero_de_escritores_en_cola++;
            sem_post(&sem_numero_de_escritores_en_cola);
            
            sem_post(&sem_comprobar_si_hay_algiuen_dentro);

            sem_wait(&sem_cola_escritor_espera_sync);
            // Al recibir la señal ya salimos de la cola y entramos en la SC
            sem_wait(&sem_numero_de_escritores_en_cola);
            numero_de_escritores_en_cola--;
            sem_post(&sem_numero_de_escritores_en_cola);
        }
        sem_post(&sem_comprobar_si_hay_algiuen_dentro);

        sem_wait(&sem_papel_lector_o_escritor_sync); // SC

        sem_wait(&sem_comprobar_si_hay_algiuen_dentro);
        escritor_dentro = 1;
        sem_post(&sem_comprobar_si_hay_algiuen_dentro);

        printf("[Escritor %i] -> Escribiendo...\n", numero_escritor + 1);
        sem_wait(&acabar_escribir[numero_escritor]);
        printf("[Escritor %i] -> Fin escritura\n", numero_escritor + 1);

        sem_wait(&sem_comprobar_si_hay_algiuen_dentro);
        escritor_dentro = 0;
        sem_post(&sem_comprobar_si_hay_algiuen_dentro);

        sem_post(&sem_papel_lector_o_escritor_sync); // SC

        sem_wait(&sem_numero_de_escritores_en_cola);
            if(numero_de_escritores_en_cola!=0)
            {
                sem_post(&sem_cola_escritor_espera_sync);
            }else
            {
                sem_wait(&sem_numero_de_lectores_en_cola);
                if(numero_de_lectores_en_cola!=0)
                {
                    sem_post(&sem_cola_lector_espera_sync);
                }
                sem_post(&sem_numero_de_lectores_en_cola);
                //sem_post(&sem_cola_escritor_espera_sync);
            }
        sem_post(&sem_numero_de_escritores_en_cola);

    }

    pthread_exit((void *)NULL);
}

int main(int argc, char *argv[])
{
    int seleccion_del_usuario;
    int opcion_usuario;
    int n1, n2, n3;
    int salir_del_bucle = 0;
    int i;

    n1 = atoi(argv[1]); // numero total de lectores posibles
    n2 = atoi(argv[2]); // numero total de lectores simultaneos
    n3 = atoi(argv[3]); // numero total escritores posibles

    int lectores[n1];
    int escritores[n3];

    if (n2 > n1)
    {
        printf("El valor n2, debe ser menor que el valor n1\n");
        return 0;
    }

    pthread_t lectores_hilos[n1];
    pthread_t escritores_hilos[n3];

    // creamos los hilos de lectores
    for (i = 0; i < n1; i++)
    {
        lectores[i] = i;
        pthread_create(&lectores_hilos[i], NULL, hilo_Lector, &lectores[i]);

        // creamos semáforo de inicio y final para cada hilo lector
        sem_init(&intentar_leer[i], 0, 0);
        sem_init(&acabar_leer[i], 0, 0);
    }

    for (i = 0; i < n3; i++)
    {
        escritores[i] = i;
        pthread_create(&escritores_hilos[i], NULL, hilo_Escritor, &escritores[i]);
        // creamos semáforo de inicio y final para cada hilo escritor
        sem_init(&intentar_escribir[i], 0, 0);
        sem_init(&acabar_escribir[i], 0, 0);
    }

    // SEMAFOROS DE SINCRONIZACIÓN
    sem_init(&sem_papel_lector_o_escritor_sync, 0, n2); // diferenciar entre solo dejar entrar a los lectores o a los escritores
    sem_init(&sem_cola_escritor_espera_sync, 0, 0);
    sem_init(&sem_cola_lector_espera_sync, 0, 0);


    // SEMAFOROS DE VARIABLES
    sem_init(&sem_comprobar_si_hay_algiuen_dentro, 0, 1);
    sem_init(&sem_numero_de_escritores_en_cola, 0, 1);
    sem_init(&sem_numero_de_lectores_en_cola, 0, 1);
    sem_init(&sem_numero_lectores_leyendo, 0, 1);

    while (salir_del_bucle == 0)
    {
        sleep(1);
        printf("\n");
        printf("1-Intentar escribir\n");
        printf("2-Finalizar escribir\n");
        printf("3-Intentar leer\n");
        printf("4-Finalizar leer\n");
        printf("5-Salir\n");
        printf("\n");
        printf("Selecciones una opción (1-5): ");
        scanf("%i", &seleccion_del_usuario);

        switch (seleccion_del_usuario)
        {
        case 1:
            printf("Introduzca el número del escritor (1 a %i): ", n3);
            scanf("%i", &opcion_usuario);
            sem_post(&intentar_escribir[opcion_usuario - 1]);
            break;

        case 2:
            printf("Introduzca el número del escritor (1 a %i): ", n3);
            scanf("%i", &opcion_usuario);
            sem_post(&acabar_escribir[opcion_usuario - 1]);
            break;
        case 3:
            printf("Introduzca el número del lector (1 a %i): ", n1);
            scanf("%i", &opcion_usuario);
            sem_post(&intentar_leer[opcion_usuario - 1]);
            break;
        case 4:
            printf("Introduzca el número del lector (1 a %i): ", n1);
            scanf("%i", &opcion_usuario);
            sem_post(&acabar_leer[opcion_usuario - 1]);
            break;
        case 5:
            salir_del_bucle = 1;
            break;

        default:
            break;
        }
    }
}