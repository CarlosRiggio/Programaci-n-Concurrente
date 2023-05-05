#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

struct mensaje_buzon_interno{
    long tipo;
};

struct mensaje
{
    long tipo;
    int ticket;
    int id_nodo;
    int puerto;
    int prioridad;
};

struct info_a_mandar_al_hilo{
    int diferencia;
    int puerto;
    int id_nodos[10];
    int prioridad;
};

// VARIABLES UTILES
int mi_ticket = 0;
int mi_id = 0;
int mi_buzon_interno = 0;
int id_nodos_pendientes[100] = {0};
int puertos_pendientes[100] = {0};
int num_pend = 0;
int quiero = 0;
int esperando = 0;
int max_ticket = 0;
int primero = 1;
int repetir = 0;
int esperar = 0;
int semaforo_de_esperar = 0;
int primer_proceso_general = 1;
int consultas_esperando = 0;
int anulaciones_esperando = 0;
int pagos_esperando = 0;
int admin_reser_esperando = 0;
int consultas_dentro = 0;
int antes_de_entrar = 0;
int hizo_requests_consultas = 0;

int permitiendo_adelantamiento_de_ronda = 0;
int no_volver_a_permitir_adelantamientos = 0;
int anulaciones_esperando_a_adelantar = 0;
int pagos_esperando_a_adelantar = 0;
int ultimo_adelantando = 0;
int alguien_dentro = 0;

int peticiones_pendientes_de_consultas[100] = {0};
int oks_a_consultas_ya_mandados = 0;
// VARIABLES UTILES

// VARIABLES PARA HACER ADELANTAMIENTOS
int proceso_prioritario_echa_a_consultas = 0;
// VARIABLES PARA HACER ADELANTAMIENTOS

// EL SEMAFORO
sem_t semaforo_seccion_critica;
// EL SEMAFORO

// SINCRONIZACION
sem_t semaforo_quiero;
sem_t semaforo_ticket;
sem_t semaforo_mi_id;
sem_t semaforo_nodos_pendientes;
sem_t semaforo_num_pendientes;
sem_t semaforo_max_ticket;
sem_t semaforo_primero_dentro;
sem_t semaforo_primero_variable;
sem_t semaforo_no_hacer_requests;
sem_t semaforo_semaforo_de_esperar;
sem_t semaforo_primer_proceso_general;///
sem_t semaforo_consultas_dentro;///
sem_t semaforo_antes_de_entrar;///

sem_t semaforo_anulaciones_adelantamiento_de_ronda;
sem_t semaforo_pagos_adelantamiento_de_ronda;
// SINCRONIZACION

//MENU
sem_t semaforo_para_iniciar_por_hilo[20];
//MENU

// SEMAFOROS DE PRIORIDADES
sem_t semaforo_anulaciones;
sem_t semaforo_pagos;
sem_t semaforo_administracion_reservas;
sem_t semaforo_consultas;
// SEMAFOROS DE PRIORIDADES

