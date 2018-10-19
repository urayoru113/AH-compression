#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ah.h"

#define row 256 
#define col 256

int main(int argc, char **argv) {
    int ret;
    ret = 0;
    if (argv[1]) {
        FILE *fp;
        if (!(fp = fopen(argv[1], "rb"))) {
            fprintf(stderr, "No such file: %s\n", argv[1]);
            ret = 1;
            goto EXIT;
        }
        return (AHEncoder(fp));
    }
    else {
        perror("Not input file\n");
        ret = 1;
    }
EXIT:
    return ret;
}
