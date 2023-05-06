// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/ipc.h>
// #include <sys/msg.h>
// #include <sys/types.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>

// struct mensaje_buzon_interno{
//     long tipo;
// };

// struct mensaje
// {
//     long tipo;
//     int ticket;
//     int id_nodo;
//     int puerto;
//     int prioridad;
// };

// struct info_a_mandar_al_hilo{
//     int diferencia;
//     int puerto;
//     int id_nodos[10];
//     int prioridad;
// };

// // VARIABLES UTILES
// int mi_ticket = 0;
// int mi_id = 0;
// int mi_buzon_interno = 0;
// int id_nodos_pendientes[100] = {0};
// int puertos_pendientes[100] = {0};
// int num_pend = 0;
// int quiero = 0;
// int esperando = 0;
// int max_ticket = 0;
// int primero = 1;
// int repetir = 0;
// int esperar = 0;
// int semaforo_de_esperar = 0;
// int primer_proceso_general = 1;
// int consultas_esperando = 0;
// int anulaciones_esperando = 0;
// int pagos_esperando = 0;
// int admin_reser_esperando = 0;
// int consultas_dentro = 0;
// int antes_de_entrar = 0;
// int hizo_requests_consultas = 0;

// int permitiendo_adelantamiento_de_ronda = 0;
// int no_volver_a_permitir_adelantamientos = 0;
// int anulaciones_esperando_a_adelantar = 0;
// int pagos_esperando_a_adelantar = 0;
// int ultimo_adelantando = 0;
// int alguien_dentro = 0;

// int peticiones_pendientes_de_consultas[100] = {0};
// int oks_a_consultas_ya_mandados = 0;
// // VARIABLES UTILES

// // VARIABLES PARA HACER ADELANTAMIENTOS
// int proceso_prioritario_echa_a_consultas = 0;
// // VARIABLES PARA HACER ADELANTAMIENTOS

// // EL SEMAFORO
// sem_t semaforo_seccion_critica;
// // EL SEMAFORO

// // SINCRONIZACION
// sem_t semaforo_quiero;
// sem_t semaforo_ticket;
// sem_t semaforo_mi_id;
// sem_t semaforo_nodos_pendientes;
// sem_t semaforo_num_pendientes;
// sem_t semaforo_max_ticket;
// sem_t semaforo_primero_dentro;
// sem_t semaforo_primero_variable;
// sem_t semaforo_no_hacer_requests;
// sem_t semaforo_semaforo_de_esperar;
// sem_t semaforo_primer_proceso_general;///
// sem_t semaforo_consultas_dentro;///
// sem_t semaforo_antes_de_entrar;///

// sem_t semaforo_anulaciones_adelantamiento_de_ronda;
// sem_t semaforo_pagos_adelantamiento_de_ronda;
// // SINCRONIZACION

// //MENU
// sem_t semaforo_para_iniciar_por_hilo[20];
// //MENU

// // SEMAFOROS DE PRIORIDADES
// sem_t semaforo_anulaciones;
// sem_t semaforo_pagos;
// sem_t semaforo_administracion_reservas;
// sem_t semaforo_consultas;
// // SEMAFOROS DE PRIORIDADES

// void *proceso( void *arg)
// {
//     int id_aux = 0;
//     int i = 0;
//     int mi_puerto;
//     int mi_prioridad;
//     struct mensaje peticion;
//     struct mensaje respuesta;
//     struct info_a_mandar_al_hilo *aux;
//     int longitud_peticion;
//     int longitud_respuesta;
//     int diferencia = 0;
//     int id_nodos[10];
//     int max_ticket_empece = 0;
//     int mi_ticket_pedi = 0;
//     int puedo_adelantar = 0;
//     // INICIAR LONGITUD
//     longitud_peticion = sizeof(peticion) - sizeof(peticion.tipo);
//     longitud_respuesta = sizeof(respuesta) - sizeof(respuesta.tipo);
//     // INICAR LONGITUD

//     // INICIAR LA INFORMACION DADA POR EL MAIN
//     aux = (struct info_a_mandar_al_hilo *)arg;
//     diferencia = aux->diferencia;
//     mi_prioridad = aux->prioridad;
//     for(i=0;i<10;i++){
//     id_nodos[i] = aux->id_nodos[i];
//     }
//     mi_puerto = aux->puerto;
//     while (1)
//     {
//         printf("[%d] Soy un proceso con prioridad: %d\n",mi_puerto,mi_prioridad);

//         //menu
//         sem_wait(&semaforo_para_iniciar_por_hilo[mi_puerto]);
//         //menu

//         sem_wait(&semaforo_quiero);
//         quiero++;
//         printf("quiero++\n");
//         sem_post(&semaforo_quiero);
// //añadir un if q se active al recibir peticion donde entre los procesos 1 y 2 a esperar a q los adelanten

//         //idea para adelantamiento de ronda
//         if(permitiendo_adelantamiento_de_ronda==1){
//             printf("me permitieron\n");
//             if(mi_prioridad==1){
//                 puedo_adelantar=1;
//                 anulaciones_esperando_a_adelantar++;
//                 sem_wait(&semaforo_anulaciones_adelantamiento_de_ronda);
//                 anulaciones_esperando_a_adelantar--;
//                 if(anulaciones_esperando_a_adelantar!=0){
//                     sem_post(&semaforo_anulaciones_adelantamiento_de_ronda);
//                     //printf("salio anu y dejo a otra anu\n");
//                 }
//                 else if(pagos_esperando_a_adelantar!=0){
//                     sem_post(&semaforo_pagos_adelantamiento_de_ronda);
//                     //printf("salio anu y dejo a pago\n");
//                 }
//                 else{
//                     ultimo_adelantando = 1;
//                     //printf("ultimo set por anu\n");
//                 }
//             }
//             if(mi_prioridad==2){
//                 puedo_adelantar=1;
//                 pagos_esperando_a_adelantar++;
//                 sem_wait(&semaforo_pagos_adelantamiento_de_ronda);
//                 pagos_esperando_a_adelantar--;
//                 if(pagos_esperando_a_adelantar!=0){
//                     sem_post(&semaforo_pagos_adelantamiento_de_ronda);
//                     //printf("salio pago y dejo a otra pago\n");
//                 }
//                 else{
//                     ultimo_adelantando = 1;
//                     //printf("ultimo set por pago\n");
//                 }
//             }
//         }
//         //idea para adelantamiento de ronda
//         printf("1\n");
//         if(puedo_adelantar==0){
//         sem_wait(&semaforo_primero_variable);
//         printf("2\n");
//     if(hizo_requests_consultas==0 || mi_prioridad!=4){
//         printf("no me la sudo\n");
//         if(primero || (consultas_dentro!=0 && mi_prioridad!=4)){
//             if(mi_prioridad==4){
//                 hizo_requests_consultas=1;
//             }
//             if(consultas_dentro!=0 && mi_prioridad!=4){//puede dar fallo
//                 esperar=1;
//                 repetir=1;
//             }
//             printf("esperar %d puerto %d\n",esperar,mi_puerto);
//             if(esperar){//si el primero no mando los mensajes antes de que le llegue una peticion mamamos (a lo mejor hay que cambiarlo de sitio)
//                 sem_wait(&semaforo_semaforo_de_esperar);
//                 semaforo_de_esperar++;
//                 sem_post(&semaforo_semaforo_de_esperar);
//                 printf("entre a esperar %d\n",mi_prioridad);
//                 sem_wait(&semaforo_no_hacer_requests);
//                 sem_wait(&semaforo_semaforo_de_esperar);
//                 semaforo_de_esperar--;
//                 if(semaforo_de_esperar!=0){//esto hace que salgan todos los que esten aqui y vayan al de la seccion critica (creo que no afecta porque solo hay uno)
//                     sem_post(&semaforo_no_hacer_requests);
//                 }
//                 sem_post(&semaforo_semaforo_de_esperar);
//                 esperar=0;
//             }
//             //idea de subir diferente segun la prioridad
//             sem_wait(&semaforo_max_ticket);
//             sem_wait(&semaforo_ticket);
//             max_ticket_empece = max_ticket;
//             switch(mi_prioridad){
//                 case 1:
//                     mi_ticket = max_ticket+1;
//                     break;
//                 case 2:
//                     mi_ticket = max_ticket+2;
//                     break;
//                 case 3:
//                     mi_ticket = max_ticket+3;
//                     break;
//                 case 4:
//                     mi_ticket = max_ticket+4;
//                     break;
//             }
//             mi_ticket_pedi = mi_ticket;
//             sem_post(&semaforo_ticket);
//             sem_post(&semaforo_max_ticket);
//             //idea de subir diferente segun la prioridad


//             printf("Mi ticket: %i , Max ticket %i, Mi puerto: %d\n", mi_ticket, max_ticket,mi_puerto);
//         for (i = 0; i < diferencia; i++)
//             {
//             sem_wait(&semaforo_ticket);
//             sem_wait(&semaforo_max_ticket);

//             // INICAR STRUCT PETICION
//             peticion.tipo = 1;
//             peticion.ticket = mi_ticket;
//             peticion.id_nodo = mi_id;
//             peticion.puerto = mi_puerto;
//             peticion.prioridad = mi_prioridad;
//             // INICIAR STRUCT PETICION
//             sem_post(&semaforo_max_ticket);
//             sem_post(&semaforo_ticket);

//             if (msgsnd(id_nodos[i], &peticion, longitud_peticion, 0) == -1) // 1 es no bloqueante
//                 printf("Error al mandar peticion\n");
//             else{
//                 printf("mande la peticion %d\n",i);
//             }
//             }
//             primero=0;
//             sem_wait(&semaforo_primero_dentro);
//         }
        
        
//         if(ultimo_adelantando==1){/////////////////////
//             primero = 1;
//             ultimo_adelantando = 0;
//             //printf("ultimo usado\n");
//         }
//     }
//         sem_post(&semaforo_primero_variable);
//     }
//         puedo_adelantar=0;
//         esperando++;
//         printf("esperando++ %d\n",mi_puerto);
//         antes_de_entrar++;

