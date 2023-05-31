/*
 *  Programação Concorrente - Laboratório 6
 *  Aluno: João Pedro Coelho de Souza
 *  DRE: 119152051
 * 
 *  Objetivo: Considere o padrao produtor/consumidor com a seguinte alteração: 
 *  em uma inserção, os produtores preenchem completamente o buffer (para isso 
 *  o buffer precisa estar vazio). A logica dos consumidores permanece a mesma.
 *  Implemente as funções de insere e retira no buffer, seguindo a logica 
 *  descrita acima. Use semaforos para tratar todas as demandas de sincronização
 *  do problema (minimizando ao máximo os custos dessa sincronização). Considere
 *  que o número de threads produtoras e threads consumidores pode variar de 1 a
 *  N a cada execução.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* Definições e declarações de variáveis globais */

#define N 5         /* Tamanho da área de Buffer                            */

int NPROD;
int NCONS;   /* Número de threads produtoras e consumidoras          */
int buffer[N];      /* Área de dados compartilhada                          */
int buffer_livre = 1;      /* Booleano para lotação do buffer: 0->Não | 1->Sim     */
sem_t slots_vazios;
sem_t slots_ocupados;   /* Semaforos para controlar produtores e consumidores   */
sem_t mutex;        /* Semaforo binário para exclusão mutua                 */

/* Funções da Lógica Produtor Consumidor */

void insere(int dado, int tid)
{
    static int in = 0;  /* Indice de inserção no buffer */
    printf("Produtor #%d quer inserir.\n", tid);
    sem_wait(&slots_vazios);
    sem_wait(&mutex);
    if(buffer_livre == 1)
    {
        printf("Produtor #%d está inserindo.\n", tid);
        buffer[in] = dado;
        in++;
        if(in == N) 
        {
            in = 0;
            buffer_livre = 0;
            sem_post(&slots_ocupados);
        }    
    }
    sem_post(&slots_ocupados);
    sem_post(&mutex);
}

int retira(int tid)
{
    static int out = 0; /* Indice de remoção no buffer */
    printf("Consumidor #%d quer retirar.\n", tid);
    sem_wait(&slots_ocupados);
    sem_wait(&mutex);
    printf("Consumidor #%d está retirando.\n", tid);
    int dado = buffer[out];        
    if(out == N)
    {
        out = 0;
        buffer_livre = 1;
        for (int i = 0; i < N; i++){ sem_post(&slots_vazios); } 
    }
    if(buffer_livre == 0){ sem_post(&slots_ocupados); }

    sem_post(&mutex);
    return dado;
}

/* Funções das threads */

void* produzir(void* arg)
{
    int* prod_id = (int*) arg;
    printf("Rodando thread produtora #%d!\n", *prod_id);

    while(1)
    {
        /* Produzindo um dado aleatório */
        int dado = 0;
        for (int i = 0; i < 100; i++){ dado += rand()%10; }

        insere(dado, *prod_id);
    }
    pthread_exit(NULL);
} 

void* consumir(void* arg)
{
    int* cons_id = (int*) arg;
    int dado;
    printf("Rodando thread consumidora #%d!\n", *cons_id);
    while (1)
    {
        dado = retira(*cons_id);

        /* Processamento aleatório do dado obtido */
        for (int i = 0, count = 0; i < 10000; i++){ if(dado%i == 0){ count++; } }
    }
    pthread_exit(NULL);
}

/* Função main */

int main(int argc, char* argv[])
{
    /* Validação dos argumentos de entrada do programa */
    if(argc < 3)
    {
        printf(" >> [ERRO]: Argumentos insuficientes.\n");
        printf(" >> A chamada do programa deve ser da forma ./lab06 <NPROD> <NCONS> \n");
        exit(-1);
    }

    NPROD = atoi(argv[1]);
    NCONS = atoi(argv[2]);

    if(NPROD < 1) NPROD = 1;
    if(NCONS < 1) NCONS = 1;

    /* Inicialização dos semáforos */
    sem_init(&mutex, 0, 1);   
    sem_init(&slots_vazios, 0, N);
    sem_init(&slots_ocupados, 0, 0);

    /* Criação das threads produtoras e consumidoras */
    pthread_t threads_produtoras[NPROD];
    pthread_t threads_consumidoras[NCONS];

    for(int i = 0; i < NPROD; i++)
    {
        printf("Criando produtora #%d\n",i);
        if(pthread_create(&threads_produtoras[i], NULL, produzir, (void*)&i))
        {
            printf(">> [ERRO]: pthread_create().\n");
            exit(-1);
        }
    }

    for(int i = 0; i < NCONS; i++)
    {
        printf("Criando consumidora #%d\n",i);
        if(pthread_create(&threads_consumidoras[i], NULL, consumir, (void*)&i))
        {
            printf(">> [ERRO]: pthread_create().\n");
            exit(-1);
        }
    }

    /* Esperando todas as threads completarem suas tarefas */
    for (int i = 0; i < NPROD; i++) { pthread_join(threads_produtoras[i], NULL); }
    for (int i = 0; i < NCONS; i++) { pthread_join(threads_consumidoras[i], NULL);}

    return 0;
}