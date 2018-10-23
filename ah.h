#include <stdint.h>

#define CODE_SIZE 32

#define MAX_INDEX 513

#define NYT   0
#define INNER 1
#define LEAF  2

#define NYTSYM -1;
#define INNERSYM -2;


typedef struct node {
	struct node * parent;
    struct node * left;
    struct node * right;
    int sym;
    int order;
    int type;
    int freq;
} node_t;


int AHFirstFetch(int , node_t **);
int AHEncoder(FILE *, FILE *);
int AHNodeAdd(node_t *, int);
void AHNodeFree(node_t **);
void AHNodeDump(node_t *, int);
void AHVitter(node_t **, node_t *);
void AHOutputBuffer(FILE *, node_t *, int *);
void AHOutputFile(FILE *, uint8_t);
void AHNodeMove(node_t *, node_t *);
void AHTreeUpdateDepthAndCode(node_t *);
node_t *AHNodeNew(node_t *);