//         //PRIORIDADES IDEA 1
//         if(primer_proceso_general==0 || (proceso_prioritario_echa_a_consultas==1 && mi_prioridad==4)){//el primer proceso se salta esto
//             switch(mi_prioridad){
//             case 1:
//                 anulaciones_esperando++;
//                 if(consultas_dentro!=0){
//                     proceso_prioritario_echa_a_consultas=1;
//                 }
//                 sem_wait(&semaforo_anulaciones);
//                 if(proceso_prioritario_echa_a_consultas==1){
//                     proceso_prioritario_echa_a_consultas=0;
//                 }
//                 anulaciones_esperando--;
//                 break;
//             case 2:
//                 pagos_esperando++;
//                 if(consultas_dentro!=0){
//                     proceso_prioritario_echa_a_consultas=1;
//                 }
//                 sem_wait(&semaforo_pagos);
//                 if(proceso_prioritario_echa_a_consultas==1){
//                     proceso_prioritario_echa_a_consultas=0;
//                 }
//                 pagos_esperando--;
//                 break;
//             case 3:
//                 admin_reser_esperando++;
//                 if(consultas_dentro!=0){
//                     proceso_prioritario_echa_a_consultas=1;
//                 }
//                 sem_wait(&semaforo_administracion_reservas);
//                 if(proceso_prioritario_echa_a_consultas==1){
//                     proceso_prioritario_echa_a_consultas=0;
//                 }
//                 admin_reser_esperando--;
//                 break;
//             case 4:
//               if(consultas_dentro==0 || proceso_prioritario_echa_a_consultas==1){
//                 consultas_esperando++;
//                 sem_wait(&semaforo_consultas);
//                 consultas_esperando--;
//                 if(consultas_esperando!=0){
//                     sem_post(&semaforo_consultas);//cada consulta saca a la siguiente
//                 }
//               }
//                 break;
//             default:
//                 break;
//             }
//         }
//         antes_de_entrar--;
//         primer_proceso_general = 0;
//         //PRIORIDADES IDEA 1
//         if(mi_prioridad!=4){//consultas
//             sem_wait(&semaforo_seccion_critica);
//         }
//         else{
//             consultas_dentro++;
//             if(oks_a_consultas_ya_mandados==0){
//                 oks_a_consultas_ya_mandados=1;
//                 respuesta.tipo = 2;
//                 respuesta.id_nodo = mi_id;
//                 respuesta.ticket = 0;
//                 for (i = 0; i < num_pend; i++)
//                 {               
//                     respuesta.puerto = puertos_pendientes[i];
//                     if(peticiones_pendientes_de_consultas[i]==1){
//                         if (msgsnd(id_nodos_pendientes[i], &respuesta, longitud_respuesta, 1) == -1){ // 1 es no bloqueante
//                         printf("Error al mandar OK al nodo %i\n",id_nodos_pendientes[i]);
//                         }
//                         peticiones_pendientes_de_consultas[i]=2;
//                         printf("mando oks a consultas del nodo %d\n",id_nodos_pendientes[i]);
//                     }
//                 }
//             }
//         }
//         ///////////////////////////////añadir q los consultas vean si hay alguna request de consulta esperando pa mandarles ok
//         if(no_volver_a_permitir_adelantamientos==0 && permitiendo_adelantamiento_de_ronda==1){
//             permitiendo_adelantamiento_de_ronda = 0;
//             //printf("anulado\n");
//             no_volver_a_permitir_adelantamientos = 1;
//         }
//         alguien_dentro++;
//         // SECCION CRITICA
//         printf("***********\n");
//         printf("[%i,%d] Dentro de la seccion critica en ronda [%d,%d]\n",mi_puerto,mi_prioridad,mi_ticket_pedi,max_ticket_empece);
//         printf("***********\n");
//         sleep(20);
//         printf("***********\n");
//         printf("[%i,%d] Termine\n",mi_puerto,mi_prioridad);
//         printf("***********\n\n\n");
//         // SECCION CRITICA
//         alguien_dentro--;
//         if(mi_prioridad!=4){//consultas
//             sem_post(&semaforo_seccion_critica);
//         }
//         else{
//             consultas_dentro--;
//         }
//         esperando--;
//         sem_wait(&semaforo_quiero);
//         quiero--;
//         sem_post(&semaforo_quiero);

//         if(anulaciones_esperando_a_adelantar!=0 || pagos_esperando_a_adelantar!=0){
//             //printf("else\n");
//             if(mi_prioridad==4){
//                 hizo_requests_consultas=0;
//             }
//             primero = 0;
//             primer_proceso_general = 1;
//             if(anulaciones_esperando_a_adelantar!=0){
//                 //printf("adelantan anulaciones\n");
//                 sem_post(&semaforo_anulaciones_adelantamiento_de_ronda);
//                 //printf("done\n");
//             }
//             else{
//                 //printf("adelantan pagos\n");
//                 sem_post(&semaforo_pagos_adelantamiento_de_ronda);
//                 //printf("done\n");
//             }
//         }
//         else{
// //PRIORIDADES IDEA 1
//         if(esperando!=0 && consultas_dentro==0){
//             if(mi_prioridad==4){
//                 hizo_requests_consultas=0;
//             }
//             if(anulaciones_esperando!=0){
//                 sem_post(&semaforo_anulaciones);
//             }
//             else if(pagos_esperando!=0)
//             {
//                 sem_post(&semaforo_pagos);
//             }
//             else if(admin_reser_esperando!=0)
//             {
//                 sem_post(&semaforo_administracion_reservas);
//             }
//             else    
//                 sem_post(&semaforo_consultas);
//         }
//         //PRIORIDADES IDEA 1
//         if(esperando==0 && repetir==1){
//             if(mi_prioridad==4){
//                 hizo_requests_consultas=0;
//             }
//                 respuesta.tipo = 2;
//                 respuesta.id_nodo = mi_id;
//                 respuesta.ticket = 0;
//                     primer_proceso_general=1;
//                 for (i = 0; i < num_pend; i++)
//                 {               
//                     respuesta.puerto = puertos_pendientes[i];
//                     printf(" 1.- id_nodos_pendientes[%i]:%i, peticiones_pendientes_de_consultas[%i]:%i\n",i,id_nodos_pendientes[i],i,peticiones_pendientes_de_consultas[i]);
//                     if(peticiones_pendientes_de_consultas[i]!=2){
//                         if (msgsnd(id_nodos_pendientes[i], &respuesta, longitud_respuesta, 1) == -1){ // 1 es no bloqueante
//                         printf("Error al mandar OK al nodo %i\n",id_nodos_pendientes[i]);
//                         }
//                         else{
//                             printf("mande el ok al nodo %d\n",id_nodos_pendientes[i]);
//                         }
//                     }
//                     peticiones_pendientes_de_consultas[i]=0;
//                     printf(" 2.- id_nodos_pendientes[%i]:%i, peticiones_pendientes_de_consultas[%i]:%i\n",i,id_nodos_pendientes[i],i,peticiones_pendientes_de_consultas[i]);
//                 }
//                 oks_a_consultas_ya_mandados=0;
//                 printf("oks a consultas 0\n");
//                 sleep(1);//>>para poder observar las prioridades
//                 repetir=0;
//                 sem_wait(&semaforo_num_pendientes);
//                 num_pend = 0;
//                 sem_post(&semaforo_num_pendientes);

//                 no_volver_a_permitir_adelantamientos=0;//////
//                 permitiendo_adelantamiento_de_ronda=0;//////
//                 //printf("acabamos\n");
//                 sem_post(&semaforo_no_hacer_requests);
        
//         }
//         }


//         if(quiero==0){
//             if(mi_prioridad==4){
//                 hizo_requests_consultas=0;
//             }
//             primer_proceso_general=1; //creo
//             primero=1;
//         }
//     }
// }

// void *receptor(void *args){
// int id_nodo_origen = 0;
//     int ticket_origen = 0;
//     int tipo_origen = 0;
//     int prioridad_origen = 0;
//     int puerto_origen = 0;
//     int longitud_peticion;
//     int longitud_respuesta;
//     int longitud_mensajito = 0;
//     int cuenta = 0;
//     int diferencia = 0;
//     struct mensaje mensaje_recibido;
//     struct mensaje respuesta;
//     struct info_a_mandar_al_hilo *aux;
//     longitud_peticion = sizeof(mensaje_recibido) - sizeof(mensaje_recibido.tipo);
//     longitud_respuesta = sizeof(respuesta) - sizeof(respuesta.tipo);

//     aux = (struct info_a_mandar_al_hilo *)args;
//     diferencia = aux->diferencia;
//     //printf("diferencia en receptor: %d\n",diferencia);

//     while (1)
//     {
//         // RECIBIR MENSAJE AJENA

//         if (msgrcv(mi_id, &mensaje_recibido, longitud_peticion, 0, 0) == -1) // RECIBE TODOS LOS MENSAJES QUE LE LLEGUEN
//         {                           
//             printf("Error al recibir peticion\n");
//         }
//         srand(mi_id);
//         sleep(rand() % 4);
//         tipo_origen = mensaje_recibido.tipo;
//         ticket_origen = mensaje_recibido.ticket;
//         id_nodo_origen = mensaje_recibido.id_nodo;
//         puerto_origen = mensaje_recibido.puerto;
//         prioridad_origen = mensaje_recibido.prioridad;

//         // RECIBIR PETICION AJENA
//         if (tipo_origen == 1)
//         { // ESTO ES QUE RECIBIO UNA REQUEST
//             sem_wait(&semaforo_max_ticket);
//             if (max_ticket < ticket_origen)
//             {
//                 max_ticket = ticket_origen;
//             }
//             printf("Valor max del ticket: %i\n", max_ticket);

//             sem_post(&semaforo_max_ticket);

//             sem_wait(&semaforo_quiero);
//             sem_wait(&semaforo_ticket);
//             sem_wait(&semaforo_mi_id);
//             sem_wait(&semaforo_num_pendientes);
//             sem_wait(&semaforo_nodos_pendientes);

//             printf("Quiero: %i\n",quiero);
//             printf("Mi ticket: %i, ticket origen: %i\n",mi_ticket,ticket_origen);
//             printf("Mi id: %i, id origen: %i\n",mi_id,id_nodo_origen);

//             if (quiero==0 || (ticket_origen < mi_ticket) || (ticket_origen == mi_ticket && (id_nodo_origen < mi_id)) || (prioridad_origen==4 && consultas_dentro!=0 && num_pend==0 && antes_de_entrar==0))//esto pa varias consultas en diferentes nodos
//             {
//                 if(no_volver_a_permitir_adelantamientos==0 && alguien_dentro==0 && quiero!=0){
//                     printf("activado\n");
//                     permitiendo_adelantamiento_de_ronda = 1;
//                 }
//                 respuesta.id_nodo = mi_id;
//                 respuesta.tipo = 2;
//                 respuesta.puerto = puerto_origen;
//                 respuesta.ticket = mi_ticket;
//                 if (msgsnd(id_nodo_origen, &respuesta, longitud_respuesta, 1) == -1) // 1 es no bloqueante
//                     printf("Error al mandar ok\n");
//                 else
//                     printf("Mande el OK al nodo: %i y puerto: %i\n",id_nodo_origen,puerto_origen);
//             }
//             else
//             {
//                 primero = 1;///
//                 repetir = 1;///
//                 esperar = 1;///
//                 printf("esperar++\n");
//                 if(no_volver_a_permitir_adelantamientos==0 && alguien_dentro==0){
//                     permitiendo_adelantamiento_de_ronda = 1;
//                 }
//                 id_nodos_pendientes[num_pend] = id_nodo_origen;
//                 puertos_pendientes[num_pend] = puerto_origen;//////se puede quitar
//                 if(prioridad_origen==4){
//                     peticiones_pendientes_de_consultas[num_pend] = 1;
//                     oks_a_consultas_ya_mandados=0;
//                     printf("anoto consulta del nodo %d\n",id_nodo_origen);
//                 }
//                 num_pend++;
//                 printf("Rechace mandar el OK al nodo: %i y puerto: %i\n",id_nodo_origen,puerto_origen);
//             }
//             sem_post(&semaforo_nodos_pendientes);
//             sem_post(&semaforo_num_pendientes);
//             sem_post(&semaforo_mi_id);
//             sem_post(&semaforo_ticket);
//             sem_post(&semaforo_quiero);
//         }
//         else{
//             printf("OK recibido del nodo: %d\n",id_nodo_origen);
//             cuenta++;
//             if(cuenta==(diferencia)){//ya recibio todos los oks
//                 cuenta=0;
//                 sem_post(&semaforo_primero_dentro);
//             }
//         }
//     }
// }

// void main(int argc, char *args[])
// {
//     struct info_a_mandar_al_hilo info;
//     int i = 0;
//     int numero_procesos = 0;
//     int minimo = 0;
//     int maximo = 0;
//     int id_nodos[10];

//     pthread_t thread_receptor;
//     int diferencia;

//     // INICIAR VARIABLES POR SCANF
//     printf("Cual es mi nodo: ");
//     scanf("%i",&mi_id);
//     printf("ID minimo de los buzones: ");
//     scanf("%i",&minimo);
//     printf("ID maximo de los buzones: ");
//     scanf("%i",&maximo);
//     printf("Cuantos procesos en este nodo: ");
//     scanf("%i",&numero_procesos);
//     mi_buzon_interno = mi_id + 1;
//     // INICIAR VARIABLES POR SCANF

//     // INICIAR SEMAFOROS
//     sem_init(&semaforo_quiero, 0, 1);
//     sem_init(&semaforo_ticket, 0, 1);
//     sem_init(&semaforo_mi_id, 0, 1);
//     sem_init(&semaforo_nodos_pendientes, 0, 1);
//     sem_init(&semaforo_num_pendientes, 0, 1);
//     sem_init(&semaforo_max_ticket, 0, 1);
//     sem_init(&semaforo_seccion_critica, 0, 1);
//     sem_init(&semaforo_primero_variable, 0, 1);
//     sem_init(&semaforo_primero_dentro, 0, 0);
//     sem_init(&semaforo_no_hacer_requests, 0, 0);
//     sem_init(&semaforo_anulaciones, 0, 0);
//     sem_init(&semaforo_administracion_reservas, 0, 0);
//     sem_init(&semaforo_pagos, 0, 0);
//     sem_init(&semaforo_consultas, 0, 0);
//     sem_init(&semaforo_semaforo_de_esperar, 0, 1);
//     sem_init(&semaforo_anulaciones_adelantamiento_de_ronda, 0, 0);
//     sem_init(&semaforo_pagos_adelantamiento_de_ronda, 0, 0);

