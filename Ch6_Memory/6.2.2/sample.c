#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void) {
    int* ptr;

    ptr = (int*)malloc(100 * sizeof(int));
    memset(ptr, 0, (100 * sizeof(int)));
    printf("malloc: [%x]\n", ptr);
    free(ptr);
    ptr = NULL;

    ptr = (int*) calloc (200, sizeof(int));
    printf("calloc: [%x]\n", ptr);
    free(ptr);
    ptr = NULL;

    return;
}