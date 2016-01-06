/*!
 <PRE>
 SOURCE FILE : main.c
 DESCRIPTION.: Este programa contem a implementação do algoritmo grasp e vns
               utilizado nos trabalhos da disciplina metaheuristica.
 AUTHOR......: Jeferson Queiroga Pereira
  CHANGES.....: Precisa concerta o paramentros para a entrada de um novo arquivo
               que não esteja na base     
 OBS.........: Part of the XXX Project.

 LAST UPDATE.: No Novembro 7th, 2015.
 </pre>
*/


#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include "Leitura.h"
#include "Grasp.h"
#include "Tool.h"
#include "buscaLocal.h"


void vnd(int ncities,int * s, int *custo);

int main () {
   srand(time(NULL));  
  
   //  float pOtima; 
  int i,j;
  int resultado;
  double res;
  int *s;
  int *bestRota;
  int l=0;
  int k=0;
  clock_t tInicio, tFim;
  clock_t tempoRodada1, tempoRodada2;
  double tempoTotal;
  double tempo_gasto;
  int otima=0;
  int soma;
  int custo;
  int melhorCusto;
  melhorCusto=1000000;
  tempo_gasto=0;
  bool entrou3opt;
  bool encerrar; 
  int opcao;  
  int *vetSolucao;
  char nomeTeste[30];
  char *diretorioArquivo;
  int z=0;   
  int *sfinal;
  int tempoEntrada=0;
  float coeficiente;
  int y; 

  
  int resultadoOld=0;
  bool mudou=true;  
  
  //tempo
  double ti,tf,tempo; // ti = tempo inicial // tf = tempo final
  ti = tf = tempo = 0; 
  timeval tempo_inicio,tempo_fim; 

  printf("############ TSP - GRASP-VNS ##############\n");  
  printf("1  - Bayg29     2 - att48    3  - berlin52   4 - KroA100\n");  
  printf("5  - KroB100    6 - Lin105   7  - bier127    8 - pr144\n");  
  printf("9  - kroB150    10 - ch150   11 - pr152     12 - brazil58\n");  
  printf("13 - eil76      14 -brg180   15 - d198      16 - gr202\n");  
  printf("17 - pr264      18 -pr299    19 - gr431     20 - rat575\n");
  printf("------>>>>>>>>> PARA ADICIONAR OUTRA CIDADE DIGITE: 30 \n");
  printf("-- Digite a Opcao da cidade: ");
  scanf("%d",&opcao);
  

  tempoEntrada=1200;
  coeficiente=0.4;

 
  switch (opcao){
        case 1:
            strcpy(nomeTeste,"bayg29");
            otima=1610;             
            break;
        case 2:
           otima=10610;               
           strcpy(nomeTeste,"att48");             
            
            break;
        case 3:
           otima=7542;    
            strcpy(nomeTeste,"berlin52");                
          
            break;
        case 4:
           otima=21282;    
          strcpy(nomeTeste,"kroA100");                
            
            break;
        case 5:
           otima=22141;               
           strcpy(nomeTeste,"kroB100");                       
           break;
        case 6:
           otima=14379;               
           strcpy(nomeTeste,"lin105");                       
           break;   
        case 7:
           otima=118282;
           strcpy(nomeTeste,"bier127");                       
           break;      
        case 8:
           otima=58537;    
           strcpy(nomeTeste,"pr144");                       
           break;   
         case 9:
           otima=26130;               
           strcpy(nomeTeste,"kroB150");                       
           break;   
         case 10:
           otima=6528;    
           strcpy(nomeTeste,"ch150");                       
           break;   
         case 11:
           otima=73682;    
           strcpy(nomeTeste,"pr152");                       
           break;   
         case 12:
           otima=25395;    
           strcpy(nomeTeste,"brazil58");                       
           break;   
         case 13:
           otima=538;    
           strcpy(nomeTeste,"eil76");                       
           break;   
         case 14:
           otima=1950;    
           strcpy(nomeTeste,"brg180");                       
           break;      
         case 15:
           otima=15780;    
           strcpy(nomeTeste,"d198");                       
           break;        
         case 16:
           otima=40160;    
           strcpy(nomeTeste,"gr202");                       
           break;          
         case 17:
           otima=49135;    
           strcpy(nomeTeste,"pr264");                       
           break;
         case 18:
           otima=48191;    
           strcpy(nomeTeste,"pr299");                       
           break;     
         case 19:
           otima=171414;    
           strcpy(nomeTeste,"gr431");                       
           break;     
         case 20:
           otima=6773;
           strcpy(nomeTeste,"rat575");                       
           break; 
         case 30:
           printf("-- Digite o nome do arquivo - sem  extensao: ");
           scanf("%s",nomeTeste);
           printf("-- Digite o valor do ótimo da instancia:");
           scanf("%d",&otima);
           break;                     
 } 
 


  char diretorio[50];
  printf("Aguarde...");
  strcpy(diretorio,"instancias/");
  strcat(diretorio,nomeTeste);
  strcat(diretorio,".tsp");
  readInstance(diretorio);


  s=(int *) malloc((ncities)*sizeof(int));
 // bestRota=(int*)malloc((ncities)*sizeof(int));
     
   gettimeofday(&tempo_inicio,NULL); 
   tInicio = clock(); 
   
   //Chamando a parte construtiva do grasp
   constroi_solucao(ncities,s,distMat,coeficiente);          
 

 //INicio do VNS  
 while(melhorCusto>otima && tempo_gasto<tempoEntrada){ //tempo_gasto<900 //&& <1000
     //Parte na qual chama o grasp
     
     vetSolucao=(int*)malloc((ncities)*sizeof(int));
    
     
     opcao=1;
    
     while (opcao<=2)
     {
         

          for (i = 0; i <ncities; i++)
              vetSolucao[i] = s[i];
              
          
          switch(opcao) {
               case 1: pertuba2Opt(vetSolucao,ncities);
                     break;     
               case 2: pertuba3opt(vetSolucao,ncities);
                     break;
                    
          }
          custo=calcula_rota(ncities,vetSolucao);
          vnd(ncities,vetSolucao,&custo);                   
          if(custo<melhorCusto){
              for(i=0;i<ncities;i++){
                s[i]=vetSolucao[i];
              } 
              //passando o melhor custo
              melhorCusto=custo;
              opcao=1;
               tempoRodada1=clock();
               
          }                   //caso opt e 3 tenha encontrado um minino local, saia do while.
          else{
               opcao++;     
          }
          
        
     }//fim-do-while
     
       
      // printf("melhor solucao - %d \n",melhorCusto);      
     //-------------------------  
       z++;  
       tFim = clock(); 
      
      //Segunda maneira de medir o tempo
      gettimeofday(&tempo_fim,NULL); 
      tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0)); 
      ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0)); 
      tempo = (tf - ti) / 1000;

      tempo_gasto =(double)((tFim - tInicio) / (CLOCKS_PER_SEC)); 
}   
//-----------------------------------------------------------------------------------------------
       sfinal=(int *) malloc((ncities+1)*sizeof(int));
       for(i=0;i<=ncities;i++){
          sfinal[i]=s[i];
       }   
       sfinal[ncities]=sfinal[0];   
