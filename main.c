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
        FILE *InputFile;
        FILE *OutputFile;
        if (!(InputFile = fopen(argv[1], "rb"))) {
            fprintf(stderr, "No such file: %s\n", argv[1]);
            ret = 1;
            goto EXIT;
        }
        if (!(OutputFile = fopen("out.ah", "wb+"))) {
            fprintf(stderr, "Perssion denied.\nCannot open file: %s\n ", "out.ah");
            fclose(InputFile);
            ret = 1;
            goto EXIT;
        }
        if (AHEncoder(InputFile, OutputFile)) {
            fclose(InputFile);
            fclose(OutputFile);
            ret = 1;
            goto EXIT;
        }
    }
    else {
        perror("Not input file\n");
        ret = 1;
        goto EXIT;
    }
EXIT:
    return ret;
}
