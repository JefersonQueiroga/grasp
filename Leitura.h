extern void fatal (char *s);
extern void **createMatrix (int i, int j, int s);
extern char *filename;
extern int  ncities;
extern char *instanceName;
extern int(*distance)(int, int);  /* pointer to function returning distance */
extern double *xx, *yy;
extern int  isEuclidean;
extern int  **distMat;
extern int  notIntegral;
extern void readInstance (char *s);