//     for(i=0;i<10;i++){
//         sem_init(&semaforo_para_iniciar_por_hilo[i], 0, 0);
//     }
//     // INICIAR SEMAFOROS

//     // INICIAR ID_NODOS
//     int a = 0;
//     for (i = minimo; i <= maximo; i++)
//     {
//         if (i != mi_id)
//         {
//             id_nodos[a] = i;
//             a++;
//         }
//     }
//     diferencia = (maximo - minimo); //buzon 0 es de nodo 0 buzon 1 es el buzon interno del nodo 0
//     // INICIAR ID_NODOS

//     // INICIAR THREADS
//     info.diferencia = diferencia;
//     for(a = 0; a<10; a++){
//     info.id_nodos[a] = id_nodos[a];
//     }
//     for(a = 0; a<numero_procesos; a++){
//         info.puerto = a+1;
//         if(a<4){
//             info.prioridad = 1;
//         }
//         /*else if(a<8 && a>=4){
//             info.prioridad = 2;
//         }
//         else if(a<12 && a>=8){
//             info.prioridad = 3;
//         }*/
//         else{
//             info.prioridad = 4;
//         }
//         pthread_create(&thread_receptor, NULL, proceso, (void *)&info);
//         sleep(1);
//     }
//     pthread_create(&thread_receptor, NULL, receptor, (void *)&info);//esto crea el receptor y le pasa la struct con la diferencia
//     // INICIAR THREADS

// ///////////////////////////////////////////////////////////////////////////////hacer menuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
// int opcion=1;
// while(opcion!=0){
//     printf("Que hilo intenta entrar? ");
//     printf("[");
//     for(i=1;i<(numero_procesos+1);i++){
//         printf("%d",i);
//     }
//     printf("]\n");
//     scanf("%d",&opcion);
//     sem_post(&semaforo_para_iniciar_por_hilo[opcion]);
//     }
// }



// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/ipc.h>
// #include <sys/msg.h>
// #include <sys/types.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>
// #include <ctype.h>
// #include <time.h>

// #define MAX_INSTR_LENGTH 1000

// struct mensaje
// {
//     long tipo;
//     int ticket;
//     int id_nodo;
//     int puerto;
//     long prioridad;
// };

// struct info_a_mandar_al_hilo{
//     int diferencia;
//     int puerto;
//     int id_nodos[10];
//     int prioridad;
// };

// // VARIABLES UTILES
// int mi_ticket = 0;
// int mi_id = 0;
// int mi_buzon_interno = 0;
// int id_nodos_pendientes[100] = {0};
// int puertos_pendientes[100] = {0};
// int num_pend = 0;
// int quiero = 0;
// int esperando = 0;
// int max_ticket = 0;
// int primero = 1;
// int repetir = 0;
// int esperar = 0;
// int semaforo_de_esperar = 0;
// int primer_proceso_general = 1;
// int consultas_esperando = 0;
// int anulaciones_esperando = 0;
// int pagos_esperando = 0;
// int admin_reser_esperando = 0;
// int consultas_dentro = 0;
// int antes_de_entrar = 0;
// int hizo_requests_consultas = 0;

// int permitiendo_adelantamiento_de_ronda = 0;
// int no_volver_a_permitir_adelantamientos = 0;
// int anulaciones_esperando_a_adelantar = 0;
// int pagos_esperando_a_adelantar = 0;
// int ultimo_adelantando = 0;
// int alguien_dentro = 0;

// int peticiones_pendientes_de_consultas[100] = {0};
// int oks_a_consultas_ya_mandados = 0;
// // VARIABLES UTILES

// //CAMBIO DE VARIABLES
// int id_nodos[10];
// int diferencia;
// int puerto = 1;
// //CAMBIO DE VARIABLES

// // VARIABLES PARA HACER ADELANTAMIENTOS
// int proceso_prioritario_echa_a_consultas = 0;
// // VARIABLES PARA HACER ADELANTAMIENTOS

// // EL SEMAFORO
// sem_t semaforo_seccion_critica;
// // EL SEMAFORO

// // SINCRONIZACION
// sem_t semaforo_quiero;
// sem_t semaforo_ticket;
// sem_t semaforo_mi_id;
// sem_t semaforo_nodos_pendientes;
// sem_t semaforo_num_pendientes;
// sem_t semaforo_max_ticket;
// sem_t semaforo_primero_dentro;
// sem_t semaforo_primero_variable;
// sem_t semaforo_no_hacer_requests;
// sem_t semaforo_semaforo_de_esperar;
// sem_t semaforo_primer_proceso_general;
// sem_t semaforo_consultas_dentro;
// sem_t semaforo_antes_de_entrar;
// sem_t semaforo_primero;
// sem_t semaforo_repetir;
// sem_t semaforo_esperar;
// sem_t semaforo_consultas_esperando;
// sem_t semaforo_hizo_requests_consultas;
// sem_t semaforo_permitiendo_adelantamiento_de_ronda;
// sem_t semaforo_no_volver_a_permitir_adelantamientos;
// sem_t semaforo_anulaciones_esperando_a_adelantar;
// sem_t semaforo_pagos_esperando_a_adelantar;
// sem_t semaforo_ultimo_adelantando;
// sem_t semaforo_alguien_dentro;
// sem_t semaforo_oks_a_consultas_ya_mandados;
// sem_t semaforo_esperando;
// sem_t semaforo_impedir_adelantamientos_extra;

// sem_t semaforo_anulaciones_adelantamiento_de_ronda;
// sem_t semaforo_pagos_adelantamiento_de_ronda;
// // SINCRONIZACION

// // SEMAFOROS DE PRIORIDADES
// sem_t semaforo_anulaciones;
// sem_t semaforo_pagos;
// sem_t semaforo_administracion_reservas;
// sem_t semaforo_consultas;
// // SEMAFOROS DE PRIORIDADES

// void *proceso( void *arg)
// {
//     int id_aux = 0;
//     int i = 0;
//     int mi_puerto;
//     long mi_prioridad;
//     struct mensaje peticion;
//     struct mensaje respuesta;
//     int longitud_peticion;
//     int longitud_respuesta;
//     int max_ticket_empece = 0;
//     int mi_ticket_pedi = 0;
//     int puedo_adelantar = 0;
//     // INICIAR LONGITUD
//     longitud_peticion = sizeof(peticion) - sizeof(peticion.tipo);
//     longitud_respuesta = sizeof(respuesta) - sizeof(respuesta.tipo);
//     // INICAR LONGITUD

//     // INICIAR LA INFORMACION DADA POR EL MAIN
//     mi_prioridad = (long)arg;
//     mi_puerto = puerto;

//     //printf("mi_prioridad: %li\n",mi_prioridad);
//         sem_wait(&semaforo_quiero);
//         quiero++;
//         sem_post(&semaforo_quiero);

//         //idea para adelantamiento de ronda
//         sem_wait(&semaforo_impedir_adelantamientos_extra);
//         sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
//         if(permitiendo_adelantamiento_de_ronda==1){
//             sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
//             if(mi_prioridad==1){
//                 puedo_adelantar=1;
//                 sem_wait(&semaforo_anulaciones_esperando_a_adelantar);
//                 anulaciones_esperando_a_adelantar++;
//                 sem_post(&semaforo_anulaciones_esperando_a_adelantar);
//                 sem_wait(&semaforo_anulaciones_adelantamiento_de_ronda);
//                 sem_wait(&semaforo_anulaciones_esperando_a_adelantar);
//                 anulaciones_esperando_a_adelantar--;
//                 sem_post(&semaforo_anulaciones_esperando_a_adelantar);
//                 sem_wait(&semaforo_anulaciones_esperando_a_adelantar);
//                 sem_wait(&semaforo_pagos_esperando_a_adelantar);
//                 if(anulaciones_esperando_a_adelantar!=0){
//                     sem_post(&semaforo_anulaciones_adelantamiento_de_ronda);
//                     //printf("salio anu y dejo a otra anu\n");
//                 }
//                 else if(pagos_esperando_a_adelantar!=0){
//                     sem_post(&semaforo_pagos_adelantamiento_de_ronda);
//                     //printf("salio anu y dejo a pago\n");
//                 }
//                 else{
//                     sem_wait(&semaforo_ultimo_adelantando);
//                     ultimo_adelantando = 1;
//                     sem_post(&semaforo_ultimo_adelantando);
//                     //printf("ultimo set por anu\n");
//                 }
//                 sem_post(&semaforo_pagos_esperando_a_adelantar);
//                 sem_post(&semaforo_anulaciones_esperando_a_adelantar);
//             }
//             if(mi_prioridad==2){
//                 puedo_adelantar=1;
//                 sem_wait(&semaforo_pagos_esperando_a_adelantar);
//                 pagos_esperando_a_adelantar++;
//                 sem_post(&semaforo_pagos_esperando_a_adelantar);
//                 sem_wait(&semaforo_pagos_adelantamiento_de_ronda);
//                 sem_wait(&semaforo_pagos_esperando_a_adelantar);
//                 pagos_esperando_a_adelantar--;
//                 sem_post(&semaforo_pagos_esperando_a_adelantar);
//                 sem_wait(&semaforo_pagos_esperando_a_adelantar);
//                 if(pagos_esperando_a_adelantar!=0){
//                     sem_post(&semaforo_pagos_adelantamiento_de_ronda);
//                     //printf("salio pago y dejo a otra pago\n");
//                 }
//                 else{
//                     sem_wait(&semaforo_ultimo_adelantando);
//                     ultimo_adelantando = 1;
//                     sem_post(&semaforo_ultimo_adelantando);
//                     //printf("ultimo set por pago\n");
//                 }
//                 sem_post(&semaforo_pagos_esperando_a_adelantar);
//             }
//         }
//         else{
//             sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
//         }
//         //idea para adelantamiento de ronda
//         if(puedo_adelantar==0){

//         sem_wait(&semaforo_primero_variable);

//         sem_wait(&semaforo_hizo_requests_consultas);
//         if(hizo_requests_consultas==0 || mi_prioridad!=4){
//         sem_post(&semaforo_hizo_requests_consultas);

//         sem_wait(&semaforo_primero);
//         sem_wait(&semaforo_consultas_dentro);//dudoso
//         if(primero || (consultas_dentro!=0 && mi_prioridad!=4)){
//             sem_post(&semaforo_consultas_dentro);
//             sem_post(&semaforo_primero);
//             sem_wait(&semaforo_consultas_dentro);
//             if(consultas_dentro!=0 && mi_prioridad!=4){//puede dar fallo
//                 sem_post(&semaforo_consultas_dentro);
//                 sem_wait(&semaforo_esperar);
//                 esperar=1;
//                 sem_post(&semaforo_esperar);
//                 sem_wait(&semaforo_repetir);
//                 repetir=1;
//                 sem_post(&semaforo_repetir);
//             }
//             else{
//                 sem_post(&semaforo_consultas_dentro);
//             }
//             sem_wait(&semaforo_esperar);
//             if(esperar){//si el primero no mando los mensajes antes de que le llegue una peticion mamamos (a lo mejor hay que cambiarlo de sitio)
//                 sem_post(&semaforo_esperar);

//                 sem_wait(&semaforo_semaforo_de_esperar);
//                 semaforo_de_esperar++;
//                 sem_post(&semaforo_semaforo_de_esperar);

