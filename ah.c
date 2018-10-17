#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ah.h"

node_t *node_new(node_t *node) {
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->order = 0;
    node->weight = 0;
    node->type = NYT;
    node->val = 0;
    return node;
}

void node_free(node_t *node) {
    while (node->left != NULL) 
        if (node->left->left != NULL) node = node->left; 
}

int AHEncoder(char *path) {
    FILE *fp;
    if (!(fp = fopen(path, "rb")))
        return printf("Not found file: %s\n", path), 1;

    node_t *array[MAX_INDEX];
    for (int i = 0; i < MAX_INDEX; i++) array[i] = NULL;

    node_t *root = malloc(sizeof(root));
    if (root == NULL) return 1;
    root = node_new(root);
    array[0] = root;

    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        node_t *node = malloc(sizeof(node));
        if (node == NULL) return 1;
        node = node_new(node);
    }

    return 0;
}
