#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "Tool.h"
#include "Estruturas.h"
#include "Listas.h"



void insere_ordenado_lista(struct lista *i,       
                           struct lista **inicio, 
                           struct lista **final)  
{
  struct lista *antigo, *ponteiro;

  if (*final==NULL) { 
     i->proximo = NULL;
     i->anterior = NULL;
     *final = i;
     *inicio = i;
     return;
  }
  ponteiro = *inicio; 
  antigo = NULL;
  while(ponteiro) {
    if(ponteiro->distancia < i->distancia) {
      antigo = ponteiro;
      ponteiro = ponteiro->proximo;
    }
    else {
      if(ponteiro->anterior) {
        ponteiro->anterior->proximo = i;
        i->proximo = ponteiro;
        i->anterior = ponteiro->anterior;
        ponteiro->anterior = i;
        return;
      }
      i->proximo = ponteiro; 
      i->anterior = NULL;
      ponteiro->anterior = i;
      *inicio = i;
      return;
    }
  }
  antigo->proximo = i;
  i->proximo = NULL;
  i->anterior = antigo;
  *final = i;
}


/* Insere o registro i no final de uma lista duplamente encadeada */
void insere_final_lista(struct lista *i,
                        struct lista **inicio,
		        struct lista **final)
{
  struct lista *aux, *ponteiro;

  if (*final == NULL) {
    i->proximo  = NULL;
    i->anterior = NULL;
    *final = i;
    *inicio = i;
    return;
  }
  ponteiro = *inicio;
  aux = NULL;
  while (ponteiro) {
    aux = ponteiro;
    ponteiro = ponteiro->proximo;
  }
  aux->proximo = i;
  i->proximo = NULL;
  i->anterior = aux;
  *final = i;
}


/* Mostra o conteudo da lista completa */
void mostra_lista(struct lista **inicio, /* primeiro elemento da lista */
	          struct lista **final)  /* último elemento da lista */
{
  struct lista *registro = *inicio;
  int cont = 0;

  if (!registro) printf("Lista vazia\n");
  while (registro) {
    printf("L[%2d]   cidadeOrigem = %2d  cidadeDestino = %d  distancia = %d\n",
       cont, registro->cidadeOrigem, registro->cidadeDestino, registro->distancia);
    registro = registro->proximo;   /* obtem próximo endereço */
    cont++;
  }
  printf("\n");
}


/* Mostra o comprimento da lista completa */
int tamanho_lista(struct lista **inicio, /* primeiro elemento da lista */
                  struct lista **final)  /* último elemento da lista */
{
  struct lista *registro = *inicio;
  int cont = 0;

  while(registro) {
    registro = registro->proximo;   /* obtem próximo endereço */
    cont++;
  }
  return cont;
}


/* Encontra o registro de numero posicao */
struct lista *encontra_registro_lista(int posicao,
                                      struct lista **inicio)
{
  struct lista *registro = *inicio;
  int contador = 0;

  while (registro) {
    if (contador == posicao) return registro;
    contador++;
    registro = registro->proximo;  /* obtem novo registro */
  }
  printf("Registro nao encontrado1.\n");
  return NULL; /* nao encontrou */
}


/* Encontra o registro em uma lista duplamente encadeada */
struct lista *encontra_registro(int cidadeOrigem,
                                int cidadeDestino,
                                struct lista **inicio)
{
  struct lista *registro = *inicio;

  while (registro) {
    if (registro->cidadeOrigem == cidadeOrigem && registro->cidadeDestino == cidadeDestino)
       return registro;
    registro = registro->proximo;  /* obtem novo registro */
  }
  printf("Registro nao encontrado2.\n");
  return NULL; /* nao encontrou */
}



/* Remove o elemento registro da lista */
void apaga_registro(struct lista *registro,
                    struct lista **inicio,
                    struct lista **final)
{
  if (registro) {
    if (*inicio==registro) {
       *inicio=registro->proximo;
       if (*inicio)
          (*inicio)->anterior = NULL;
       else *final = NULL;
    }
    else {
      registro->anterior->proximo = registro->proximo;
      if (registro!=*final)
         registro->proximo->anterior = registro->anterior;
      else
         *final = registro->anterior;
    }
    free(registro); /* devolve memoria para o sistema */
  }
}



/* Apaga TODA a lista */
void apaga_lista(struct lista **inicio,
                 struct lista **final)
{
    struct lista *registro = *inicio;
    while(registro) {
	apaga_registro(registro, inicio, final);
    	if (registro) registro = registro->proximo;   /* obtem próximo endereço */
    }
}



/* Retorna um vetor até k de tamanho */
int * retorne_vetor(int tamanho, struct lista **inicio)
{
  struct lista *registro = *inicio;
  int *vetor;
  int contador=0;
  vetor = (int*) malloc(tamanho * sizeof(int));

  while (contador<tamanho) {
    vetor[contador]=registro->cidadeOrigem;
    contador++;
    registro = registro->proximo;  /* obtem novo registro */
  }
  if(tamanho==0){
     vetor[0]=registro->cidadeOrigem;        
  }  

         
     return vetor; /* nao encontrou */
}
