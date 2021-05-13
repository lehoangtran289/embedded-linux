#include <stdio.h>
#include <stdlib.h>

int func(int flag) {
    int array_buff[10000];
    if (flag) {
        return -1;
    }

    // process
    printf("here\n");

    return 0;
}

int main() {
    func(0);
}
