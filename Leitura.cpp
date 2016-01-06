#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Leitura.h"

char *filename;
int  ncities;
char *instanceName;

int  (*distance)(int, int);  /* pointer to function returning distance */
  
int  isEuclidean;      
int  notIntegral;      /* x,y coordinates include non-integral values */

double  *xx, *yy;      /* used only for Euclidean instances */
int  **distMat;        /* used only for matrix instances */

/*************************************************************************/

static FILE *fp;

#define BUFSIZE 512
typedef char mybuf[BUFSIZE];

static int linenum;
static mybuf buffer;         /* used for reading input lines */
static mybuf msg;            /* used for error messages */
static mybuf edgeWeightType;
static mybuf edgeWeightFormat;
static mybuf name;

/*************************************************************************/

void fatal (char *s) {
  fprintf (stderr, "%s\n", s);
  exit(1);
}

/*************************************************************************/

//cria a matriz com ponteiros e parte para dados linkando as posições simetricas
void **createMatrix (int i, int j, int s) {

  int k;
  char **result = (char **) (malloc(i*sizeof(result) + i*j*s));
  char *p = (char *)result + (i*sizeof(result));

  for (k=0; k<i; ++k) {
    result[k] = p;
    p += j*sizeof(s);
  }

  return (void **)result;
}

/*************************************************************************/

int arredonda(double rval){ //função para arredondamento
   if(rval < 0.0)
      return (int)(rval - 0.5);
   else
      return (int)(rval + 0.5);
}

/*************************************************************************/

static int roundDistance (int a, int b) {
  double xd = xx[a] - xx[b];
  double yd = yy[a] - yy[b];
  double r  = sqrt(xd*xd + yd*yd) + 0.5;
  return (int)r;
}

/*************************************************************************/

static int geoDistance (int a, int b) {
  
  if(a == b) return 0;
  
  int graus_a, graus_b;
  double RAIO_TERRA= 6378.388, PI = 3.141592, min_a, min_b, lat_a, lat_b, long_a, long_b, aux_1, aux_2, aux_3, r;
  
  graus_a = arredonda(xx[a]); //pega a parte inteira da coordenada geografica
  min_a = xx[a] - graus_a; //pega a parte francionaria da coordenada geografica
  lat_a = PI * (graus_a + 5.0 * min_a/3.0) / 180.0; //converte o valor para radianos
  graus_a = arredonda(yy[a]); //pega a parte inteira da coordenada geografica
  min_a = yy[a] - graus_a; //pega a parte francionaria da coordenada geografica
  long_a = PI * (graus_a + 5.0 * min_a/3.0) / 180.0; //converte o valor para radianos

  graus_b = arredonda(xx[b]); //pega a parte inteira da coordenada geografica
  min_b = xx[b] - graus_b; //pega a parte francionaria da coordenada geografica
  lat_b = PI * (graus_b + 5.0 * min_b/3.0) / 180.0; //converte o valor para radianos
  graus_b = arredonda(yy[b]); //pega a parte inteira da coordenada geografica
  min_b = yy[b] - graus_b; //pega a parte francionaria da coordenada geografica
  long_b = PI * (graus_b + 5.0 * min_b/3.0) / 180.0; //converte o valor para radianos
  
  aux_1 = cos(long_a - long_b);
  aux_2 = cos(lat_a - lat_b);
  aux_3 = cos(lat_a + lat_b);
  
  r = ( RAIO_TERRA * acos( 0.5 * ((1.0 + aux_1)*aux_2 - (1.0 - aux_1)*aux_3 )) + 1.0);

  return (int)r;
}

/*************************************************************************/

static int attDistance (int a, int b) { //calcula as distancias dos arquivos no formato att
  double xd = xx[a] - xx[b];
  double yd = yy[a] - yy[b];
  
  double r  = sqrt((xd*xd + yd*yd)/10.0);
  int t = arredonda(r);
  if (t < r) return (int)(r + 1);
  else return (int)r;
}

/*************************************************************************/

static int matDistance (int a, int b) {
  return distMat[a][b];
}

/*************************************************************************/

static int isWhiteSpace(char c) {
  return (c==' ') || (c=='\t');
}

/*************************************************************************/

static char *getline (void) {
  size_t length;
  size_t i;

  char *result = fgets (buffer, BUFSIZE, fp);
  if (result == 0) return result;

  length = strlen (result);
  if (length > 0 && result[length-1] == '\n') {
    result[--length] = 0;
  }
  else fatal ("Buffer size in getline() isn't large enough");
  
  for (i=0; i<length; ++i) 
    if (!isWhiteSpace(result[i])) return result;
  return getline();
}

/*************************************************************************/

static char *match (char *source, char *prefix) {

  size_t prefixLength = strlen (prefix);
  
  if (strncmp(source, prefix, prefixLength) != 0) return 0;
  source += prefixLength;
  while (isWhiteSpace(*source)) source++;
  if (*source == ':') source++;
  while (isWhiteSpace(*source)) source++;
  
  return source;
}

/*************************************************************************/

static int getInt (char *s) {

  int i;
  int result = sscanf (s, "%d", &i);
  
  if (result != 1) {
    sprintf (msg, "Error reading integer on line %d", linenum);
    fatal(msg);
  }
  return i;
}

/*************************************************************************/