void *proceso( void *arg)
{
    int id_aux = 0;
    int i = 0;
    int mi_puerto;
    int mi_prioridad;
    struct mensaje peticion;
    struct mensaje respuesta;
    struct info_a_mandar_al_hilo *aux;
    int longitud_peticion;
    int longitud_respuesta;
    int diferencia = 0;
    int id_nodos[10];
    int max_ticket_empece = 0;
    int mi_ticket_pedi = 0;
    int puedo_adelantar = 0;
    // INICIAR LONGITUD
    longitud_peticion = sizeof(peticion) - sizeof(peticion.tipo);
    longitud_respuesta = sizeof(respuesta) - sizeof(respuesta.tipo);
    // INICAR LONGITUD

    // INICIAR LA INFORMACION DADA POR EL MAIN
    aux = (struct info_a_mandar_al_hilo *)arg;
    diferencia = aux->diferencia;
    mi_prioridad = aux->prioridad;
    for(i=0;i<10;i++){
    id_nodos[i] = aux->id_nodos[i];
    }
    mi_puerto = aux->puerto;
    while (1)
    {
        printf("[%d] Soy un proceso con prioridad: %d\n",mi_puerto,mi_prioridad);

        //menu
        sem_wait(&semaforo_para_iniciar_por_hilo[mi_puerto]);
        //menu

        sem_wait(&semaforo_quiero);
        quiero++;
        printf("quiero++\n");
        sem_post(&semaforo_quiero);
//añadir un if q se active al recibir peticion donde entre los procesos 1 y 2 a esperar a q los adelanten

        //idea para adelantamiento de ronda
        if(permitiendo_adelantamiento_de_ronda==1){
            printf("me permitieron\n");
            if(mi_prioridad==1){
                puedo_adelantar=1;
                anulaciones_esperando_a_adelantar++;
                sem_wait(&semaforo_anulaciones_adelantamiento_de_ronda);
                anulaciones_esperando_a_adelantar--;
                if(anulaciones_esperando_a_adelantar!=0){
                    sem_post(&semaforo_anulaciones_adelantamiento_de_ronda);
                    //printf("salio anu y dejo a otra anu\n");
                }
                else if(pagos_esperando_a_adelantar!=0){
                    sem_post(&semaforo_pagos_adelantamiento_de_ronda);
                    //printf("salio anu y dejo a pago\n");
                }
                else{
                    ultimo_adelantando = 1;
                    //printf("ultimo set por anu\n");
                }
            }
            if(mi_prioridad==2){
                puedo_adelantar=1;
                pagos_esperando_a_adelantar++;
                sem_wait(&semaforo_pagos_adelantamiento_de_ronda);
                pagos_esperando_a_adelantar--;
                if(pagos_esperando_a_adelantar!=0){
                    sem_post(&semaforo_pagos_adelantamiento_de_ronda);
                    //printf("salio pago y dejo a otra pago\n");
                }
                else{
                    ultimo_adelantando = 1;
                    //printf("ultimo set por pago\n");
                }
            }
        }
        //idea para adelantamiento de ronda
        printf("1\n");
        if(puedo_adelantar==0){
        sem_wait(&semaforo_primero_variable);
        printf("2\n");
    if(hizo_requests_consultas==0 || mi_prioridad!=4){
        printf("no me la sudo\n");
        if(primero || (consultas_dentro!=0 && mi_prioridad!=4)){
            if(mi_prioridad==4){
                hizo_requests_consultas=1;
            }
            if(consultas_dentro!=0 && mi_prioridad!=4){//puede dar fallo
                esperar=1;
                repetir=1;
            }
            printf("esperar %d puerto %d\n",esperar,mi_puerto);
            if(esperar){//si el primero no mando los mensajes antes de que le llegue una peticion mamamos (a lo mejor hay que cambiarlo de sitio)
                sem_wait(&semaforo_semaforo_de_esperar);
                semaforo_de_esperar++;
                sem_post(&semaforo_semaforo_de_esperar);
                printf("entre a esperar %d\n",mi_prioridad);
                sem_wait(&semaforo_no_hacer_requests);
                sem_wait(&semaforo_semaforo_de_esperar);
                semaforo_de_esperar--;
                if(semaforo_de_esperar!=0){//esto hace que salgan todos los que esten aqui y vayan al de la seccion critica (creo que no afecta porque solo hay uno)
                    sem_post(&semaforo_no_hacer_requests);
                }
                sem_post(&semaforo_semaforo_de_esperar);
                esperar=0;
            }
            //idea de subir diferente segun la prioridad
            sem_wait(&semaforo_max_ticket);
            sem_wait(&semaforo_ticket);
            max_ticket_empece = max_ticket;
            switch(mi_prioridad){
                case 1:
                    mi_ticket = max_ticket+1;
                    break;
                case 2:
                    mi_ticket = max_ticket+2;
                    break;
                case 3:
                    mi_ticket = max_ticket+3;
                    break;
                case 4:
                    mi_ticket = max_ticket+4;
                    break;
            }
            mi_ticket_pedi = mi_ticket;
            sem_post(&semaforo_ticket);
            sem_post(&semaforo_max_ticket);
            //idea de subir diferente segun la prioridad


            printf("Mi ticket: %i , Max ticket %i, Mi puerto: %d\n", mi_ticket, max_ticket,mi_puerto);
        for (i = 0; i < diferencia; i++)
            {
            sem_wait(&semaforo_ticket);
            sem_wait(&semaforo_max_ticket);

            // INICAR STRUCT PETICION
            peticion.tipo = 1;
            peticion.ticket = mi_ticket;
            peticion.id_nodo = mi_id;
            peticion.puerto = mi_puerto;
            peticion.prioridad = mi_prioridad;
            // INICIAR STRUCT PETICION
            sem_post(&semaforo_max_ticket);
            sem_post(&semaforo_ticket);

            if (msgsnd(id_nodos[i], &peticion, longitud_peticion, 0) == -1) // 1 es no bloqueante
                printf("Error al mandar peticion\n");
            else{
                printf("mande la peticion %d\n",i);
            }
            }
            primero=0;
            sem_wait(&semaforo_primero_dentro);
        }
        
        
        if(ultimo_adelantando==1){/////////////////////
            primero = 1;
            ultimo_adelantando = 0;
            //printf("ultimo usado\n");
        }
    }
        sem_post(&semaforo_primero_variable);
    }
        puedo_adelantar=0;
        esperando++;
        printf("esperando++ %d\n",mi_puerto);
        antes_de_entrar++;

        //PRIORIDADES IDEA 1
        if(primer_proceso_general==0 || (proceso_prioritario_echa_a_consultas==1 && mi_prioridad==4)){//el primer proceso se salta esto
            switch(mi_prioridad){
            case 1:
                anulaciones_esperando++;
                if(consultas_dentro!=0){
                    proceso_prioritario_echa_a_consultas=1;
                }
                sem_wait(&semaforo_anulaciones);
                if(proceso_prioritario_echa_a_consultas==1){
                    proceso_prioritario_echa_a_consultas=0;
                }
                anulaciones_esperando--;
                break;
            case 2:
                pagos_esperando++;
                if(consultas_dentro!=0){
                    proceso_prioritario_echa_a_consultas=1;
                }
                sem_wait(&semaforo_pagos);
                if(proceso_prioritario_echa_a_consultas==1){
                    proceso_prioritario_echa_a_consultas=0;
                }
                pagos_esperando--;
                break;
            case 3:
                admin_reser_esperando++;
                if(consultas_dentro!=0){
                    proceso_prioritario_echa_a_consultas=1;
                }
                sem_wait(&semaforo_administracion_reservas);
                if(proceso_prioritario_echa_a_consultas==1){
                    proceso_prioritario_echa_a_consultas=0;
                }
                admin_reser_esperando--;
                break;
            case 4:
              if(consultas_dentro==0 || proceso_prioritario_echa_a_consultas==1){
                consultas_esperando++;
                sem_wait(&semaforo_consultas);
                consultas_esperando--;
                if(consultas_esperando!=0){
                    sem_post(&semaforo_consultas);//cada consulta saca a la siguiente
                }
              }
                break;
            default:
                break;
            }
        }
        antes_de_entrar--;
        primer_proceso_general = 0;
        //PRIORIDADES IDEA 1
        if(mi_prioridad!=4){//consultas
            sem_wait(&semaforo_seccion_critica);
        }
        else{
            consultas_dentro++;
            if(oks_a_consultas_ya_mandados==0){
                oks_a_consultas_ya_mandados=1;
                respuesta.tipo = 2;
                respuesta.id_nodo = mi_id;
                respuesta.ticket = 0;
                for (i = 0; i < num_pend; i++)
                {               
                    respuesta.puerto = puertos_pendientes[i];
                    if(peticiones_pendientes_de_consultas[i]==1){
                        if (msgsnd(id_nodos_pendientes[i], &respuesta, longitud_respuesta, 1) == -1){ // 1 es no bloqueante
                        printf("Error al mandar OK al nodo %i\n",id_nodos_pendientes[i]);
                        }
                        peticiones_pendientes_de_consultas[i]=2;
                        printf("mando oks a consultas del nodo %d\n",id_nodos_pendientes[i]);
                    }
                }
            }
        }
        ///////////////////////////////añadir q los consultas vean si hay alguna request de consulta esperando pa mandarles ok
        if(no_volver_a_permitir_adelantamientos==0 && permitiendo_adelantamiento_de_ronda==1){
            permitiendo_adelantamiento_de_ronda = 0;
            //printf("anulado\n");
            no_volver_a_permitir_adelantamientos = 1;
        }
        alguien_dentro++;
        // SECCION CRITICA
        printf("***********\n");
        printf("[%i,%d] Dentro de la seccion critica en ronda [%d,%d]\n",mi_puerto,mi_prioridad,mi_ticket_pedi,max_ticket_empece);
        printf("***********\n");
        sleep(20);
        printf("***********\n");
        printf("[%i,%d] Termine\n",mi_puerto,mi_prioridad);
        printf("***********\n\n\n");
        // SECCION CRITICA
        alguien_dentro--;
        if(mi_prioridad!=4){//consultas
            sem_post(&semaforo_seccion_critica);
        }
        else{
            consultas_dentro--;
        }
        esperando--;
        sem_wait(&semaforo_quiero);
        quiero--;
        sem_post(&semaforo_quiero);

        if(anulaciones_esperando_a_adelantar!=0 || pagos_esperando_a_adelantar!=0){
            //printf("else\n");
            if(mi_prioridad==4){
                hizo_requests_consultas=0;
            }
            primero = 0;
            primer_proceso_general = 1;
            if(anulaciones_esperando_a_adelantar!=0){
                //printf("adelantan anulaciones\n");
                sem_post(&semaforo_anulaciones_adelantamiento_de_ronda);
                //printf("done\n");
            }
            else{
                //printf("adelantan pagos\n");
                sem_post(&semaforo_pagos_adelantamiento_de_ronda);
                //printf("done\n");
            }
        }
        else{
//PRIORIDADES IDEA 1
        if(esperando!=0 && consultas_dentro==0){
            if(mi_prioridad==4){
                hizo_requests_consultas=0;
            }
            if(anulaciones_esperando!=0){
                sem_post(&semaforo_anulaciones);
            }
            else if(pagos_esperando!=0)
            {
                sem_post(&semaforo_pagos);
            }
            else if(admin_reser_esperando!=0)
            {
                sem_post(&semaforo_administracion_reservas);
            }
            else    
                sem_post(&semaforo_consultas);
        }
        //PRIORIDADES IDEA 1
        if(esperando==0 && repetir==1){
            if(mi_prioridad==4){
                hizo_requests_consultas=0;
            }
                respuesta.tipo = 2;
                respuesta.id_nodo = mi_id;
                respuesta.ticket = 0;
                    primer_proceso_general=1;
                for (i = 0; i < num_pend; i++)
                {               
                    respuesta.puerto = puertos_pendientes[i];
                    printf(" 1.- id_nodos_pendientes[%i]:%i, peticiones_pendientes_de_consultas[%i]:%i\n",i,id_nodos_pendientes[i],i,peticiones_pendientes_de_consultas[i]);
                    if(peticiones_pendientes_de_consultas[i]!=2){
                        if (msgsnd(id_nodos_pendientes[i], &respuesta, longitud_respuesta, 1) == -1){ // 1 es no bloqueante
                        printf("Error al mandar OK al nodo %i\n",id_nodos_pendientes[i]);
                        }
                        else{
                            printf("mande el ok al nodo %d\n",id_nodos_pendientes[i]);
                        }
                    }
                    peticiones_pendientes_de_consultas[i]=0;
                    printf(" 2.- id_nodos_pendientes[%i]:%i, peticiones_pendientes_de_consultas[%i]:%i\n",i,id_nodos_pendientes[i],i,peticiones_pendientes_de_consultas[i]);
                }
                oks_a_consultas_ya_mandados=0;
                printf("oks a consultas 0\n");
                sleep(1);//>>para poder observar las prioridades
                repetir=0;
                sem_wait(&semaforo_num_pendientes);
                num_pend = 0;
                sem_post(&semaforo_num_pendientes);

                no_volver_a_permitir_adelantamientos=0;//////
                permitiendo_adelantamiento_de_ronda=0;//////
                //printf("acabamos\n");
                sem_post(&semaforo_no_hacer_requests);
        
        }
        }


        if(quiero==0){
            if(mi_prioridad==4){
                hizo_requests_consultas=0;
            }
            primer_proceso_general=1; //creo
            primero=1;
        }
    }
}

