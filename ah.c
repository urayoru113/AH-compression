#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ah.h"


void AHNodeDump(node_t *root, int count) {
    node_t *node = root;
    printf("%*s", count, "");
    printf("sym: %d ", node->sym);
    printf("order: %d ", node->order);
    printf("freq: %d ", node->freq);
    printf("\n");
    if (node->left != NULL) {
        /*
        if (node->left->depth < count) {
            printf("loop\n");
            printf("node %d\n", node->left->order);
            return;
        }
        */
        AHNodeDump(node->left, count + 1);
    }
    if (node->right != NULL) {
        /*
        if (node->right->depth < count) {
            printf("loop\n");
            printf("node %d\n", node->right->order);
            return;
        }
        */
        AHNodeDump(node->right, count + 1);
    }
}

node_t *AHNodeNew(node_t *parent) {
    node_t *node = malloc(sizeof(*node));
    if (node == NULL) return perror("Malloc:"), NULL;
    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    node->sym = NYTSYM;
    node->order = 0;
    node->type = NYT;
    node->freq = 0;
    return node;
}

int AHFirstFetch(int ch, node_t **tree_array) {
    for (int i = 0; i < MAX_INDEX; i++) {
        if (tree_array[i] != NULL && ch == tree_array[i]->sym) return 0;
    }
    return 1;
}

int AHNodeAdd(node_t *parent, int ch) {
    node_t *left = AHNodeNew(parent);
    if (left == NULL) return perror("Malloc:"), 1;
    node_t *right = AHNodeNew(parent);
    if (right == NULL) return perror("Malloc:"), free(left), 1;
    left->parent = parent;
    left->sym = NYTSYM;
    left->order = parent->order - 2;
    left->type = NYT;
    left->freq = 0;
    right->parent = parent;
    right->sym = ch;
    right->order = parent->order - 1;
    right->type = LEAF;
    right->freq = 1;
    parent->left = left;
    parent->right = right;
    parent->sym = INNERSYM;
    parent->type = INNER;
    parent->freq = 0;
    return 0;
}

void AHArrayDump(node_t **tree_array) {
    printf("order: ");
    for (int i = 0; i <= MAX_INDEX; i++) {
        if (tree_array[i] != NULL) {
            printf("%6d ", tree_array[i]->order);
        }
    }
    printf("\n");
    printf("sym:   ");
    for (int i = 0; i <= MAX_INDEX; i++) {
        if (tree_array[i] != NULL) {
            printf("%6d ", tree_array[i]->sym);
        }
    }
    printf("\n");
    printf("freq:  ");
    for (int i = 0; i <= MAX_INDEX; i++) {
        if (tree_array[i] != NULL) {
            printf("%6d ", tree_array[i]->freq);
        }
    }
    printf("\n");
}

void AHNodeMove(node_t *source, node_t *target) {
    if (target->order & 1) {
        target->parent->right = source;
    } else {
        target->parent->left = source;
    }
    source->parent = target->parent;
    source->order = target->order;
}


/*
void AHTreeUpdateDepthAndCode(node_t *node) {
    if (node->left != NULL) {
        node->left->depth = node->depth + 1;
        -node>right->depth = node->depth + 1;
        node->right->code = (node->code << 1) + 1;
        AHTreeUpdateDepthAndCode(node->right);
    }
}
*/

void AHVitter(node_t **tree_array, node_t *cur_node) {
    int head = 0; //block head index
    int tail = 0; //block tail index
    node_t *aux;
    node_t *aux_parent;
    int aux_order;
    while (cur_node->parent != NULL) {
        head = cur_node->order;
        tail = 0;
        for (int i = MAX_INDEX - 1; i > cur_node->order; i--) {
            if (tree_array[i] != NULL) {
                if ((cur_node->type == INNER                   &&
                    tree_array[i]->freq == cur_node->freq + 1  &&
                    tree_array[i]->type == LEAF)               ||
                    (cur_node->type == LEAF                    &&
                    tree_array[i]->freq == cur_node->freq)) {
                    tail = tree_array[i]->order;
                    break;
                }
            }
        }
        aux = tree_array[head];
        aux_parent = tree_array[head]->parent;
        aux_order = tree_array[head]->order;
        if (cur_node->type == INNER &&
            tail > head) {
            cur_node->freq++;
            for (int i = head; i < tail; i++) {
                tree_array[i] = tree_array[i + 1];
            }
            tree_array[tail] = aux;
            for (int i = tail; i > head; i--) {
                AHNodeMove(tree_array[i], tree_array[i - 1]);
            }
            if (aux_order & 1) {
                aux_parent->right = tree_array[head];
            } else {
                aux_parent->left = tree_array[head];
            }
            tree_array[head]->parent = aux_parent;
            tree_array[head]->order = aux_order;
            cur_node = aux_parent;
        }  
        else if (cur_node->type == LEAF &&
                 tail > head &&
                 ((tail != head + 1) |
                 (tree_array[head]->parent != tree_array[tail]))) {
            for (int i = head; i < tail; i++) {
                tree_array[i] = tree_array[i + 1];
            }
            tree_array[tail] = aux;
            for (int i = tail; i > head; i--) {
                AHNodeMove(tree_array[i], tree_array[i - 1]);
            }
            tree_array[head]->parent = aux_parent;
            tree_array[head]->order = aux_order;
            if (aux_order & 1) {
                aux_parent->right = tree_array[head];
            } else {
                aux_parent->left = tree_array[head];
            }
            cur_node->freq++;
            cur_node = cur_node->parent;
        }
        else {
            cur_node->freq++;
            cur_node = cur_node->parent;
        }
    }
    cur_node->freq++;
}

