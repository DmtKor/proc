#include <stdio.h>

/* 
 * Simple program to be launched by exec()
 */

int main(int argc, char *argv[]) {
    puts("Arguments...");
    for (int i = 0; i < argc; i++) {
        printf("%d: %s\n", i+1, argv[i]);
    }
    puts("Child process output...");
    return 0;
}