void *receptor(void *args){
int id_nodo_origen = 0;
    int ticket_origen = 0;
    int tipo_origen = 0;
    int prioridad_origen = 0;
    int puerto_origen = 0;
    int longitud_peticion;
    int longitud_respuesta;
    int longitud_mensajito = 0;
    int cuenta = 0;
    int diferencia = 0;
    struct mensaje mensaje_recibido;
    struct mensaje respuesta;
    struct info_a_mandar_al_hilo *aux;
    longitud_peticion = sizeof(mensaje_recibido) - sizeof(mensaje_recibido.tipo);
    longitud_respuesta = sizeof(respuesta) - sizeof(respuesta.tipo);

    aux = (struct info_a_mandar_al_hilo *)args;
    diferencia = aux->diferencia;
    //printf("diferencia en receptor: %d\n",diferencia);

    while (1)
    {
        // RECIBIR MENSAJE AJENA

        if (msgrcv(mi_id, &mensaje_recibido, longitud_peticion, 0, 0) == -1) // RECIBE TODOS LOS MENSAJES QUE LE LLEGUEN
        {                           
            printf("Error al recibir peticion\n");
        }
        srand(mi_id);
        sleep(rand() % 4);
        tipo_origen = mensaje_recibido.tipo;
        ticket_origen = mensaje_recibido.ticket;
        id_nodo_origen = mensaje_recibido.id_nodo;
        puerto_origen = mensaje_recibido.puerto;
        prioridad_origen = mensaje_recibido.prioridad;

        // RECIBIR PETICION AJENA
        if (tipo_origen == 1)
        { // ESTO ES QUE RECIBIO UNA REQUEST
            sem_wait(&semaforo_max_ticket);
            if (max_ticket < ticket_origen)
            {
                max_ticket = ticket_origen;
            }
            printf("Valor max del ticket: %i\n", max_ticket);

            sem_post(&semaforo_max_ticket);

            sem_wait(&semaforo_quiero);
            sem_wait(&semaforo_ticket);
            sem_wait(&semaforo_mi_id);
            sem_wait(&semaforo_num_pendientes);
            sem_wait(&semaforo_nodos_pendientes);

            printf("Quiero: %i\n",quiero);
            printf("Mi ticket: %i, ticket origen: %i\n",mi_ticket,ticket_origen);
            printf("Mi id: %i, id origen: %i\n",mi_id,id_nodo_origen);

            if (quiero==0 || (ticket_origen < mi_ticket) || (ticket_origen == mi_ticket && (id_nodo_origen < mi_id)) || (prioridad_origen==4 && consultas_dentro!=0 && num_pend==0 && antes_de_entrar==0))//esto pa varias consultas en diferentes nodos
            {
                if(no_volver_a_permitir_adelantamientos==0 && alguien_dentro==0 && quiero!=0){
                    printf("activado\n");
                    permitiendo_adelantamiento_de_ronda = 1;
                }
                respuesta.id_nodo = mi_id;
                respuesta.tipo = 2;
                respuesta.puerto = puerto_origen;
                respuesta.ticket = mi_ticket;
                if (msgsnd(id_nodo_origen, &respuesta, longitud_respuesta, 1) == -1) // 1 es no bloqueante
                    printf("Error al mandar ok\n");
                else
                    printf("Mande el OK al nodo: %i y puerto: %i\n",id_nodo_origen,puerto_origen);
            }
            else
            {
                primero = 1;///
                repetir = 1;///
                esperar = 1;///
                printf("esperar++\n");
                if(no_volver_a_permitir_adelantamientos==0 && alguien_dentro==0){
                    permitiendo_adelantamiento_de_ronda = 1;
                }
                id_nodos_pendientes[num_pend] = id_nodo_origen;
                puertos_pendientes[num_pend] = puerto_origen;//////se puede quitar
                if(prioridad_origen==4){
                    peticiones_pendientes_de_consultas[num_pend] = 1;
                    oks_a_consultas_ya_mandados=0;
                    printf("anoto consulta del nodo %d\n",id_nodo_origen);
                }
                num_pend++;
                printf("Rechace mandar el OK al nodo: %i y puerto: %i\n",id_nodo_origen,puerto_origen);
            }
            sem_post(&semaforo_nodos_pendientes);
            sem_post(&semaforo_num_pendientes);
            sem_post(&semaforo_mi_id);
            sem_post(&semaforo_ticket);
            sem_post(&semaforo_quiero);
        }
        else{
            printf("OK recibido del nodo: %d\n",id_nodo_origen);
            cuenta++;
            if(cuenta==(diferencia)){//ya recibio todos los oks
                cuenta=0;
                sem_post(&semaforo_primero_dentro);
            }
        }
    }
}