//                 sem_wait(&semaforo_no_hacer_requests);
//                 sem_wait(&semaforo_semaforo_de_esperar);
//                 semaforo_de_esperar--;
//                 if(semaforo_de_esperar!=0){//esto hace que salgan todos los que esten aqui y vayan al de la seccion critica (creo que no afecta porque solo hay uno)
//                     sem_post(&semaforo_no_hacer_requests);
//                 }
//                 sem_post(&semaforo_semaforo_de_esperar);
//                 sem_wait(&semaforo_esperar);
//                 esperar=0;
//                 sem_post(&semaforo_esperar);
//             }
//             else{
//                 sem_post(&semaforo_esperar);
//             }
//             sem_wait(&semaforo_max_ticket);
//             sem_wait(&semaforo_ticket);
//             max_ticket_empece = max_ticket;
//             switch(mi_prioridad){
//                 case 1:
//                     mi_ticket = max_ticket+1;
//                     break;
//                 case 2:
//                     mi_ticket = max_ticket+2;
//                     break;
//                 case 3:
//                     mi_ticket = max_ticket+3;
//                     break;
//                 case 4:
//                     mi_ticket = max_ticket+4;
//                     break;
//             }
//             mi_ticket_pedi = mi_ticket;
//             sem_post(&semaforo_ticket);
//             sem_post(&semaforo_max_ticket);


//             //printf("Mi ticket: %i , Max ticket %i, Mi puerto: %d\n", mi_ticket, max_ticket,mi_puerto);
//             sem_wait(&semaforo_ticket);
//             sem_wait(&semaforo_max_ticket);
//             // INICAR STRUCT PETICION
//             peticion.tipo = 1;
//             peticion.ticket = mi_ticket;
//             peticion.id_nodo = mi_id;
//             peticion.puerto = mi_puerto;
//             peticion.prioridad = mi_prioridad;
//             // INICIAR STRUCT PETICION
//             sem_post(&semaforo_max_ticket);
//             sem_post(&semaforo_ticket);
//             for (i = 0; i < diferencia; i++)
//             {
//             if (msgsnd(id_nodos[i], &peticion, longitud_peticion, 0) == -1) // 1 es no bloqueante
//                 exit(-1);
//             }
//             primero=0;
//             if(mi_prioridad==4){
//                 sem_wait(&semaforo_hizo_requests_consultas);
//                 hizo_requests_consultas=1;
//                 sem_post(&semaforo_hizo_requests_consultas);
//             }
//             sem_wait(&semaforo_primero_dentro);
//             //////////////////////////////////////////////////////cerrar la ventana de adelantar cuando recibo la seccion critica
//             sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
//             sem_wait(&semaforo_no_volver_a_permitir_adelantamientos);
//             if(no_volver_a_permitir_adelantamientos==0 && permitiendo_adelantamiento_de_ronda==1){
//                 permitiendo_adelantamiento_de_ronda = 0;
//                 no_volver_a_permitir_adelantamientos = 1;
//             }
//             sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
//             sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);

//         }
//         else{
//             sem_post(&semaforo_consultas_dentro);
//             sem_post(&semaforo_primero);
//         }
        
//         sem_wait(&semaforo_ultimo_adelantando);
//         if(ultimo_adelantando==1){/////////////////////
//             sem_post(&semaforo_ultimo_adelantando);
//             sem_wait(&semaforo_primero);
//             primero = 1;
//             sem_post(&semaforo_primero);
//             sem_wait(&semaforo_ultimo_adelantando);
//             ultimo_adelantando = 0;
//             sem_post(&semaforo_ultimo_adelantando);
//         }
//         else{
//             sem_post(&semaforo_ultimo_adelantando);
//         }
//         }
//         else{
//             sem_post(&semaforo_hizo_requests_consultas);
//         }
//         sem_post(&semaforo_primero_variable);
//         sem_post(&semaforo_impedir_adelantamientos_extra);
//         }
//         puedo_adelantar=0;
//         sem_wait(&semaforo_esperando);
//         esperando++;
//         sem_post(&semaforo_esperando);
//         sem_wait(&semaforo_antes_de_entrar);
//         antes_de_entrar++;
//         sem_post(&semaforo_antes_de_entrar);

//         //PRIORIDADES IDEA 1
//         sem_wait(&semaforo_primer_proceso_general);
//         if(primer_proceso_general==0 || (proceso_prioritario_echa_a_consultas==1 && mi_prioridad==4)){//el primer proceso se salta esto
//             sem_post(&semaforo_primer_proceso_general);
//             switch(mi_prioridad){
//             case 1:
//                 anulaciones_esperando++;
//                 sem_wait(&semaforo_consultas_dentro);
//                 if(consultas_dentro!=0){
//                     proceso_prioritario_echa_a_consultas=1;
//                 }
//                 sem_post(&semaforo_consultas_dentro);
//                 sem_wait(&semaforo_anulaciones);
//                 if(proceso_prioritario_echa_a_consultas==1){
//                     proceso_prioritario_echa_a_consultas=0;
//                 }
//                 anulaciones_esperando--;
//                 break;
//             case 2:
//                 pagos_esperando++;
//                 sem_wait(&semaforo_consultas_dentro);
//                 if(consultas_dentro!=0){
//                     proceso_prioritario_echa_a_consultas=1;
//                 }
//                 sem_post(&semaforo_consultas_dentro);
//                 sem_wait(&semaforo_pagos);
//                 if(proceso_prioritario_echa_a_consultas==1){
//                     proceso_prioritario_echa_a_consultas=0;
//                 }
//                 pagos_esperando--;
//                 break;
//             case 3:
//                 admin_reser_esperando++;
//                 sem_wait(&semaforo_consultas_dentro);
//                 if(consultas_dentro!=0){
//                     proceso_prioritario_echa_a_consultas=1;
//                 }
//                 sem_post(&semaforo_consultas_dentro);
//                 sem_wait(&semaforo_administracion_reservas);
//                 if(proceso_prioritario_echa_a_consultas==1){
//                     proceso_prioritario_echa_a_consultas=0;
//                 }
//                 admin_reser_esperando--;
//                 break;
//             case 4:
//               sem_wait(&semaforo_consultas_dentro);
//               if(consultas_dentro==0 || proceso_prioritario_echa_a_consultas==1){
//                 sem_post(&semaforo_consultas_dentro);
//                 sem_wait(&semaforo_consultas_esperando);
//                 consultas_esperando++;
//                 sem_post(&semaforo_consultas_esperando);
//                 sem_wait(&semaforo_consultas);
//                 sem_wait(&semaforo_consultas_esperando);
//                 consultas_esperando--;
//                 sem_post(&semaforo_consultas_esperando);
//                 sem_wait(&semaforo_consultas_esperando);
//                 if(consultas_esperando!=0){
//                     sem_post(&semaforo_consultas_esperando);
//                     sem_post(&semaforo_consultas);//cada consulta saca a la siguiente
//                 }
//                 else{
//                     sem_post(&semaforo_consultas_esperando);
//                 }
//               }
//               else{
//                 sem_post(&semaforo_consultas_dentro);
//               }
//                 break;
//             default:
//                 break;
//             }
//         }
//         else{
//             sem_post(&semaforo_primer_proceso_general);
//         }
//         sem_wait(&semaforo_antes_de_entrar);
//         antes_de_entrar--;
//         sem_post(&semaforo_antes_de_entrar);
//         sem_wait(&semaforo_primer_proceso_general);
//         primer_proceso_general = 0;
//         sem_post(&semaforo_primer_proceso_general);
//         //PRIORIDADES IDEA 1
//         if(mi_prioridad!=4){//consultas
//             sem_wait(&semaforo_seccion_critica);
//         }
//         else{
//             sem_wait(&semaforo_consultas_dentro);
//             consultas_dentro++;
//             sem_post(&semaforo_consultas_dentro);
//             sem_wait(&semaforo_oks_a_consultas_ya_mandados);
//             if(oks_a_consultas_ya_mandados==0){
//                 oks_a_consultas_ya_mandados=1;
//                 sem_post(&semaforo_oks_a_consultas_ya_mandados);
//                 respuesta.tipo = 2;
//                 respuesta.id_nodo = mi_id;
//                 respuesta.ticket = 0;
//                 for (i = 0; i < num_pend; i++)
//                 {               
//                     respuesta.puerto = puertos_pendientes[i];
//                     if(peticiones_pendientes_de_consultas[i]==1){
//                         if (msgsnd(id_nodos_pendientes[i], &respuesta, longitud_respuesta, 1) == -1){ // 1 es no bloqueante
//                         exit(-1);
//                         }
//                         peticiones_pendientes_de_consultas[i]=2;
//                     }
//                 }
//             }
//             else{
//                 sem_post(&semaforo_oks_a_consultas_ya_mandados);
//             }
//         }


//         sem_wait(&semaforo_alguien_dentro);
//         alguien_dentro++;
//         sem_post(&semaforo_alguien_dentro);
//         // SECCION CRITICA
//             time_t tiempo_actual;
//             time_t tiempo_actual2;
//             struct tm *info_tiempo;
//             struct tm *info_tiempo2;
//             char hora_actual[20];
//             char hora_actual2[20];

//             time(&tiempo_actual);
//             info_tiempo = localtime(&tiempo_actual);

//             strftime(hora_actual, sizeof(hora_actual), "%Y-%m-%d %H:%M:%S", info_tiempo);
//         printf("***********\n");
//         printf("[%i,%s,%li] Dentro de la seccion critica en ronda [%d,%d]\n",mi_id,hora_actual,mi_prioridad,mi_ticket_pedi,max_ticket_empece);
//         printf("***********\n");
//         sleep(8);
//         time(&tiempo_actual2);
//             info_tiempo2 = localtime(&tiempo_actual2);

//             strftime(hora_actual2, sizeof(hora_actual2), "%Y-%m-%d %H:%M:%S", info_tiempo2);
//         printf("***********\n");
//         printf("[%i,%s,%li] Termine\n",mi_id,hora_actual2,mi_prioridad);
//         printf("***********\n\n\n");
//         fflush(stdout);
//         // SECCION CRITICA
//         sem_wait(&semaforo_alguien_dentro);
//         alguien_dentro--;
//         sem_post(&semaforo_alguien_dentro);

//         if(mi_prioridad!=4){//consultas
//             sem_post(&semaforo_seccion_critica);
//         }
//         else{
//             sem_wait(&semaforo_consultas_dentro);
//             consultas_dentro--;
//             sem_post(&semaforo_consultas_dentro);
//         }
//         sem_wait(&semaforo_esperando);
//         esperando--;
//         sem_post(&semaforo_esperando);
//         sem_wait(&semaforo_quiero);
//         quiero--;
//         sem_post(&semaforo_quiero);

