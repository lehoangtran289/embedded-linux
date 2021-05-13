#include <stdio.h>
#include <stdlib.h>

char* func(int val) {
    char *string = (char *) malloc (sizeof(char) * 512);
    sprintf(string, "string = %d", val);
    return string;
}

int func_prac(int a, int b) {
    printf("%s, %s\n", func(a), func(b));
    return 0;
}

int main() {
    printf("%s\n", func(1));
    func_prac(10, 15);
}