void main(int argc, char *args[])
{
    struct info_a_mandar_al_hilo info;
    int i = 0;
    int numero_procesos = 0;
    int minimo = 0;
    int maximo = 0;
    int id_nodos[10];

    pthread_t thread_receptor;
    int diferencia;

    // INICIAR VARIABLES POR SCANF
    printf("Cual es mi nodo: ");
    scanf("%i",&mi_id);
    printf("ID minimo de los buzones: ");
    scanf("%i",&minimo);
    printf("ID maximo de los buzones: ");
    scanf("%i",&maximo);
    printf("Cuantos procesos en este nodo: ");
    scanf("%i",&numero_procesos);
    mi_buzon_interno = mi_id + 1;
    // INICIAR VARIABLES POR SCANF

    // INICIAR SEMAFOROS
    sem_init(&semaforo_quiero, 0, 1);
    sem_init(&semaforo_ticket, 0, 1);
    sem_init(&semaforo_mi_id, 0, 1);
    sem_init(&semaforo_nodos_pendientes, 0, 1);
    sem_init(&semaforo_num_pendientes, 0, 1);
    sem_init(&semaforo_max_ticket, 0, 1);
    sem_init(&semaforo_seccion_critica, 0, 1);
    sem_init(&semaforo_primero_variable, 0, 1);
    sem_init(&semaforo_primero_dentro, 0, 0);
    sem_init(&semaforo_no_hacer_requests, 0, 0);
    sem_init(&semaforo_anulaciones, 0, 0);
    sem_init(&semaforo_administracion_reservas, 0, 0);
    sem_init(&semaforo_pagos, 0, 0);
    sem_init(&semaforo_consultas, 0, 0);
    sem_init(&semaforo_semaforo_de_esperar, 0, 1);
    sem_init(&semaforo_anulaciones_adelantamiento_de_ronda, 0, 0);
    sem_init(&semaforo_pagos_adelantamiento_de_ronda, 0, 0);

    for(i=0;i<10;i++){
        sem_init(&semaforo_para_iniciar_por_hilo[i], 0, 0);
    }
    // INICIAR SEMAFOROS

    // INICIAR ID_NODOS
    int a = 0;
    for (i = minimo; i <= maximo; i++)
    {
        if (i != mi_id)
        {
            id_nodos[a] = i;
            a++;
        }
    }
    diferencia = (maximo - minimo); //buzon 0 es de nodo 0 buzon 1 es el buzon interno del nodo 0
    // INICIAR ID_NODOS

    // INICIAR THREADS
    info.diferencia = diferencia;
    for(a = 0; a<10; a++){
    info.id_nodos[a] = id_nodos[a];
    }
    for(a = 0; a<numero_procesos; a++){
        info.puerto = a+1;
        if(a<4){
            info.prioridad = 1;
        }
        /*else if(a<8 && a>=4){
            info.prioridad = 2;
        }
        else if(a<12 && a>=8){
            info.prioridad = 3;
        }*/
        else{
            info.prioridad = 4;
        }
        pthread_create(&thread_receptor, NULL, proceso, (void *)&info);
        sleep(1);
    }
    pthread_create(&thread_receptor, NULL, receptor, (void *)&info);//esto crea el receptor y le pasa la struct con la diferencia
    // INICIAR THREADS

///////////////////////////////////////////////////////////////////////////////hacer menuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
int opcion=1;
while(opcion!=0){
    printf("Que hilo intenta entrar? ");
    printf("[");
    for(i=1;i<(numero_procesos+1);i++){
        printf("%d",i);
    }
    printf("]\n");
    scanf("%d",&opcion);
    sem_post(&semaforo_para_iniciar_por_hilo[opcion]);
    }
}