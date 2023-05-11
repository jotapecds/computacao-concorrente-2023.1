/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: as threads A so podem imprimir "Bye" depois que a thread B imprimir  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Variaveis globais */
pthread_mutex_t mutex;
pthread_cond_t cond1, cond2;
int count = 0;

void *t1 (void *arg) {
    printf("Oi Maria!\n");

    pthread_mutex_lock(&mutex);
    if(++count == 2) {
        pthread_cond_signal(&cond1);
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void *t2 (void *arg) {
    printf("Oi José!\n");

    pthread_mutex_lock(&mutex);
    if(++count == 2) {
        pthread_cond_signal(&cond1);
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void *t3 (void *arg) {
    pthread_mutex_lock(&mutex);
    while(count < 2){
        pthread_cond_wait(&cond1, &mutex);
    }
    
    printf("Sente-se por favor.\n");

    count++;
    pthread_cond_broadcast(&cond2);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
    
}

void *t4 (void *arg) {
    pthread_mutex_lock(&mutex);
    while(count < 3){
        pthread_cond_wait(&cond2, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("Até mais José!\n");
    pthread_exit(NULL);
}

void *t5 (void *arg) {
    pthread_mutex_lock(&mutex);
    while(count < 3){
        pthread_cond_wait(&cond2, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("Até mais Maria!\n");
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    pthread_t threads[5];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init (&cond1, NULL);
    pthread_cond_init (&cond2, NULL);

    /* Cria as threads */
    pthread_create(&threads[0], NULL, t1, NULL);
    pthread_create(&threads[1], NULL, t2, NULL);
    pthread_create(&threads[2], NULL, t3, NULL);
    pthread_create(&threads[3], NULL, t4, NULL);
    pthread_create(&threads[4], NULL, t5, NULL);

    /* Espera todas as threads completarem */
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);

    return 0;
}
