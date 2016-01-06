void doisOpt( int ncities, int * s );
void pertuba3opt(int *s, int ncities);
void pertuba2Opt(int *s, int ncities);
void tresOpt(int *s, int ncities);



void doisOpt( int cid, int * s ) {

    int ncities=cid-1;
    int i, j, h, l, temp;
    int n_inner_loop;
    int melhora_flag = true;
    int num_exchanges = 0, num_scans = 0;
    int remove, add, melhora;

    while( melhora_flag ) {
	melhora_flag = false;
	num_scans++;
	for( i = 0 ; i < ncities-2 ; i++ ) {
	    if ( i ){
		  n_inner_loop = ncities;
        }  
	    else{
		  n_inner_loop = ncities - 1;
        }  
	    
        for( j = i+2 ; j < n_inner_loop ; j++ ) {
    	remove = distance(s[i],s[i+1]) + distance(s[j],s[j+1]);
		add   = distance(s[i],s[j]) + distance(s[i+1],s[j+1]);
        
		melhora= - remove + add;
		if( melhora < 0 ) {
		    melhora_flag = true;
		    num_exchanges++;
		    h = i + 1;
		    l = j;
		    while( l >= h) {
			temp=s[h];				
			s[h]=s[l];
			s[l]=temp;
			h++;
			l--;
		    }
		}
	    }
	}
    }
    
}


void pertuba3opt(int *s, int ncities){
     int A,A1;
     int B,B1;     
     int C,C1;
     A,A1,B,B1,C,C1=0;
     int *lista1, *lista2, *vetorI; 
     int j=0;
     int elementos=0;
     int tam=0;
     int tamAB;
     int tamBC;
     int PA1, PB1, PC1;
     int aux=0;
     int k,l, i;
     k,l = 0;
     
     
     do{
          A=rand()% ncities;     
     }while(A==ncities-2);


     A1=A+1;
     
     do{
          B=rand()%ncities;                    
     }while(B==ncities-2 || B==A || B==A1 );
     
     B1=B+1;
     
     do{
          C=rand()%ncities;     
     }while(C==ncities-2 || C==A || C==A1 || C==B || C==B1 );
     
          
         //colocando em ordem ;
         if(A>B) {aux=A; A=B; B=aux;}
         if(B>C) {aux=B; B=C; C=aux;}
         if(A>B) { aux=A; A=B; B=aux;}
         A1=A+1;
         B1=B+1;
         C1=C+1;     
         
                 
                            
         //alocacao
         tamAB=B-A;
         tamBC=C-B;
                 
         lista1=(int*) malloc((tamAB)*sizeof(int));
         lista2=(int*) malloc((tamBC) *sizeof(int));
         vetorI=(int*) malloc((tamBC) *sizeof(int));
      
         
         //prencher vetores;         
         k=0;
         l=0;
         for(i=0;i<ncities;i++){
               if(i>A && i<B1){
                    lista1[k]=s[i];
                    k++;    
               }
               if(i>B && i<C1){
                    lista2[l]=s[i];
                    l++;     
               }       
         }
                 
         
         
         //invertendo vetor;
         j = 0;
         for(i =tamBC-1; i >= 0; i--) {
              vetorI[j] = lista2[i];
              ++j;
         }
        
          
         
         l=0;
         k=0;
         for(i=0;i<ncities;i++){
               if(i>A && i<A+tamBC+1){
                    s[i]=vetorI[l];    
                    l++;
                }
                if(i>A+tamBC && i<C1){
                    s[i]=lista1[k];
                    k++;                  
               }       
         }
         
                 
        
         
         free(lista1);
         free(lista2);
         free(vetorI);
 
 
}




void tresOpt(int *s, int ncities){
     int vs;
     int vt;
     int i,j,k,l;
     int a,a1,b,b1,c,c1;
     int *lista1, *lista2, *vetorI; 
     int elementos=0;
     int tam=0;
     int tamAB,tamBC;
     int PA1, PB1, PC1;
     int aux=0;
     int ma, mb,mc;
   //  printf("Teste--------------------");
     vs=calcula_rota(ncities,s);
     for(a=0,a1=1;a<ncities-5,a1<ncities-4;a++,a1++){
          for(b=a1+1,b1=a1+2;b<ncities-3,b1<ncities-2;b++,b1++){
               for(c=b1+1,c1=b1+2;c<ncities-1,c1<ncities;c++,c1++){
                    vt=vs - distance(a,a1)-distance(b,b1)-distance(c,c1) + distance(a,c) + distance(b1,a1)+ distance(b,c1);    
                    if(vt<vs){

                         ma=a;
                         mb=b;
                         mc=c;  
                         vs=vt;    
                    }
               }
          }
     } 
  
                              
                 
                            
         //alocacao
         tamAB=b-a;
         tamBC=c-b;

         a1=a+1;
         b1=b+1;
         c1=c+1;
                 
         lista1=(int*) malloc((tamAB)*sizeof(int));
         lista2=(int*) malloc((tamBC) *sizeof(int));
         vetorI=(int*) malloc((tamBC) *sizeof(int));
      
         
         //prencher vetores;         
         k=0;
         l=0;
         for(i=0;i<ncities;i++){
               if(i>a && i<b1){
                    lista1[k]=s[i];
                    k++;    
               }
               if(i>b && i<c1){
                    lista2[l]=s[i];
                    l++;     
               }       
         }
                 
         
         
         //invertendo vetor;
         j = 0;
         for(i =tamBC-1; i >= 0; i--) {
              vetorI[j] = lista2[i];
              ++j;
         }
        
          
         
         l=0;
         k=0;
         for(i=0;i<ncities;i++){
               if(i>a && i<a+tamBC+1){
                    s[i]=vetorI[l];    
                    l++;
                }
                if(i>a+tamBC && i<c1){
                    s[i]=lista1[k];
                    k++;                  
               }       
         }
         
                 
        
         
         free(lista1);
         free(lista2);
         free(vetorI);
 
 
}


void pertuba2Opt(int *s, int ncities){
     int a,a1;
     int b,b1;  
     int *lista1;
     int *inverso;
     int l,j,i;
     
     do{
         a=rand()% ncities;     
     }while(a>ncities-2);
     
     a1=a+1;   
     
     do{
          b=rand()% ncities;     
     }while( b>ncities-1 || b==a || b==a1);
     
     
     
    
     int aux;

     if(a>b){
          aux=a;
          a=b;
          b=a;          
     }
     
     a1=a+1;
     b1=b+1;

     int tam=b-a;
    lista1=(int*) malloc((tam)*sizeof(int));
    inverso=(int*) malloc((tam) *sizeof(int)); 
    
     l=0;
     for(i=0;i<ncities;i++){
        if(i>a && i<b1){
         lista1[l]=s[i];
         l++;    
        } 
     }
     
     //invertendo vetor;
    j = 0;
    for(i =tam-1; i >= 0; i--) {
         inverso[j] = lista1[i];
         ++j;
    } 
    
    l=0;
    for(i=0;i<ncities;i++){
          if(i>a && i<a+tam+1){
               s[i]=inverso[l];    
               l++;
           }
    }     
     
     
     free(lista1);
     free(inverso); 
     
     
}


