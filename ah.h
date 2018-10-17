#include <stdint.h>

#define MAX_INDEX 256

#define NYT  0
#define NODE 1
#define LEAF 2


typedef struct node {
    struct node * parent;
    struct node * left;
    struct node * right;
    int order;
    int weight;
    int type;
    uint8_t val; //symbol
} node_t;

int AHEncoder(char *);
node_t *node_new(node_t *);