static void readEdgeWeightSection(void) {

  int val;
  int i,j;

  if (match (edgeWeightType, "EXPLICIT")) {
    if (match (edgeWeightFormat, "UPPER_DIAG_ROW")) {
      
      distMat = (int **) createMatrix(ncities+1,ncities+1,sizeof(int));
      
      for (i=1; i<=ncities; ++i) 
	      for (j=i; j<=ncities; ++j) {
	          fscanf (fp, "%d", &val);
	          distMat[i][j] = distMat[j][i] = val;
          }

      isEuclidean = 0;
      distance = matDistance;
      return;
    }
    
    if (match (edgeWeightFormat, "UPPER_ROW")) {
      
      distMat = (int **) createMatrix(ncities+1,ncities+1,sizeof(int));
      
      for (i=1; i<=ncities; ++i){
          distMat[i][i]=0; 
	      for (j=i+1; j<=ncities; ++j) {
	          fscanf (fp, "%d", &val);
	          distMat[i][j] = distMat[j][i] = val;
          }
      }
      isEuclidean = 0;
      distance = matDistance;
      return;
    }
  }

  sprintf (msg, "Program doesn't support edge weights with type %s and format %s",
	   edgeWeightType, edgeWeightFormat);
  fatal(msg);
}

/*************************************************************************/

static void readDoubles(FILE *fp) {

  double d1, d2;
  int i,j;

  for (i=1; i<=ncities; ++i) {
    fscanf (fp, "%d %lf %lf", &j, &d1, &d2);
    if ((j<1) || (j >ncities))
      fatal ("City number is out of bounds");
    xx[j] = d1;
    yy[j] = d2;
    if (d1 != (int)d1) notIntegral=1;
    if (d2 != (int)d2) notIntegral=1;
  }
}
    
/*************************************************************************/

static void readNodeCoordSection(void) {
  
  int filePosition;

  if (match (edgeWeightType, "EUC_2D") || 
      match (edgeWeightType, "GEO") ||
      match (edgeWeightType, "ATT")) {
    xx = (double *) calloc (ncities+1, sizeof(double));
    yy = (double *) calloc (ncities+1, sizeof(double));

    notIntegral = 0;

    filePosition = ftell(fp);

    readDoubles(fp);

    isEuclidean = 1;
    if (match (edgeWeightType, "EUC_2D"))
       distance = roundDistance;
    else
      if (match (edgeWeightType, "GEO"))
         distance = geoDistance;
      else
         distance = attDistance;
    return;
  }
  else {
    
    sprintf (msg, "Program doesn't support node coordinates with type %s",
	     edgeWeightType);
    fatal(msg);
    return;
  }
}

/*************************************************************************/

void readDisplayDataSection(){
  double d1, d2;
  int i,j;

  for (i=1; i<=ncities; ++i) {   
    fscanf (fp, "%d %lf %lf", &j, &d1, &d2);
    if ((j<1) || (j >ncities))
      fatal ("City number is out of bounds");
  }   
}

/*************************************************************************/

void readInstance (char *s) {

  char *line;
  char *trailer;

  mybuf type;
  mybuf edgeDataFormat;
  mybuf nodeCoordType;
  mybuf displayDataType;

  instanceName = name;
  filename = s;
  fp = fopen (filename, "r");
  if (fp == NULL) fatal ("Couldn't open input file.\n");
  linenum = 0;

  ncities = 0;
  strcpy (instanceName, "NONE");
  strcpy (type, "NONE");
  strcpy (edgeWeightType, "NONE");
  strcpy (edgeDataFormat, "NONE");
  strcpy (nodeCoordType, "NONE");
  strcpy (displayDataType, "NONE");
  //faz a leitura do arquivo e armazena as informações do cabeçalho nas variaveis de nome equivalente
  //quando encontra a parte de dados chama a função de leitura correspondente ao formato
  while ((line = getline())) {
    linenum++;
    
    if ((trailer = match (line, "NAME")))
      strcpy (instanceName, trailer);
    else if ((trailer = match (line, "TYPE")))
      strcpy (type, trailer);
    else if ((trailer = match (line, "COMMENT"))) {}
    else if ((trailer = match (line, "DIMENSION")))
      ncities = getInt (trailer);
    else if ((trailer = match (line, "EDGE_WEIGHT_TYPE")))
      strcpy (edgeWeightType, trailer);
    else if ((trailer = match (line, "EDGE_WEIGHT_FORMAT")))
      strcpy (edgeWeightFormat, trailer);
    else if ((trailer = match (line, "EDGE_DATA_FORMAT")))
      strcpy (edgeDataFormat, trailer);
    else if ((trailer = match (line, "NODE_COORD_TYPE")))
      strcpy (nodeCoordType, trailer);
    else if ((trailer = match (line, "DISPLAY_DATA_TYPE")))
      strcpy (displayDataType, trailer);
    else if ((trailer = match (line, "EOF"))) break;
    else if ((trailer = match (line, "EDGE_WEIGHT_SECTION")))
      readEdgeWeightSection();
    else if ((trailer = match (line, "NODE_COORD_SECTION")))
      readNodeCoordSection();
    else if ((trailer = match (line, "DISPLAY_DATA_SECTION")))
      readDisplayDataSection();
    else {
      sprintf (msg, "Unexpected input line: %s", line);
      fatal(msg);
    }
  }
  if (match (type, "TSP") == 0) {
    sprintf (msg, "This message has type %s, not TSP", type);
    fatal (msg);
  }

  if (distance == 0)
    fatal ("No matrix or coordinate information in input file");
}
