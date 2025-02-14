#include <stdint.h>
#include <stdio.h>

#include "multitarefas.h"

/*
 * Prototipos das tarefas
 */
void tarefa_1(void);
void tarefa_2(void);
void tarefa_3(void);

/*
 * Configuracao dos tamanhos das pilhas
 */
#define TAM_PILHA_1		(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_2		(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_OCIOSA	(TAM_MINIMO_PILHA + 24)

/*
 * Declaracao das pilhas das tarefas
 */
uint32_t PILHA_TAREFA_1[TAM_PILHA_1];
uint32_t PILHA_TAREFA_2[TAM_PILHA_2];
uint32_t PILHA_TAREFA_OCIOSA[TAM_PILHA_OCIOSA];


/*
 * Sem�foro
 */
semaforo_t semaforo_a = {0,0};
semaforo_t semaforo_b = {1,0};

/*
 * Funcao principal de entrada do sistema
 */
int main(void)
{
	
	/* Criacao das tarefas */
	/* Parametros: ponteiro, nome, ponteiro da pilha, tamanho da pilha, prioridade da tarefa */
	
	CriaTarefa(tarefa_1, "Tarefa 1", PILHA_TAREFA_1, TAM_PILHA_1, 1);
	
	CriaTarefa(tarefa_2, "Tarefa 2", PILHA_TAREFA_2, TAM_PILHA_2, 2);
        
	/* Cria tarefa ociosa do sistema */
	CriaTarefa(tarefa_ociosa,"Tarefa ociosa", PILHA_TAREFA_OCIOSA, TAM_PILHA_OCIOSA, 0);
	
	/* Configura marca de tempo */
	ConfiguraMarcaTempo();   
	
	/* Inicia sistema multitarefas */
	IniciaMultitarefas();
	
	/* Nunca chega aqui */
	while (1)
	{
	}
}

#define BUFFER_SIZE     8

volatile uint16_t  buffer[BUFFER_SIZE];

// Consumidor
void tarefa_1(void)
{
	volatile uint16_t a = 0, readbuf = 0;
	for(;;)
	{       
          SemaforoAguarda(&semaforo_a); // wait(cheio)
          for(a = 0; a<BUFFER_SIZE;a++){
            readbuf = buffer[a];
          }
          SemaforoLibera(&semaforo_b); // signal(vazio)
	}
}

// Produtor
void tarefa_2(void)
{
	volatile uint16_t b = 0;
	for(;;)
	{
          
          SemaforoAguarda(&semaforo_b); // wait(vazio)
          for(b = 0; b<BUFFER_SIZE;b++){
            buffer[b] = rand()%100;
          }
          SemaforoLibera(&semaforo_a); // signal(cheio)
          
	}
}