//         if(mi_prioridad==4){
//             sem_wait(&semaforo_hizo_requests_consultas);
//             hizo_requests_consultas=0;
//             sem_post(&semaforo_hizo_requests_consultas);
//         }
//         sem_wait(&semaforo_anulaciones_esperando_a_adelantar);
//         sem_wait(&semaforo_pagos_esperando_a_adelantar);
//         if(anulaciones_esperando_a_adelantar!=0 || pagos_esperando_a_adelantar!=0){
//             sem_post(&semaforo_pagos_esperando_a_adelantar);
//             sem_post(&semaforo_anulaciones_esperando_a_adelantar);
//             sem_wait(&semaforo_primero);
//             primero = 0;
//             sem_post(&semaforo_primero);
//             sem_wait(&semaforo_primer_proceso_general);
//             primer_proceso_general = 1;
//             sem_post(&semaforo_primer_proceso_general);
//             if(anulaciones_esperando_a_adelantar!=0){
//                 sem_post(&semaforo_anulaciones_adelantamiento_de_ronda);
//             }
//             else{
//                 sem_post(&semaforo_pagos_adelantamiento_de_ronda);
//             }
//         }
//         else{
//         sem_post(&semaforo_pagos_esperando_a_adelantar);
//         sem_post(&semaforo_anulaciones_esperando_a_adelantar);
//         //PRIORIDADES IDEA 1
//         sem_wait(&semaforo_consultas_dentro);
//         sem_wait(&semaforo_esperando);
//         if(esperando!=0 && consultas_dentro==0){
//             if(anulaciones_esperando!=0){
//                 sem_post(&semaforo_anulaciones);
//             }
//             else if(pagos_esperando!=0)
//             {
//                 sem_post(&semaforo_pagos);
//             }
//             else if(admin_reser_esperando!=0)
//             {
//                 sem_post(&semaforo_administracion_reservas);
//             }
//             else    
//                 sem_post(&semaforo_consultas);
//         }
//         sem_post(&semaforo_esperando);
//         sem_post(&semaforo_consultas_dentro);
//         //PRIORIDADES IDEA 1
//         sem_wait(&semaforo_repetir);
//         sem_wait(&semaforo_esperando);
//         if(esperando==0 && repetir==1){
//             sem_post(&semaforo_esperando);
//             sem_post(&semaforo_repetir);
//                 respuesta.tipo = 2;
//                 respuesta.id_nodo = mi_id;
//                 respuesta.ticket = 0;
//                 sem_wait(&semaforo_primer_proceso_general);
//                 primer_proceso_general=1;
//                 sem_post(&semaforo_primer_proceso_general);
//                 for (i = 0; i < num_pend; i++)
//                 {               
//                     respuesta.puerto = puertos_pendientes[i];
//                     if(peticiones_pendientes_de_consultas[i]!=2){
//                         if (msgsnd(id_nodos_pendientes[i], &respuesta, longitud_respuesta, 1) == -1){ // 1 es no bloqueante
//                         exit(-1);
//                         }
//                     }
//                     peticiones_pendientes_de_consultas[i]=0;
//                 }
//                 sem_wait(&semaforo_oks_a_consultas_ya_mandados);
//                 oks_a_consultas_ya_mandados=0;
//                 sem_post(&semaforo_oks_a_consultas_ya_mandados);
//                 sleep(1);//>>para poder observar las prioridades
//                 sem_wait(&semaforo_repetir);
//                 repetir=0;
//                 sem_post(&semaforo_repetir);
//                 sem_wait(&semaforo_num_pendientes);
//                 num_pend = 0;
//                 sem_post(&semaforo_num_pendientes);

//                 sem_wait(&semaforo_no_volver_a_permitir_adelantamientos);
//                 no_volver_a_permitir_adelantamientos=0;//////
//                 sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
//                 sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
//                 permitiendo_adelantamiento_de_ronda=0;//////
//                 sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
//                 //printf("acabamos\n");
//                 sem_post(&semaforo_no_hacer_requests);
        
//         }
//         else{
//             sem_post(&semaforo_esperando);
//             sem_post(&semaforo_repetir);
//         }
//         }

//         sem_wait(&semaforo_quiero);
//         if(quiero==0){
//             sem_post(&semaforo_quiero);
//             sem_wait(&semaforo_primer_proceso_general);
//             primer_proceso_general=1;
//             sem_post(&semaforo_primer_proceso_general);
//             sem_wait(&semaforo_primero);
//             primero=1;
//             sem_post(&semaforo_primero);
//         }
//         else{
//             sem_post(&semaforo_quiero);
//         }
// }

// void *receptor(void *args){
// int id_nodo_origen = 0;
//     int ticket_origen = 0;
//     int tipo_origen = 0;
//     int prioridad_origen = 0;
//     int puerto_origen = 0;
//     int longitud_peticion;
//     int longitud_respuesta;
//     int longitud_mensajito = 0;
//     int cuenta = 0;
//     struct mensaje mensaje_recibido;
//     struct mensaje respuesta;
//     struct info_a_mandar_al_hilo *aux;
//     longitud_peticion = sizeof(mensaje_recibido) - sizeof(mensaje_recibido.tipo);
//     longitud_respuesta = sizeof(respuesta) - sizeof(respuesta.tipo);

//     //printf("diferencia en receptor: %d\n",diferencia);

//     while (1)
//     {
//         // RECIBIR MENSAJE AJENA

//         if (msgrcv(mi_id, &mensaje_recibido, longitud_peticion, 0, 0) == -1) // RECIBE TODOS LOS MENSAJES QUE LE LLEGUEN
//         {                           
//             exit(-1);
//         }
//         srand(mi_id);
//         sleep(rand() % 2);
//         tipo_origen = mensaje_recibido.tipo;
//         ticket_origen = mensaje_recibido.ticket;
//         id_nodo_origen = mensaje_recibido.id_nodo;
//         puerto_origen = mensaje_recibido.puerto;
//         prioridad_origen = mensaje_recibido.prioridad;

//         // RECIBIR PETICION AJENA
//         if (tipo_origen == 1)
//         { // ESTO ES QUE RECIBIO UNA REQUEST
//             sem_wait(&semaforo_max_ticket);
//             if (max_ticket < ticket_origen)
//             {
//                 max_ticket = ticket_origen;
//             }

//             sem_post(&semaforo_max_ticket);

//             sem_wait(&semaforo_quiero);
//             sem_wait(&semaforo_ticket);
//             sem_wait(&semaforo_mi_id);//////////////////////////////////
//             sem_wait(&semaforo_num_pendientes);
//             //sem_wait(&semaforo_nodos_pendientes);

//             /*printf("Quiero: %i\n",quiero);
//             printf("Mi ticket: %i, ticket origen: %i\n",mi_ticket,ticket_origen);
//             printf("Mi id: %i, id origen: %i\n",mi_id,id_nodo_origen);*/
//             sem_wait(&semaforo_antes_de_entrar);
//             if (quiero==0 || (ticket_origen < mi_ticket) || (ticket_origen == mi_ticket && (id_nodo_origen < mi_id)) || (prioridad_origen==4 && consultas_dentro!=0 && num_pend==0 && antes_de_entrar==0))//esto pa varias consultas en diferentes nodos
//             {
//                 sem_post(&semaforo_num_pendientes);
//                 sem_post(&semaforo_mi_id);
//                 sem_post(&semaforo_ticket);
//                 sem_post(&semaforo_quiero);


//                 sem_post(&semaforo_antes_de_entrar);
//                 sem_wait(&semaforo_no_volver_a_permitir_adelantamientos);
//                 sem_wait(&semaforo_alguien_dentro);
//                 sem_wait(&semaforo_quiero);
//                 if(no_volver_a_permitir_adelantamientos==0 && alguien_dentro==0 && quiero!=0){
//                     sem_post(&semaforo_quiero);
//                     sem_post(&semaforo_alguien_dentro);
//                     sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
//                     sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
//                     permitiendo_adelantamiento_de_ronda = 1;
//                     sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
//                 }
//                 else{
//                     sem_post(&semaforo_quiero);
//                     sem_post(&semaforo_alguien_dentro);
//                     sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
//                 }
//                 sem_wait(&semaforo_mi_id);
//                 respuesta.id_nodo = mi_id;
//                 sem_post(&semaforo_mi_id);
//                 respuesta.tipo = 2;
//                 respuesta.puerto = puerto_origen;
//                 sem_wait(&semaforo_ticket);
//                 respuesta.ticket = mi_ticket;
//                 sem_post(&semaforo_ticket);
//                 if (msgsnd(id_nodo_origen, &respuesta, longitud_respuesta, 1) == -1) // 1 es no bloqueante
//                     exit(-1);
//             }
//             else
//             {   
//                 sem_post(&semaforo_num_pendientes);
//                 sem_post(&semaforo_mi_id);
//                 sem_post(&semaforo_ticket);
//                 sem_post(&semaforo_quiero);

//                 sem_post(&semaforo_antes_de_entrar);
//                 sem_wait(&semaforo_primero);
//                 primero = 1;///
//                 sem_post(&semaforo_primero);
//                 sem_wait(&semaforo_repetir);
//                 repetir = 1;///
//                 sem_post(&semaforo_repetir);
//                 sem_wait(&semaforo_esperar);
//                 esperar = 1;///
//                 sem_post(&semaforo_esperar);
//                 sem_wait(&semaforo_no_volver_a_permitir_adelantamientos);
//                 sem_wait(&semaforo_alguien_dentro);
//                 if(no_volver_a_permitir_adelantamientos==0 && alguien_dentro==0){
//                     sem_post(&semaforo_alguien_dentro);
//                     sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
//                     sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
//                     permitiendo_adelantamiento_de_ronda = 1;
//                     sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
//                 }
//                 else{
//                     sem_post(&semaforo_alguien_dentro);
//                     sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
//                 }
//                 sem_wait(&semaforo_num_pendientes);
//                 id_nodos_pendientes[num_pend] = id_nodo_origen;
//                 puertos_pendientes[num_pend] = puerto_origen;//////se puede quitar
//                 sem_post(&semaforo_num_pendientes);
//                 if(prioridad_origen==4){
//                     sem_wait(&semaforo_num_pendientes);
//                     peticiones_pendientes_de_consultas[num_pend] = 1;
//                     sem_post(&semaforo_num_pendientes);
//                     sem_wait(&semaforo_oks_a_consultas_ya_mandados);
//                     oks_a_consultas_ya_mandados=0;
//                     sem_post(&semaforo_oks_a_consultas_ya_mandados);
//                 }
//                 sem_wait(&semaforo_num_pendientes);
//                 num_pend++;
//                 sem_post(&semaforo_num_pendientes);
//             }
//             //sem_post(&semaforo_nodos_pendientes);
//         }
//         else{
//             cuenta++;
//             if(cuenta==(diferencia)){//ya recibio todos los oks
//                 cuenta=0;
//                 sem_post(&semaforo_primero_dentro);
//             }
//         }
//     }
// }

// int main(int argc, char *argv[])
// {
//     struct info_a_mandar_al_hilo info;
//     int i = 0;
//     int numero_procesos = 0;
//     long prioridad_del_proceso = 0;
//     pthread_t thread_receptor;

//     // INICIAR SEMAFOROS
//     sem_init(&semaforo_quiero, 0, 1);
//     sem_init(&semaforo_ticket, 0, 1);
//     sem_init(&semaforo_mi_id, 0, 1);
//     sem_init(&semaforo_nodos_pendientes, 0, 1);
//     sem_init(&semaforo_num_pendientes, 0, 1);
//     sem_init(&semaforo_max_ticket, 0, 1);
//     sem_init(&semaforo_seccion_critica, 0, 1);
//     sem_init(&semaforo_primero_variable, 0, 1);
//     sem_init(&semaforo_primero_dentro, 0, 0);
//     sem_init(&semaforo_no_hacer_requests, 0, 0);
//     sem_init(&semaforo_anulaciones, 0, 0);
//     sem_init(&semaforo_administracion_reservas, 0, 0);
//     sem_init(&semaforo_pagos, 0, 0);
//     sem_init(&semaforo_consultas, 0, 0);
//     sem_init(&semaforo_semaforo_de_esperar, 0, 1);
//     sem_init(&semaforo_anulaciones_adelantamiento_de_ronda, 0, 0);
//     sem_init(&semaforo_pagos_adelantamiento_de_ronda, 0, 0);

//     sem_init(&semaforo_primero,0,1);
//     sem_init(&semaforo_repetir,0,1);
//     sem_init(&semaforo_esperar,0,1);
//     sem_init(&semaforo_primer_proceso_general,0,1);
//     sem_init(&semaforo_consultas_esperando,0,1);
//     sem_init(&semaforo_consultas_dentro,0,1);
//     sem_init(&semaforo_antes_de_entrar,0,1);
//     sem_init(&semaforo_hizo_requests_consultas,0,1);
//     sem_init(&semaforo_permitiendo_adelantamiento_de_ronda,0,1);
//     sem_init(&semaforo_no_volver_a_permitir_adelantamientos,0,1);
//     sem_init(&semaforo_anulaciones_esperando_a_adelantar,0,1);
//     sem_init(&semaforo_pagos_esperando_a_adelantar,0,1);
//     sem_init(&semaforo_ultimo_adelantando,0,1);
//     sem_init(&semaforo_alguien_dentro,0,1);
//     sem_init(&semaforo_oks_a_consultas_ya_mandados,0,1);
//     sem_init(&semaforo_esperando,0,1);
//     sem_init(&semaforo_impedir_adelantamientos_extra,0,1);
//     // INICIAR SEMAFOROS

