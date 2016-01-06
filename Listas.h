
void insere_ordenado_lista(struct lista *i,struct lista **in,struct lista **f);
void insere_final_lista(struct lista *i, struct lista **in, struct lista **f);
void mostra_lista(struct lista **in,struct lista **f);  
int tamanho_lista(struct lista **in,struct lista **f); 
struct lista *encontra_registro_lista(int posicao,struct lista **in);
struct lista *encontra_registro(int campo1, int campo2, struct lista **in);
void apaga_registro(struct lista *registro, struct lista **inicio, struct lista **fi);
void apaga_lista(struct lista **in, struct lista **fi);           
int *retorne_vetor(int tamanho, struct lista **inicio);




