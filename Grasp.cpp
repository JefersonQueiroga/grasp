#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "Listas.h"
#include "Estruturas.h"
#include "Tool.h"
#include "Construcao.h"
#include "Leitura.h"


void constroi_solucao(int numCidade, int *s, int **d, float alpha)
{
    struct lista *inicio_LC, *final_LC, *nao_visitada;
    int tamanho_LC;
    int *vet;
    //printf("++++++++++++Teste");
    // s=NULL;
    /* Inicio da Fase de Construcao de uma solucao */
    inicio_LC = final_LC = NULL;
    int j;
    
     //Escolha do primeira cidade da rota, aleatoriamente.
      s[0]=1;  /* modifiquei */
      for (j=1; j<=numCidade; j++){//modifiquei
        nao_visitada = (struct lista *)malloc(sizeof(struct lista));
        if (!nao_visitada) {
           printf("Problema na alocação!!!\n");
           exit(1);
        }
      /* vou inserir um registro no final de uma lista  */
        if(s[0]!=j){
           nao_visitada->cidadeOrigem = j;
           nao_visitada->cidadeDestino = 0;
           insere_final_lista(nao_visitada, &inicio_LC, &final_LC);
        }
    }

    
    j = 1;
    int cidade_escolhida;
    
    while (j <numCidade){
      struct lista *registro = inicio_LC;
      tamanho_LC = tamanho_lista(&inicio_LC,&final_LC);

      //printf("Tamanho da lista de candidatos = %d \n",tamanho_LC);
      /* Ordenando a lista encadeada   */
      struct lista *inicio_LC_ordenada, *final_LC_ordenada;
      inicio_LC_ordenada = final_LC_ordenada = NULL;

      while (registro){
        nao_visitada = (struct lista *)malloc(sizeof(struct lista));
        if (!nao_visitada) {
           printf("Problema na alocação de memoria. \n");
           exit(1);
        }
        nao_visitada->cidadeOrigem = registro->cidadeOrigem;
        nao_visitada->cidadeDestino = registro->cidadeDestino;
        nao_visitada->distancia = distance(s[j-1],nao_visitada->cidadeOrigem);
        //nao_visitada->distancia = d[s[j-1]][nao_visitada->cidadeOrigem];
        /* vou inserir um registro de forma ordenada em uma lista  */
        insere_ordenado_lista(nao_visitada, &inicio_LC_ordenada, &final_LC_ordenada);
        registro = registro->proximo;   /* obtem próximo endereço */
      }
      //mostra_lista(&inicio_LC_ordenada, &final_LC_ordenada);

      int tamanho_LRC =(int)(tamanho_LC*alpha);
      //printf("--------Tamanho %d",tamanho_LRC);
      //printf("Tamanho da lista restrita de candidatos = %d \n",tamanho_LRC);
      vet = retorne_vetor(tamanho_LRC, &inicio_LC_ordenada);
      int posicao_escolhida;
      
      if(tamanho_LRC<1){
          //printf("=========entrou\n");     
          posicao_escolhida=0;
      }
      else{
          posicao_escolhida =roleta2(tamanho_LRC, vet );
      }
      //printf("\n-----Posicao Escolhida %d\n",posicao_escolhida);
      
      registro = encontra_registro_lista(posicao_escolhida,&inicio_LC_ordenada);
     
      cidade_escolhida = registro->cidadeOrigem;

      //printf("Cidade escolhida = %d \n\n",cidade_escolhida);

      /* Apaga a lista de candidatos ordenada */
      apaga_lista(&inicio_LC_ordenada, &final_LC_ordenada);
      //printf("Apagar Lista\n");
      /* Insere a cidade escolhida apos a ultima cidade inserida*/
      s[j] = cidade_escolhida;
      /* Apaga a cidade escolhida da lista de nao visitadas */
      registro = encontra_registro(cidade_escolhida, 0, &inicio_LC);
      //printf("Teste encontrar registro\n");
  //    printf("Ultima cidade %d \n" , registro->cidadeOrigem);
      apaga_registro(registro,&inicio_LC,&final_LC);
     // printf("Apagar registro\n");
      j++;
    }
    //s[j]=0];
     free(vet);
     free(inicio_LC);
     free(final_LC);

}
     


