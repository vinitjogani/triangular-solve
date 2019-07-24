// System libraries
#include <stdio.h>
// Project libraries
#include "includes.h"


int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s a_filename b_filename\n", argv[0]);
        return 1;
    }

    MTXReader A(argv[1]);
    MTXReader b(argv[2]);
    Utils::timedTest(A, b);
}