//     if (argc != 6) {
//         printf("Usage: %s [nodo_minimo] [nodo_maximo] [node_id] [instrucciones] [tiempos]\n", argv[0]);
//         return -1;
//     }

//     int nodo_minimo = atoi(argv[1]);
//     int nodo_maximo = atoi(argv[2]);
//     mi_id = atoi(argv[3]);
//     char* instrucciones = argv[4];
//     char* tiempos = argv[5];

//     int longitud_instrucciones = 0;
//     // Array para almacenar las instrucciones como enteros.
//     int instr_arr[MAX_INSTR_LENGTH];
//     int instr_count = 0;

//     // Dividir la cadena de instrucciones en subcadenas separadas por comas y convertirlas en enteros.
//     char* token = strtok(instrucciones, ",");
//     while (token != NULL && instr_count < MAX_INSTR_LENGTH) {
//         // Omitir los espacios en blanco antes del número.
//         while (*token== ' ') {
//             token++;
//         }
//         int instr = atoi(token);
//         longitud_instrucciones++;
//         instr_arr[instr_count++] = instr;
//         token = strtok(NULL, ",");
//     }

//     // Array para almacenar los tiempos como enteros.
//     int tiempos_arr[MAX_INSTR_LENGTH];
//     int tiempos_count = 0;

//     // Dividir la cadena de tiempos en subcadenas separadas por comas y convertirlas en enteros.
//     char* token_tiempo = strtok(tiempos, ",");
//     while (token_tiempo != NULL && tiempos_count < MAX_INSTR_LENGTH) {
//         // Omitir los espacios en blanco antes del número.
//         while (*token_tiempo == ' ') {
//             token_tiempo++;
//         }
//         int tiempo = atoi(token_tiempo);
//         tiempos_arr[tiempos_count++] = tiempo;
//         token_tiempo = strtok(NULL, ",");
//     }

//     // INICIAR ID_NODOS
//     int a = 0;
//     for (i = nodo_minimo; i <= nodo_maximo; i++)
//     {
//         if (i != mi_id)
//         {
//             id_nodos[a] = i;
//             a++;
//         }
//     }
//     diferencia = (nodo_maximo - nodo_minimo); //buzon 0 es de nodo 0 buzon 1 es el buzon interno del nodo 0
//     // INICIAR ID_NODOS

//     // INICIAR THREADS
//     pthread_create(&thread_receptor, NULL, receptor, NULL);//esto crea el receptor y le pasa la struct con la diferencia
    


//     //INICIAR HILOS
//     for (int j = 0; j < longitud_instrucciones; j += 4) {
        
//         //printf("j: %d\n", j);

//         // Extraer cuatro elementos del array de instrucciones.
//         int instr1 = instr_arr[j];
//         int instr2 = instr_arr[j+1];
//         int instr3 = instr_arr[j+2];
//         int instr4 = instr_arr[j+3];

//         // Extraer un elemento del array de tiempos.
//         int tiempo = tiempos_arr[j/4];

//         // Mostrar los resultados.
//         //printf("Instrucciones: %d %d %d %d, Tiempo: %d\n", instr1, instr2, instr3, instr4, tiempo);
//         prioridad_del_proceso=1;
//         for(a=0; a<instr1; a++){
//             printf("prioridad: %li\n",prioridad_del_proceso);
//             pthread_create(&thread_receptor, NULL, proceso, (void *)prioridad_del_proceso);
//         }
//         prioridad_del_proceso=2;
//         for(a=0; a<instr2; a++){
//             printf("prioridad: %li\n",prioridad_del_proceso);
//             pthread_create(&thread_receptor, NULL, proceso, (void *)prioridad_del_proceso);
//         }
//         prioridad_del_proceso=3;
//         for(a=0; a<instr3; a++){
//             printf("prioridad: %li\n",prioridad_del_proceso);
//             pthread_create(&thread_receptor, NULL, proceso, (void *)prioridad_del_proceso);
//         }
//         prioridad_del_proceso=4;
//         for(a=0; a<instr4; a++){
//             printf("prioridad: %li\n",prioridad_del_proceso);
//             pthread_create(&thread_receptor, NULL, proceso, (void *)prioridad_del_proceso);
//         }
//         sleep(tiempo);

//     }
//     /*for(a = 0; a<numero_procesos; a++){
//         info.puerto = a+1;
//         if(a<4){
//             info.prioridad = 1;
//         }
//         else if(a<8 && a>=4){
//             info.prioridad = 2;
//         }
//         else if(a<12 && a>=8){
//             info.prioridad = 3;
//         }
//         else{
//             info.prioridad = 4;
//         }
//         pthread_create(&thread_receptor, NULL, proceso, (void *)&info);
//         sleep(1);
//     }*/
//     sleep(300);
//     return -1;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#define MAX_INSTR_LENGTH 1000

struct mensaje
{
    long tipo;
    int ticket;
    int id_nodo;
    int puerto;
    long prioridad;
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

int consultas_esperando_la_seccion_critica = 0;

int peticiones_pendientes_de_consultas[100] = {0};
int oks_a_consultas_ya_mandados = 0;
// VARIABLES UTILES

//CAMBIO DE VARIABLES
int id_nodos[10];
int diferencia;
int puerto = 1;
//CAMBIO DE VARIABLES

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
sem_t semaforo_primer_proceso_general;
sem_t semaforo_consultas_dentro;
sem_t semaforo_antes_de_entrar;
sem_t semaforo_primero;
sem_t semaforo_repetir;
sem_t semaforo_esperar;
sem_t semaforo_consultas_esperando;
sem_t semaforo_hizo_requests_consultas;
sem_t semaforo_permitiendo_adelantamiento_de_ronda;
sem_t semaforo_no_volver_a_permitir_adelantamientos;
sem_t semaforo_anulaciones_esperando_a_adelantar;
sem_t semaforo_pagos_esperando_a_adelantar;
sem_t semaforo_ultimo_adelantando;
sem_t semaforo_alguien_dentro;
sem_t semaforo_oks_a_consultas_ya_mandados;
sem_t semaforo_esperando;
sem_t semaforo_consultas_esperando_la_seccion_critica;

sem_t semaforo_anulaciones_adelantamiento_de_ronda;
sem_t semaforo_pagos_adelantamiento_de_ronda;

sem_t semaforo_coordinador_main_hilos;
// SINCRONIZACION

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
    long mi_prioridad;
    struct mensaje peticion;
    struct mensaje respuesta;
    int longitud_peticion;
    int longitud_respuesta;
    int max_ticket_empece = 0;
    int mi_ticket_pedi = 0;
    int puedo_adelantar = 0;
    // INICIAR LONGITUD
    longitud_peticion = sizeof(peticion) - sizeof(peticion.tipo);
    longitud_respuesta = sizeof(respuesta) - sizeof(respuesta.tipo);
    // INICAR LONGITUD

    // INICIAR LA INFORMACION DADA POR EL MAIN
    mi_prioridad = (long)arg;
    mi_puerto = puerto;
    sem_post(&semaforo_coordinador_main_hilos);

    //printf("mi_prioridad: %li\n",mi_prioridad);
        sem_wait(&semaforo_quiero);
        quiero++;
        sem_post(&semaforo_quiero);

        //idea para adelantamiento de ronda
        sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
        if(permitiendo_adelantamiento_de_ronda==1){
            sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
            if(mi_prioridad==1){
                puedo_adelantar=1;
                sem_wait(&semaforo_anulaciones_esperando_a_adelantar);
                anulaciones_esperando_a_adelantar++;
                sem_post(&semaforo_anulaciones_esperando_a_adelantar);
                sem_wait(&semaforo_anulaciones_adelantamiento_de_ronda);
                sem_wait(&semaforo_anulaciones_esperando_a_adelantar);
                anulaciones_esperando_a_adelantar--;
                sem_post(&semaforo_anulaciones_esperando_a_adelantar);
                sem_wait(&semaforo_anulaciones_esperando_a_adelantar);
                sem_wait(&semaforo_pagos_esperando_a_adelantar);
                if(anulaciones_esperando_a_adelantar!=0){
                    sem_post(&semaforo_anulaciones_adelantamiento_de_ronda);
                    //printf("salio anu y dejo a otra anu\n");
                }
                else if(pagos_esperando_a_adelantar!=0){
                    sem_post(&semaforo_pagos_adelantamiento_de_ronda);
                    //printf("salio anu y dejo a pago\n");
                }
                else{
                    sem_wait(&semaforo_ultimo_adelantando);
                    ultimo_adelantando = 1;
                    sem_post(&semaforo_ultimo_adelantando);
                    //printf("ultimo set por anu\n");
                }
                sem_post(&semaforo_pagos_esperando_a_adelantar);
                sem_post(&semaforo_anulaciones_esperando_a_adelantar);
            }
            if(mi_prioridad==2){
                puedo_adelantar=1;
                sem_wait(&semaforo_pagos_esperando_a_adelantar);
                pagos_esperando_a_adelantar++;
                sem_post(&semaforo_pagos_esperando_a_adelantar);
                sem_wait(&semaforo_pagos_adelantamiento_de_ronda);
                sem_wait(&semaforo_pagos_esperando_a_adelantar);
                pagos_esperando_a_adelantar--;
                sem_post(&semaforo_pagos_esperando_a_adelantar);
                sem_wait(&semaforo_pagos_esperando_a_adelantar);
                if(pagos_esperando_a_adelantar!=0){
                    sem_post(&semaforo_pagos_adelantamiento_de_ronda);
                    //printf("salio pago y dejo a otra pago\n");
                }
                else{
                    sem_wait(&semaforo_ultimo_adelantando);
                    ultimo_adelantando = 1;
                    sem_post(&semaforo_ultimo_adelantando);
                    //printf("ultimo set por pago\n");
                }
                sem_post(&semaforo_pagos_esperando_a_adelantar);
            }
        }
        else{
            sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
        }
        //idea para adelantamiento de ronda
        if(puedo_adelantar==0){

        sem_wait(&semaforo_primero_variable);

        sem_wait(&semaforo_hizo_requests_consultas);
        if(hizo_requests_consultas==0 || mi_prioridad!=4){
        sem_post(&semaforo_hizo_requests_consultas);

        sem_wait(&semaforo_primero);
        sem_wait(&semaforo_consultas_dentro);//dudoso
        if(primero || (consultas_dentro!=0 && mi_prioridad!=4)){
            if(mi_prioridad==4){
                sem_wait(&semaforo_consultas_esperando_la_seccion_critica);
                consultas_esperando_la_seccion_critica=1;
                sem_post(&semaforo_consultas_esperando_la_seccion_critica);
            }
            sem_post(&semaforo_consultas_dentro);
            sem_post(&semaforo_primero);
            sem_wait(&semaforo_consultas_dentro);
            if(consultas_dentro!=0 && mi_prioridad!=4){//puede dar fallo
                sem_post(&semaforo_consultas_dentro);
                sem_wait(&semaforo_esperar);
                esperar=1;
                sem_post(&semaforo_esperar);
                sem_wait(&semaforo_repetir);
                repetir=1;
                sem_post(&semaforo_repetir);
            }
            else{
                sem_post(&semaforo_consultas_dentro);
            }
            sem_wait(&semaforo_esperar);
            if(esperar){//si el primero no mando los mensajes antes de que le llegue una peticion mamamos (a lo mejor hay que cambiarlo de sitio)
                sem_post(&semaforo_esperar);

                sem_wait(&semaforo_semaforo_de_esperar);
                semaforo_de_esperar++;
                sem_post(&semaforo_semaforo_de_esperar);

                sem_wait(&semaforo_no_hacer_requests);
                sem_wait(&semaforo_semaforo_de_esperar);
                semaforo_de_esperar--;
                if(semaforo_de_esperar!=0){//esto hace que salgan todos los que esten aqui y vayan al de la seccion critica (creo que no afecta porque solo hay uno)
                    sem_post(&semaforo_no_hacer_requests);
                }
                sem_post(&semaforo_semaforo_de_esperar);
                sem_wait(&semaforo_esperar);
                esperar=0;
                sem_post(&semaforo_esperar);
            }
            else{
                sem_post(&semaforo_esperar);
            }
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


            //printf("Mi ticket: %i , Max ticket %i, Mi puerto: %d\n", mi_ticket, max_ticket,mi_puerto);
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
            for (i = 0; i < diferencia; i++)
            {
            if (msgsnd(id_nodos[i], &peticion, longitud_peticion, 0) == -1) // 1 es no bloqueante
                exit(-1);
            }
            primero=0;
            if(mi_prioridad==4){
                sem_wait(&semaforo_hizo_requests_consultas);
                hizo_requests_consultas=1;
                sem_post(&semaforo_hizo_requests_consultas);
            }
            sem_wait(&semaforo_primero_dentro);
        }
        else{
            sem_post(&semaforo_consultas_dentro);
            sem_post(&semaforo_primero);
        }
        
        sem_wait(&semaforo_ultimo_adelantando);
        if(ultimo_adelantando==1){/////////////////////
            sem_post(&semaforo_ultimo_adelantando);
            sem_wait(&semaforo_primero);
            primero = 1;
            sem_post(&semaforo_primero);
            sem_wait(&semaforo_ultimo_adelantando);
            ultimo_adelantando = 0;
            sem_post(&semaforo_ultimo_adelantando);
        }
        else{
            sem_post(&semaforo_ultimo_adelantando);
        }
        }
        else{
            sem_post(&semaforo_hizo_requests_consultas);
        }
        if(mi_prioridad==4){
            sem_wait(&semaforo_consultas_esperando_la_seccion_critica);
            consultas_esperando_la_seccion_critica=0;
            sem_post(&semaforo_consultas_esperando_la_seccion_critica);
        }
        sem_post(&semaforo_primero_variable);
        }
        puedo_adelantar=0;
        sem_wait(&semaforo_esperando);
        esperando++;
        sem_post(&semaforo_esperando);
        sem_wait(&semaforo_antes_de_entrar);
        antes_de_entrar++;
        sem_post(&semaforo_antes_de_entrar);

