#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ah.h"

void AHNodeDump(node_t *root) {
    node_t *node = root;
    if (node->left != NULL) {
        printf("order: %d %x\n ", node->order, node->sym);
        AHNodeDump(node->left);
    } else {
        printf("order: %d %x\n ", node->order, node->sym);
    }
    if (node->right != NULL) {
        AHNodeDump(node->right);
    }
}

node_t *AHNodeNew(node_t *node) {
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->sym = NYTSYM;
    node->order = 0;
    node->type = NYT;
    node->freq = 0;
    return node;
}

int AHFirstFetch(int ch, node_t **ah_array) {
    for (int i = 0; i < MAX_INDEX; i++) {
        if (ah_array[i] != NULL && ch == ah_array[i]->sym) return 0;
    }
    return 1;
}

int AHNodeAdd(node_t *parent, int ch) {
    node_t *left = malloc(sizeof(*left));
    if (left == NULL) return perror("Malloc:"), 1;
    node_t *right = malloc(sizeof(*right));
    if (right == NULL) return perror("Malloc:"), free(left), 1;

    left = AHNodeNew(left);
    left->parent = parent;
    left->sym = NYTSYM;
    left->order = parent->order - 2;
    left->type = NYT;
    left->freq = 0;
    right = AHNodeNew(right);
    right->parent = parent;
    right->sym = ch;
    right->order = parent->order - 1;
    right->type = NYT;
    right->freq = 1;
    parent->left = left;
    parent->right = right;
    parent->sym = INNERSYM;
    parent->type = INNER;
    parent->freq = 1;

    return 0;
}


int AHEncoder(char *path) {
    FILE *fp;
    int ret;
    ret = 0;
    if (!(fp = fopen(path, "rb"))) {
        fprintf(stderr, "No such file: %s\n", path);
        ret = 1;
        goto LEAVE;
    }

    node_t *ah_array[MAX_INDEX];  // Adative Huffman Tree's index
    for (int i = 0; i < MAX_INDEX; i++) ah_array[i] = NULL;
    
    printf("%p\n", ah_array[0]);

    node_t *root = malloc(sizeof(*root));
    if (root == NULL) {
        ret = 1;
        perror("Malloc:");
        goto CLOSE;
    }
    root = AHNodeNew(root);
    root->type = INNER;
    root->order = MAX_INDEX;
    ah_array[MAX_INDEX - 1] = root;

    node_t *nyt = malloc(sizeof(*nyt));
    if (nyt == NULL) {
        ret = 1;
        perror("Malloc:");
        goto FREE;
    }
    nyt = root;

    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (AHFirstFetch(ch, ah_array)) {
            if (AHNodeAdd(nyt, ch)) {
                ret = 1;
                goto FREE;
            }
            ah_array[nyt->order - 2] = nyt->left;
            ah_array[nyt->order - 1] = nyt->right;
            nyt = nyt->left;
        }
    }
    AHNodeDump(root);

FREE:
    AHNodeFree(ah_array);
CLOSE:
    fclose(fp);
LEAVE:
    return ret;
}

void AHNodeFree(node_t **ah_array) {
    for (int i = 0; i < MAX_INDEX; i++)
        if (ah_array[i] != NULL)
            free(ah_array[i]);
}