//-----------------------------------------------------------------------------------------------          
          
          printf("\n \n------------------------------------------------\n");
          printf("- Instancia %s \n",nomeTeste);
          printf("- MELHOR SOLUCAO DA LITERATURA: %d \n",otima); 
          printf("- SOLUCAO OBTIDA: %d \n",melhorCusto); 
          res=(double)melhorCusto/otima-1;
          printf("Afastamento Percentual da Solucao Otima: %.3lf \n",res*100);
          printf("Tempo de Execucao: %.3f \n",tempo/1000);
          printf("ROTA: \n ");
          for(i=0;i<=ncities;i++){
                printf("-%d-",sfinal[i]);
          } 
          printf("\n------------------------------------------------\n");
          
//---------------------------------GRAVAR EM ARQUIVO------------------------------------------------           
  
   
    //printf("Digite o nome do arquivo");
  //  scanf("%s",&nomeArquivo);
   // strcat(diretorio,nomeArquivo);
     
   // strcpy (diretorio,"\\resultado");
    //strcat (diretorio,nomeTeste);
   
     //fprintf(stdout,"resultado\\");
     sprintf(diretorio,"testes/%s.txt",nomeTeste);
     FILE *fp; 
     fp=fopen (diretorio,"a");
     if (!fp){
         printf ("Erro na abertura do arquivo.");
     }
     else{
     
       fprintf(fp,"%d;%.3f",melhorCusto,tempo/1000);   
       fprintf(fp,"\n");     
       fclose(fp);            
          
     }
  
  

//-----------------------------------------------------------------------------  
      free(s);
     
      return 0;
}

void vnd(int ncities, int * s, int *fo){
     int *sl;
     int i;
     int fosl;
     int opcao;
     sl=(int *) malloc((ncities)*sizeof(int));
     
     for(i=0;i<ncities;i++)
     {
          sl[i]=s[i];          
     }
     fosl=*fo;
     
     opcao=1;
     
     while(opcao<=2){
       switch(opcao){
       
       case 1: doisOpt(ncities,sl);
               break;
       case 2: tresOpt(sl,ncities);
               break;
       }
     
       fosl=calcula_rota(ncities,sl);
     
       if(fosl<*fo){
          for(i=0;i<ncities;i++){
                s[i]=sl[i];
          }
          opcao=1;        
          *fo=fosl;
       }
        opcao++;   
     }

     free(sl);
}


void ehCiclo(int *s, int n ){
     int i;
     int *visitados;
     bool aux;
     aux=false;
     
     visitados=(int *) malloc((ncities+1)*sizeof(int));
     for(i=0;i<=n;i++){
          visitados[i]=0;     
     }
     for(i=0;i<ncities;i++){
          visitados[s[i]]++;     
     }
     
     for(i=0;i<ncities;i++){
          if(visitados[i]!=1){
               aux=true;
          }     
     }
     
     if(aux){
          printf("Não eh um ciclo hamiltoniano");     
     }
         
}

