        //PRIORIDADES IDEA 1
        sem_wait(&semaforo_primer_proceso_general);
        if(primer_proceso_general==0 || (proceso_prioritario_echa_a_consultas==1 && mi_prioridad==4)){//el primer proceso se salta esto
            sem_post(&semaforo_primer_proceso_general);
            switch(mi_prioridad){
            case 1:
                anulaciones_esperando++;
                sem_wait(&semaforo_consultas_dentro);
                if(consultas_dentro!=0){
                    proceso_prioritario_echa_a_consultas=1;
                }
                sem_post(&semaforo_consultas_dentro);
                sem_wait(&semaforo_anulaciones);
                if(proceso_prioritario_echa_a_consultas==1){
                    proceso_prioritario_echa_a_consultas=0;
                }
                anulaciones_esperando--;
                break;
            case 2:
                pagos_esperando++;
                sem_wait(&semaforo_consultas_dentro);
                if(consultas_dentro!=0){
                    proceso_prioritario_echa_a_consultas=1;
                }
                sem_post(&semaforo_consultas_dentro);
                sem_wait(&semaforo_pagos);
                if(proceso_prioritario_echa_a_consultas==1){
                    proceso_prioritario_echa_a_consultas=0;
                }
                pagos_esperando--;
                break;
            case 3:
                admin_reser_esperando++;
                sem_wait(&semaforo_consultas_dentro);
                if(consultas_dentro!=0){
                    proceso_prioritario_echa_a_consultas=1;
                }
                sem_post(&semaforo_consultas_dentro);
                sem_wait(&semaforo_administracion_reservas);
                if(proceso_prioritario_echa_a_consultas==1){
                    proceso_prioritario_echa_a_consultas=0;
                }
                admin_reser_esperando--;
                break;
            case 4:
              sem_wait(&semaforo_consultas_dentro);
              if(consultas_dentro==0 || proceso_prioritario_echa_a_consultas==1){
                sem_post(&semaforo_consultas_dentro);
                sem_wait(&semaforo_consultas_esperando);
                consultas_esperando++;
                sem_post(&semaforo_consultas_esperando);
                sem_wait(&semaforo_consultas);
                sem_wait(&semaforo_consultas_esperando);
                consultas_esperando--;
                sem_post(&semaforo_consultas_esperando);
                sem_wait(&semaforo_consultas_esperando);
                if(consultas_esperando!=0){
                    sem_post(&semaforo_consultas_esperando);
                    sem_post(&semaforo_consultas);//cada consulta saca a la siguiente
                }
                else{
                    sem_post(&semaforo_consultas_esperando);
                }
              }
              else{
                sem_post(&semaforo_consultas_dentro);
              }
                break;
            default:
                break;
            }
        }
        else{
            sem_post(&semaforo_primer_proceso_general);
        }
        sem_wait(&semaforo_antes_de_entrar);
        antes_de_entrar--;
        sem_post(&semaforo_antes_de_entrar);
        sem_wait(&semaforo_primer_proceso_general);
        primer_proceso_general = 0;
        sem_post(&semaforo_primer_proceso_general);
        //PRIORIDADES IDEA 1
        if(mi_prioridad!=4){//consultas
            sem_wait(&semaforo_seccion_critica);
        }
        else{
            sem_wait(&semaforo_consultas_dentro);
            consultas_dentro++;
            sem_post(&semaforo_consultas_dentro);
            sem_wait(&semaforo_oks_a_consultas_ya_mandados);
            if(oks_a_consultas_ya_mandados==0){
                oks_a_consultas_ya_mandados=1;
                sem_post(&semaforo_oks_a_consultas_ya_mandados);
                respuesta.tipo = 2;
                respuesta.id_nodo = mi_id;
                respuesta.ticket = 0;
                for (i = 0; i < num_pend; i++)
                {               
                    respuesta.puerto = puertos_pendientes[i];
                    if(peticiones_pendientes_de_consultas[i]==1){
                        if (msgsnd(id_nodos_pendientes[i], &respuesta, longitud_respuesta, 1) == -1){ // 1 es no bloqueante
                        exit(-1);
                        }
                        peticiones_pendientes_de_consultas[i]=2;
                    }
                }
            }
            else{
                sem_post(&semaforo_oks_a_consultas_ya_mandados);
            }
        }
        sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
        sem_wait(&semaforo_no_volver_a_permitir_adelantamientos);
        if(no_volver_a_permitir_adelantamientos==0 && permitiendo_adelantamiento_de_ronda==1){
            permitiendo_adelantamiento_de_ronda = 0;
            no_volver_a_permitir_adelantamientos = 1;
        }
        sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
        sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);

        sem_wait(&semaforo_alguien_dentro);
        alguien_dentro++;
        sem_post(&semaforo_alguien_dentro);
        // SECCION CRITICA
            time_t tiempo_actual;
            time_t tiempo_actual2;
            struct tm *info_tiempo;
            struct tm *info_tiempo2;
            char hora_actual[20];
            char hora_actual2[20];

            time(&tiempo_actual);
            info_tiempo = localtime(&tiempo_actual);

            strftime(hora_actual, sizeof(hora_actual), "%Y-%m-%d %H:%M:%S", info_tiempo);
        printf("***********\n");
        printf("[%i,%s,%li] Dentro de la seccion critica en ronda [%d,%d]\n",mi_id,hora_actual,mi_prioridad,mi_ticket_pedi,max_ticket_empece);
        printf("***********\n");
        sleep(8);
        time(&tiempo_actual2);
            info_tiempo2 = localtime(&tiempo_actual2);

            strftime(hora_actual2, sizeof(hora_actual2), "%Y-%m-%d %H:%M:%S", info_tiempo2);
        printf("***********\n");
        printf("[%i,%s,%li] Termine\n",mi_id,hora_actual2,mi_prioridad);
        printf("***********\n\n\n");
        fflush(stdout);
        // SECCION CRITICA
        sem_wait(&semaforo_alguien_dentro);
        alguien_dentro--;
        sem_post(&semaforo_alguien_dentro);

        if(mi_prioridad!=4){//consultas
            sem_post(&semaforo_seccion_critica);
        }
        else{
            sem_wait(&semaforo_consultas_dentro);
            consultas_dentro--;
            sem_post(&semaforo_consultas_dentro);
        }
        sem_wait(&semaforo_esperando);
        esperando--;
        sem_post(&semaforo_esperando);
        sem_wait(&semaforo_quiero);
        quiero--;
        sem_post(&semaforo_quiero);

        if(mi_prioridad==4){
            sem_wait(&semaforo_hizo_requests_consultas);
            hizo_requests_consultas=0;
            sem_post(&semaforo_hizo_requests_consultas);
        }
        sem_wait(&semaforo_anulaciones_esperando_a_adelantar);
        sem_wait(&semaforo_pagos_esperando_a_adelantar);
        if(anulaciones_esperando_a_adelantar!=0 || pagos_esperando_a_adelantar!=0){
            sem_post(&semaforo_pagos_esperando_a_adelantar);
            sem_post(&semaforo_anulaciones_esperando_a_adelantar);
            sem_wait(&semaforo_primero);
            primero = 0;
            sem_post(&semaforo_primero);
            sem_wait(&semaforo_primer_proceso_general);
            primer_proceso_general = 1;
            sem_post(&semaforo_primer_proceso_general);
            if(anulaciones_esperando_a_adelantar!=0){
                sem_post(&semaforo_anulaciones_adelantamiento_de_ronda);
            }
            else{
                sem_post(&semaforo_pagos_adelantamiento_de_ronda);
            }
        }
        else{
        sem_post(&semaforo_pagos_esperando_a_adelantar);
        sem_post(&semaforo_anulaciones_esperando_a_adelantar);
        //PRIORIDADES IDEA 1
        sem_wait(&semaforo_consultas_dentro);
        sem_wait(&semaforo_esperando);
        if(esperando!=0 && consultas_dentro==0){
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
        sem_post(&semaforo_esperando);
        sem_post(&semaforo_consultas_dentro);
        //PRIORIDADES IDEA 1
        sem_wait(&semaforo_repetir);
        sem_wait(&semaforo_esperando);
        if(esperando==0 && repetir==1){
            sem_post(&semaforo_esperando);
            sem_post(&semaforo_repetir);
                respuesta.tipo = 2;
                respuesta.id_nodo = mi_id;
                respuesta.ticket = 0;
                sem_wait(&semaforo_primer_proceso_general);
                primer_proceso_general=1;
                sem_post(&semaforo_primer_proceso_general);
                for (i = 0; i < num_pend; i++)
                {               
                    respuesta.puerto = puertos_pendientes[i];
                    if(peticiones_pendientes_de_consultas[i]!=2){
                        if (msgsnd(id_nodos_pendientes[i], &respuesta, longitud_respuesta, 1) == -1){ // 1 es no bloqueante
                        exit(-1);
                        }
                    }
                    peticiones_pendientes_de_consultas[i]=0;
                }
                sem_wait(&semaforo_oks_a_consultas_ya_mandados);
                oks_a_consultas_ya_mandados=0;
                sem_post(&semaforo_oks_a_consultas_ya_mandados);
                sleep(1);//>>para poder observar las prioridades
                sem_wait(&semaforo_repetir);
                repetir=0;
                sem_post(&semaforo_repetir);
                sem_wait(&semaforo_num_pendientes);
                num_pend = 0;
                sem_post(&semaforo_num_pendientes);

                sem_wait(&semaforo_no_volver_a_permitir_adelantamientos);
                no_volver_a_permitir_adelantamientos=0;//////
                sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
                sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
                permitiendo_adelantamiento_de_ronda=0;//////
                sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
                //printf("acabamos\n");
                sem_post(&semaforo_no_hacer_requests);
        
        }
        else{
            sem_post(&semaforo_esperando);
            sem_post(&semaforo_repetir);
        }
        }

        sem_wait(&semaforo_quiero);
        if(quiero==0){
            sem_post(&semaforo_quiero);
            sem_wait(&semaforo_primer_proceso_general);
            primer_proceso_general=1;
            sem_post(&semaforo_primer_proceso_general);
            sem_wait(&semaforo_primero);
            primero=1;
            sem_post(&semaforo_primero);
        }
        else{
            sem_post(&semaforo_quiero);
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
    struct mensaje mensaje_recibido;
    struct mensaje respuesta;
    struct info_a_mandar_al_hilo *aux;
    longitud_peticion = sizeof(mensaje_recibido) - sizeof(mensaje_recibido.tipo);
    longitud_respuesta = sizeof(respuesta) - sizeof(respuesta.tipo);

    //printf("diferencia en receptor: %d\n",diferencia);

    while (1)
    {
        // RECIBIR MENSAJE AJENA

        if (msgrcv(mi_id, &mensaje_recibido, longitud_peticion, 0, 0) == -1) // RECIBE TODOS LOS MENSAJES QUE LE LLEGUEN
        {                           
            exit(-1);
        }
        /*srand(mi_id);
        sleep(rand() % 2);*/
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

            sem_post(&semaforo_max_ticket);

            sem_wait(&semaforo_quiero);
            sem_wait(&semaforo_ticket);
            sem_wait(&semaforo_mi_id);//////////////////////////////////
            sem_wait(&semaforo_num_pendientes);
            //sem_wait(&semaforo_nodos_pendientes);

            /*printf("Quiero: %i\n",quiero);
            printf("Mi ticket: %i, ticket origen: %i\n",mi_ticket,ticket_origen);
            printf("Mi id: %i, id origen: %i\n",mi_id,id_nodo_origen);*/
            sem_wait(&semaforo_antes_de_entrar);
            if (quiero==0 || (ticket_origen < mi_ticket) || (ticket_origen == mi_ticket && (id_nodo_origen < mi_id)) || (prioridad_origen==4 && consultas_dentro!=0 && num_pend==0 && antes_de_entrar==0))//esto pa varias consultas en diferentes nodos
            {
                sem_post(&semaforo_num_pendientes);
                sem_post(&semaforo_mi_id);
                sem_post(&semaforo_ticket);
                sem_post(&semaforo_quiero);


                sem_post(&semaforo_antes_de_entrar);
                sem_wait(&semaforo_no_volver_a_permitir_adelantamientos);
                sem_wait(&semaforo_alguien_dentro);
                sem_wait(&semaforo_quiero);
                sem_wait(&semaforo_consultas_esperando_la_seccion_critica);
                if(no_volver_a_permitir_adelantamientos==0 && alguien_dentro==0 && quiero!=0 && consultas_esperando_la_seccion_critica==0){
                    sem_post(&semaforo_consultas_esperando_la_seccion_critica);
                    sem_post(&semaforo_quiero);
                    sem_post(&semaforo_alguien_dentro);
                    sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
                    sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
                    permitiendo_adelantamiento_de_ronda = 1;
                    sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
                }
                else{
                    sem_post(&semaforo_consultas_esperando_la_seccion_critica);
                    sem_post(&semaforo_quiero);
                    sem_post(&semaforo_alguien_dentro);
                    sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
                }
                sem_wait(&semaforo_mi_id);
                respuesta.id_nodo = mi_id;
                sem_post(&semaforo_mi_id);
                respuesta.tipo = 2;
                respuesta.puerto = puerto_origen;
                sem_wait(&semaforo_ticket);
                respuesta.ticket = mi_ticket;
                sem_post(&semaforo_ticket);
                if (msgsnd(id_nodo_origen, &respuesta, longitud_respuesta, 1) == -1) // 1 es no bloqueante
                    exit(-1);
            }
            else
            {   
                sem_post(&semaforo_num_pendientes);
                sem_post(&semaforo_mi_id);
                sem_post(&semaforo_ticket);
                sem_post(&semaforo_quiero);

                sem_post(&semaforo_antes_de_entrar);
                sem_wait(&semaforo_primero);
                primero = 1;///
                sem_post(&semaforo_primero);
                sem_wait(&semaforo_repetir);
                repetir = 1;///
                sem_post(&semaforo_repetir);
                sem_wait(&semaforo_esperar);
                esperar = 1;///
                sem_post(&semaforo_esperar);
                sem_wait(&semaforo_no_volver_a_permitir_adelantamientos);
                sem_wait(&semaforo_alguien_dentro);
                if(no_volver_a_permitir_adelantamientos==0 && alguien_dentro==0){
                    sem_post(&semaforo_alguien_dentro);
                    sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
                    sem_wait(&semaforo_permitiendo_adelantamiento_de_ronda);
                    permitiendo_adelantamiento_de_ronda = 1;
                    sem_post(&semaforo_permitiendo_adelantamiento_de_ronda);
                }
                else{
                    sem_post(&semaforo_alguien_dentro);
                    sem_post(&semaforo_no_volver_a_permitir_adelantamientos);
                }
                sem_wait(&semaforo_num_pendientes);
                id_nodos_pendientes[num_pend] = id_nodo_origen;
                puertos_pendientes[num_pend] = puerto_origen;//////se puede quitar
                sem_post(&semaforo_num_pendientes);
                if(prioridad_origen==4){
                    sem_wait(&semaforo_num_pendientes);
                    peticiones_pendientes_de_consultas[num_pend] = 1;
                    sem_post(&semaforo_num_pendientes);
                    sem_wait(&semaforo_oks_a_consultas_ya_mandados);
                    oks_a_consultas_ya_mandados=0;
                    sem_post(&semaforo_oks_a_consultas_ya_mandados);
                }
                sem_wait(&semaforo_num_pendientes);
                num_pend++;
                sem_post(&semaforo_num_pendientes);
            }
            //sem_post(&semaforo_nodos_pendientes);
        }
        else{
            cuenta++;
            if(cuenta==(diferencia)){//ya recibio todos los oks
                cuenta=0;
                sem_post(&semaforo_primero_dentro);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    struct info_a_mandar_al_hilo info;
    int i = 0;
    int numero_procesos = 0;
    long prioridad_del_proceso = 0;
    pthread_t thread_receptor;

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

    sem_init(&semaforo_primero,0,1);
    sem_init(&semaforo_repetir,0,1);
    sem_init(&semaforo_esperar,0,1);
    sem_init(&semaforo_primer_proceso_general,0,1);
    sem_init(&semaforo_consultas_esperando,0,1);
    sem_init(&semaforo_consultas_dentro,0,1);
    sem_init(&semaforo_antes_de_entrar,0,1);
    sem_init(&semaforo_hizo_requests_consultas,0,1);
    sem_init(&semaforo_permitiendo_adelantamiento_de_ronda,0,1);
    sem_init(&semaforo_no_volver_a_permitir_adelantamientos,0,1);
    sem_init(&semaforo_anulaciones_esperando_a_adelantar,0,1);
    sem_init(&semaforo_pagos_esperando_a_adelantar,0,1);
    sem_init(&semaforo_ultimo_adelantando,0,1);
    sem_init(&semaforo_alguien_dentro,0,1);
    sem_init(&semaforo_oks_a_consultas_ya_mandados,0,1);
    sem_init(&semaforo_esperando,0,1);
    sem_init(&semaforo_consultas_esperando_la_seccion_critica,0,1);
    sem_init(&semaforo_coordinador_main_hilos,0,0);
    // INICIAR SEMAFOROS

    if (argc != 6) {
        printf("Usage: %s [nodo_minimo] [nodo_maximo] [node_id] [instrucciones] [tiempos]\n", argv[0]);
        return -1;
    }

    int nodo_minimo = atoi(argv[1]);
    int nodo_maximo = atoi(argv[2]);
    mi_id = atoi(argv[3]);
    char* instrucciones = argv[4];
    char* tiempos = argv[5];

    int longitud_instrucciones = 0;
    // Array para almacenar las instrucciones como enteros.
    int instr_arr[MAX_INSTR_LENGTH];
    int instr_count = 0;

    // Dividir la cadena de instrucciones en subcadenas separadas por comas y convertirlas en enteros.
    char* token = strtok(instrucciones, ",");
    while (token != NULL && instr_count < MAX_INSTR_LENGTH) {
        // Omitir los espacios en blanco antes del número.
        while (*token== ' ') {
            token++;
        }
        int instr = atoi(token);
        longitud_instrucciones++;
        instr_arr[instr_count++] = instr;
        token = strtok(NULL, ",");
    }

    // Array para almacenar los tiempos como enteros.
    int tiempos_arr[MAX_INSTR_LENGTH];
    int tiempos_count = 0;

    // Dividir la cadena de tiempos en subcadenas separadas por comas y convertirlas en enteros.
    char* token_tiempo = strtok(tiempos, ",");
    while (token_tiempo != NULL && tiempos_count < MAX_INSTR_LENGTH) {
        // Omitir los espacios en blanco antes del número.
        while (*token_tiempo == ' ') {
            token_tiempo++;
        }
        int tiempo = atoi(token_tiempo);
        tiempos_arr[tiempos_count++] = tiempo;
        token_tiempo = strtok(NULL, ",");
    }

    // INICIAR ID_NODOS
    int a = 0;
    for (i = nodo_minimo; i <= nodo_maximo; i++)
    {
        if (i != mi_id)
        {
            id_nodos[a] = i;
            a++;
        }
    }
    diferencia = (nodo_maximo - nodo_minimo); //buzon 0 es de nodo 0 buzon 1 es el buzon interno del nodo 0
    // INICIAR ID_NODOS

    // INICIAR THREADS
    pthread_create(&thread_receptor, NULL, receptor, NULL);//esto crea el receptor y le pasa la struct con la diferencia
    
    int contador = 0;

    //INICIAR HILOS
    for (int j = 0; j < longitud_instrucciones; j += 4) {
        
        //printf("j: %d\n", j);

        // Extraer cuatro elementos del array de instrucciones.
        int instr1 = instr_arr[j];
        int instr2 = instr_arr[j+1];
        int instr3 = instr_arr[j+2];
        int instr4 = instr_arr[j+3];

        // Extraer un elemento del array de tiempos.
        int tiempo = tiempos_arr[j/4];

        // Mostrar los resultados.
        //printf("Instrucciones: %d %d %d %d, Tiempo: %d\n", instr1, instr2, instr3, instr4, tiempo);
        prioridad_del_proceso=1;
        for(a=0; a<instr1; a++){
            //printf("prioridad: %li\n",prioridad_del_proceso);
            contador++;
            pthread_create(&thread_receptor, NULL, proceso, (void *)prioridad_del_proceso);
            puerto=contador;
            sem_wait(&semaforo_coordinador_main_hilos);
        }
        prioridad_del_proceso=2;
        for(a=0; a<instr2; a++){
            //printf("prioridad: %li\n",prioridad_del_proceso);
            contador++;
            pthread_create(&thread_receptor, NULL, proceso, (void *)prioridad_del_proceso);
            puerto=contador;
            sem_wait(&semaforo_coordinador_main_hilos);
        }
        prioridad_del_proceso=3;
        for(a=0; a<instr3; a++){
            //printf("prioridad: %li\n",prioridad_del_proceso);
            contador++;
            pthread_create(&thread_receptor, NULL, proceso, (void *)prioridad_del_proceso);
            puerto=contador;
            sem_wait(&semaforo_coordinador_main_hilos);
        }
        prioridad_del_proceso=4;
        for(a=0; a<instr4; a++){
            //printf("prioridad: %li\n",prioridad_del_proceso);
            contador++;
            pthread_create(&thread_receptor, NULL, proceso, (void *)prioridad_del_proceso);
            puerto=contador;
            sem_wait(&semaforo_coordinador_main_hilos);
        }
        sleep(tiempo);

    }
    /*for(a = 0; a<numero_procesos; a++){
        info.puerto = a+1;
        if(a<4){
            info.prioridad = 1;
        }
        else if(a<8 && a>=4){
            info.prioridad = 2;
        }
        else if(a<12 && a>=8){
            info.prioridad = 3;
        }
        else{
            info.prioridad = 4;
        }
        pthread_create(&thread_receptor, NULL, proceso, (void *)&info);
        sleep(1);
    }*/
    sleep(300);
    return -1;
}