#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ah.h"

typedef uint8_t Byte;

void AHNodeDump(node_t *root, int count) {
    node_t *node = root;
    printf("%*s", count, "");
    printf("sym: %d ", node->sym);
    printf("order: %d ", node->order);
    printf("freq: %d\n", node->freq);
    if (node->left != NULL) {
        AHNodeDump(node->left, count + 1);
    }
    if (node->right != NULL) {
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

int AHFirstFetch(int ch, node_t **ah_array) {
    for (int i = 0; i < MAX_INDEX; i++) {
        if (ah_array[i] != NULL && ch == ah_array[i]->sym) return 0;
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
    right = AHNodeNew(right);
    right->parent = parent;
    right->sym = ch;
    right->order = parent->order - 1;
    right->type = LEAF;
    right->freq = 1;
    parent->left = left;
    parent->right = right;
    parent->sym = INNERSYM;
    parent->type = INNER;
    parent->freq = 1;
    return 0;
}

void AHArrayDump(node_t **ah_array) {
    for (int i = 0; i <= MAX_INDEX; i++) {
        if (ah_array[i] != NULL) {
            printf("%5d", ah_array[i]->order);
        }
    }
    printf("\n");
    for (int i = 0; i <= MAX_INDEX; i++) {
        if (ah_array[i] != NULL) {
            printf("%5d", ah_array[i]->sym);
        }
    }
    printf("\n");
    for (int i = 0; i <= MAX_INDEX; i++) {
        if (ah_array[i] != NULL) {
            printf("%5d", ah_array[i]->freq);
        }
    }
    printf("\n");
}

void AHTreeUpdate(node_t **ah_array, node_t *cur_node) {
    int head; //block head index
    int tail; //block tail index
    node_t *tmp;
    node_t *tmpparent;
    if (cur_node->order == MAX_INDEX) return;
    if (cur_node->parent->left->type == NYT) {  
        cur_node->freq++;
        cur_node->parent->freq++;
        return;
    }
    if (cur_node->type == INNER) {
        for (int i = 0; i <= MAX_INDEX; i++) {
            if (ah_array[i] != NULL &&
                    ah_array[i]->freq == cur_node->freq &&
                    ah_array[i]->type == INNER) {
                head = ah_array[i]->order;
                break;
            }
        }
        for (int i = MAX_INDEX; i >= 0; i--) {
            if (ah_array[i] != NULL && 
                    ah_array[i]->freq == cur_node->freq &&
                    ah_array[i]->type == LEAF) {
                tail = ah_array[i]->order;
                break;
            }
        }
        tmp = ah_array[head];
        tmpparent = ah_array[head]->parent;

        for (int i = head; i < tail; i++) {
            ah_array[i] = ah_array[i + 1];
        }
        ah_array[tail] = tmp;
        for (int i = tail; i > head; i--) {
            ah_array[i]->parent = ah_array[i - 1]->parent;
            if (ah_array[i - 1]->order%2) {
                ah_array[i]->parent->right = ah_array[i];
            } else {
                ah_array[i]->parent->left = ah_array[i];
            }
        }
        ah_array[head]->parent = tmpparent;
        if (tmp->order%2) {
            tmpparent->right = ah_array[head];
        } else {
            tmpparent->left = ah_array[head];
        }
    } else {
        head = cur_node->order;
        for (int i = MAX_INDEX; i >= 0; i--) {
            if (ah_array[i] != NULL && 
                    ah_array[i]->freq == cur_node->freq) {
                tail = ah_array[i]->order;
                break;
            }
        }
        tmp = ah_array[head];
        tmpparent = ah_array[head]->parent;

        for (int i = head; i < tail; i++) {
            ah_array[i] = ah_array[i + 1];
        }
        ah_array[tail] = tmp;
        for (int i = tail; i > head; i--) {
            ah_array[i]->parent = ah_array[i - 1]->parent;
            if (ah_array[i - 1]->order%2) {
                ah_array[i]->parent->right = ah_array[i];
            } else {
                ah_array[i]->parent->left = ah_array[i];
            }
        }
        ah_array[head]->parent = tmpparent;
        if (tmp->order%2) {
            tmpparent->right = ah_array[head];
        } else {
            tmpparent->left = ah_array[head];
        }
        cur_node->freq++;
    }
    for (int i = 0; i <= MAX_INDEX; i++) {
        if (ah_array[i] != NULL) {
            ah_array[i]->order = i;
            if (ah_array[i]->type == INNER) {
                ah_array[i]->freq = ah_array[i]->left->freq +\
                                    ah_array[i]->right->freq;
            }
        }
    }
}

Byte AHTreeGetSymCode(node_t **ah_array, int ch) {
    node_t *sym;
    for (int i = 0; i <= MAX_INDEX; i++) {
        if (ah_array[i] != NULL && ah_array[i]->sym == ch) {
            sym = ah_array[i];
        }
    }
    for (node_t *node = sym->parent; node->parent != NULL; node = node->parent) {

    }
    return 0; 
}
void AHOutputBuffer(FILE *OutputFile, Byte ch, int size) {
    static Byte encode = 0;
    static int pos = 7;
    for (int offset = size - 1; offset >= 0; offset--) {
        encode += (ch >> offset) & 1;
        pos--;
        if (pos == -1) {
            AHOutputFile(OutputFile, encode);
            pos = 7;
        }
        encode <<= 1;
    }
}

void AHOutputFile(FILE *OutputFile, Byte OutputBuf) {
    fputc(OutputBuf, OutputFile);
}


int AHEncoder(FILE *InputFile, FILE *OutputFile) {
    int ret;
    ret = 0;

    node_t *ah_array[MAX_INDEX + 1];  // Adative Huffman Tree's index
    for (int i = 0; i <= MAX_INDEX; i++) {
        ah_array[i] = NULL;
    }

    node_t *root = AHNodeNew(NULL);
    if (root == NULL) {
        ret = 1;
        goto LEAVE;
    }
    root->type = INNER;
    root->order = MAX_INDEX;
    ah_array[MAX_INDEX] = root;

    node_t *nyt;
    node_t *leaf;
    nyt = root;

    int ch;
    while ((ch = fgetc(InputFile)) != EOF) {
        if (AHFirstFetch(ch, ah_array)) {
            if (AHNodeAdd(nyt, ch)) {
                ret = 1;
                goto FREE;
            }
            node_t *child = nyt;
            node_t *sym = nyt;
            int size = 0;
            Byte encode = 0;
            while (sym->parent != NULL) {
                sym = sym->parent;
                if (sym->right == child) {
                    encode += (1 << size);
                } 
                child = sym;
                size += 1;
            }
            ah_array[nyt->order - 2] = nyt->left;
            ah_array[nyt->order - 1] = nyt->right;
            nyt = nyt->left;
            AHTreeUpdate(ah_array, nyt->parent);
            AHOutputBuffer(OutputFile, encode, size); 
            AHOutputBuffer(OutputFile, ch, BYTE_SIZE);
        } else {
            for (int i = 0; i <= MAX_INDEX; i++) {
                if (ah_array[i] != NULL && ah_array[i]->sym == ch) {
                    leaf = ah_array[i];
                }
            }
            AHTreeUpdate(ah_array, leaf);
            node_t *child = leaf;
            node_t *sym = leaf;
            int size = 0;
            Byte encode = 0;
            while (sym->parent != NULL) {
                sym = sym->parent;
                if (sym->right == child) {
                    encode += (1 << size);
                } 
                child = sym;
                size += 1;
            }
            AHOutputBuffer(OutputFile, encode, size); 
        }
    }
    AHNodeDump(root, 0);

FREE:
    AHNodeFree(ah_array);
LEAVE:
    return ret;
}

void AHNodeFree(node_t **ah_array) {
    for (int i = 0; i < MAX_INDEX; i++)
        if (ah_array[i] != NULL)
            free(ah_array[i]);
}
