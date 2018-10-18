#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ah.h"

#define row 256 
#define col 256

int main(int argc, char **argv) {
    if (argv[1]) {
        return (AHEncoder(argv[1]));
    }
    else {
        perror("Not input file\n");
        return 1;
    }
    return 0;
}
