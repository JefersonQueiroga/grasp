#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include "Tool.h"
#include "Estruturas.h"
#include "Listas.h"
#include "Leitura.h"



/* cria memoria para um vetor de tam posicoes */
int *cria_vetor(int tam)
{
  int *vetor;

  vetor = (int *) malloc(tam*sizeof(int));
  if (!vetor){
  	printf("Falta memoria para alocar o vetor de ponteiros");
    exit(1);
  }
  return vetor;
}



/* calcula a funcao objetivo */
int calcula_rota(int n, int *s)
{
    int dist_percorrida = 0;

    for ( int j=0; j < n-1; j++ ) {
//      printf("+d[%2d,%2d] = %f \n",s[j],s[j+1],distancia[s[j]][s[j+1]]);
      dist_percorrida += distance(s[j],s[j+1]);
    }
    dist_percorrida += distance(s[n-1],s[0]);
//    printf("Distancia=%f \n",dist_percorrida);
    return dist_percorrida;
}


/* Gera numero aleatorio entre min e max */
float randomico(float min, float max)
{
  if (min == max) return min;
  return ((float) (rand()%10000/10000.0)*(max-min) + min);
}



/* Procura a posição da cidade dada no vetor */
int busca_pos_valor(int *vetor, int cidade, int n)
{
  int i = 0;
  while (vetor[i] != cidade && i < n) i++;
  return i;
}

/* Retorna se uma cidade já foi inserida no vetor */
bool foiInserida(int *vetor, int cidade, int n)
{
  for (int i = 0; i < n; i++)
    if (vetor[i] == cidade) return true;
  return false;
}


/* Atualiza a matriz de arestas */
void atualiza_arestas(int **arestas, int n, int m, int prox_cid)
{
  for (int i = 0; i < n; i++)
    for (int j = 1; j < m; j++)
      if (prox_cid == arestas[i][j]) {
        arestas[i][j] = -1;
        arestas[i][0] -= 1;
      }
}

/* Esta rotina devolve o individuo escolhido pelo mecanismo da roleta russa */
int roleta(int nind, int *fo_pop)
{
  register int j;
  double aux;
  double *escala, *fracao;
  int escolhido;
  
  //for(int i=0; i<nind ; i++)
 // printf("elementos roleta: %d \n", fo_pop[i]);   

  fracao = cria_vetor_double(nind);
  escala = cria_vetor_double(nind);
  int soma = 0;
  for (int j=0; j < nind; j++){
    soma = fo_pop[j] + soma;
   // printf("fo_pop[%d] = %d \n",j,fo_pop[j]);
  }
  for (int j=0; j < nind; j++){
    fracao[j] = (double) fo_pop[j]/soma;
    //printf("fracao[%d] = %f \n",j,fracao[j]);
  }
  escala[0] = fracao[0]; 
   //printf("escala[%d] = %f \n",0,escala[0]);
  for (int j=1; j < nind; j++){
    escala[j] = escala[j-1] + fracao[j];
    printf("escala[%d] = %f \n",j,escala[j]);
  }


  aux = closed_interval(0,1);
  printf("Print aux %f \n",aux);  
   
  j = 0;

  while (escala[j] < aux){//mudança feita para verificar
     printf("escala  %f \n ",escala[j]);
     j++;
  }
    
     escolhido = j;
 
 // free(escala);//modificacoes 
 // free(fracao); 

  return escolhido;
}



/* cria memoria para um vetor de tam posicoes com elementos do tipo double */
double *cria_vetor_double(int tam)
{
  double *vetor;

  vetor = (double *) malloc(tam*sizeof(double));
  if (!vetor){
    printf("Falta memoria para alocar o vetor de ponteiros");
    exit(1);
  }
  return vetor;
}


double closed_interval(double a, double b)
{
     return ((b-a)*((double)rand()/RAND_MAX))+a;
}


//roleta2
int roleta2(int tamPopulacao, int *fo_pop){
      int i;
     
      // Número que será sorteado
      float numeroSorteado;
     
      // Responsável por fazer os somatórios
      float somatorio = 0;

      // Vetor com as porcentagens de cada elemento
      float porcentagemIndividuos[tamPopulacao];

      // Calculando o inverso () e o somatorio inicial das probabilidades
      for (i = 0; i < tamPopulacao; i++) {
          porcentagemIndividuos[i] = (float)(1.0f  / (float) fo_pop[i]);
          somatorio += porcentagemIndividuos[i];
      }

      // Calculando a probabilidade em si para cada elemento
      for (i = 0; i < tamPopulacao; i++) {
          porcentagemIndividuos[i] = (porcentagemIndividuos[i] / somatorio) * 100;
      }

      // Gera um numero REAL aleatório entre 0 e 100
      numeroSorteado = (float) geraNumero (0,99) + (geraNumero (0, 100) * 0.01);
    //  printf("numero sorteiado %f",numeroSorteado);
      // Reinicia a variável somatorio para reutiliza-la
      somatorio = 0;

     // Vai atualizando o valor de somatorio e escolhendo o elemento correspondente para retornar
      for (i = 0; i < tamPopulacao; i++) {
          somatorio += porcentagemIndividuos[i];
          if (numeroSorteado <= somatorio) {
             return i;
          }
      }
}

int geraNumero(int min , int max){
     int x;
    // srand(time(NULL));
     x=rand()%(max-min)+min;
     return x;     
}   

