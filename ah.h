#include <stdint.h>

#define BYTE_SIZE 8

#define MAX_INDEX 256

#define NYT   0
#define INNER 1
#define LEAF  2

#define NYTSYM -1;
#define INNERSYM -2;

typedef uint8_t Byte;

typedef struct node {
	struct node * parent;
    struct node * left;
    struct node * right;
    int sym;
    int order;
    int type;
    int depth;
    Byte freq;
    Byte code;
    
} node_t;


int AHFirstFetch(int , node_t **);
int AHEncoder(FILE *, FILE *);
int AHNodeAdd(node_t *, int);
void AHNodeFree(node_t **);
void AHNodeDump(node_t *, int);
void AHTreeUpdate(node_t **ah_array, node_t *cur_node);
void AHOutputBuffer(FILE*, uint8_t, int);
void AHOutputFile(FILE *, uint8_t);
node_t *AHNodeNew(node_t *);
