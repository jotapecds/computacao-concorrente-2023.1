#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// -- DEFINICOES --
#define NUM_THREADS 2
#define TAM_VETOR 10000

// -- VARIAVAEIS GLOBAIS --
int* vetor;

// -- FUNCOES AUXILIARES --
void preenche_vetor_aleatorio(int* vet)
{
    for(int i=0; i<TAM_VETOR; i++)
    {
        *(vet + i) = rand() % 50; // Número aleatório de 0 a 50 
    }
}

void print_vetor()
{
    for(int i=0; i<TAM_VETOR; i++)
    {
        printf("> Vetor[%d]: %d\n", i, *(vetor + i));
    }
}

int* multiplica_sequencial(int* vet)
{
    int* result = (int*) malloc(TAM_VETOR * sizeof(int));

    if(result == NULL) 
    {
        printf("-- ERRO DE ALOCACAO --\n");
        exit(-1);
    }

    for(int i=0; i<TAM_VETOR; i++)
    {
        *(result + i) = *(vet + i) * 2;
    }

    return result;
}

int verifica_resultado(int* vet_resultado)
{
    for(int i=0; i<TAM_VETOR; i++)
    {
        if(*(vetor+i) != *(vet_resultado+i))
        {
            printf("-- ERRO NA VERIFICACAO. ALGO DE ERRADO NAO ESTA CERTO :(\n");
            exit(-1);
        }
    }

    printf("-- TUDO CERTINHO, MEU PATRÃO!! :D\n");
}

// -- FUNCOES DE THREADS --
void* multiplica(void* thread_id) 
{
    for(int i=(int)thread_id; i<TAM_VETOR; i+=NUM_THREADS)
    {
        *(vetor + i) *= 2; 
    }

    pthread_exit(NULL);
}

// -- MAIN --
int main() 
{
    pthread_t threads[NUM_THREADS];
    vetor = (int*) malloc(TAM_VETOR * sizeof(int));
    int* vetor_base = (int*) malloc(TAM_VETOR * sizeof(int));

    if(vetor == NULL || vetor_base == NULL) 
    {
        printf("-- ERRO DE ALOCACAO --\n");
        exit(-1);
    }

    preenche_vetor_aleatorio(vetor);
    
    vetor_base = multiplica_sequencial(vetor); // Registrando o resultado esperado para verificar a corretude posteriormente

    for (int i=0; i<NUM_THREADS; i++) 
    {
        if(pthread_create(&threads[i], NULL, multiplica, (void*) i)) // Criando as threads
        {
            printf("-- ERRO AO CRIAR THREAD --\n");
            exit(-1);
        }
    }

    for (int i=0; i<NUM_THREADS; i++) 
    {
        pthread_join(threads[i], NULL); // Aguardando todos os fluxos antes de prosseguir com a main
    }

    verifica_resultado(vetor_base);

    return 0;
}