void AHOutputBuffer(FILE *OutputFile, node_t *node, int *offset) {
    int len = 0;
    uint8_t OutputBuffer[CODE_SIZE];
    static uint8_t encode = 0;
    node_t *tmp = node;
    if (node == NULL && *offset != 0) {
        AHOutputFile(OutputFile, encode << (8 - *offset));
        return;
    }
    while (tmp->parent != NULL) {
        if (tmp->parent->left == tmp) {
            OutputBuffer[len] = 0;
        } else {
            OutputBuffer[len] = 1;
        }
        len++;
        tmp = tmp->parent;
    }
    for (int i = len - 1; i >= 0; i--) {
        encode = (encode << 1) + OutputBuffer[i];
        (*offset)++;
        if (*offset == 8) {
            AHOutputFile(OutputFile, encode);
            *offset = 0;
        }
    }
    if (node->type == INNER) {
        for (int i = 7; i >= 0; i--) {
            encode = (encode << 1) + ((node->right->sym >> i) & 1);
            (*offset)++;
            if (*offset == 8) {
                AHOutputFile(OutputFile, encode);
                *offset = 0;
            }
        }
    }
}

void AHOutputFile(FILE *OutputFile, uint8_t encode) {
    fputc(encode, OutputFile);
    //printf("OutputBuffer content %d\n", encode);
}


int AHEncoder(FILE *InputFile, FILE *OutputFile) {
    int ret;
    ret = 0;
    node_t *tree_array[MAX_INDEX + 1];  // Adative Huffman Tree's index
    for (int i = 0; i <= MAX_INDEX; i++) {
        tree_array[i] = NULL;
    }

    node_t *root = AHNodeNew(NULL);
    if (root == NULL) {
        ret = 1;
        goto LEAVE;
    }
    root->type = NYT;
    root->order = MAX_INDEX;
    tree_array[MAX_INDEX] = root;

    node_t *nyt = NULL;
    node_t *leaf = NULL;
    node_t *node = NULL;
    nyt = root;
    int ch;
    int offset = 0;
    while ((ch = fgetc(InputFile)) != EOF) {
        if (AHFirstFetch(ch, tree_array)) {
            if (AHNodeAdd(nyt, ch)) {
                ret = 1;
                goto FREE;
            }
            node = nyt;
            nyt = node->left;
            AHOutputBuffer(OutputFile, node, &offset);//put nyt and symbol
            tree_array[node->left->order] = node->left;
            tree_array[node->right->order] = node->right;
            AHVitter(tree_array, node);
        } else {
            for (int i = MAX_INDEX; tree_array[i] != NULL; i--) {
                if (tree_array[i]->sym == ch) {
                    leaf = tree_array[i];
                }
            }
            AHOutputBuffer(OutputFile, leaf, &offset); 
            AHVitter(tree_array, leaf);
        }
    }
    /*
     * Padding last OutputBuf to zero
     * example: If input ab output will be 01100001 00110001 1
     * and padding its to 01100001 00110001 10000000
     */
    //AHNodeDump(tree_array[MAX_INDEX], 0);
    //AHArrayDump(tree_array);
    AHOutputBuffer(OutputFile, NULL, &offset); 
    AHOutputFile(OutputFile, (uint8_t)(8 - offset));

FREE:
    AHNodeFree(tree_array);
LEAVE:
    return ret;
}

void AHNodeFree(node_t **tree_array) {
    for (int i = 0; i < MAX_INDEX; i++)
        if (tree_array[i] != NULL)
            free(tree_array[i]);
}
