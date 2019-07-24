// System libraries
#include <stdio.h>
// Project libraries
#include "includes.h"


int main(int argc, char **argv) {
    // Verify arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s a_filename b_filename\n", argv[0]);
        return 1;
    }

    // Create loaders for both files
    MTXReader A(argv[1]);
    MTXReader b(argv[2]);
    // Trigger a timed test of the triangular solve
    Utils::timedTest(A, b);
}