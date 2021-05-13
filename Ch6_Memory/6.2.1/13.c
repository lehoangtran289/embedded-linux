#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void func_if(char string[]) {
    char buff[5];
    if (strlen(string) > 4) {
        for (int i = 0; i < 4; i++) {
            buff[i] = string[i];
        }
        printf("%s\n", buff);
        return;
    }
    strcpy(buff, string);
    printf("%s\n", buff);
}

int main() {
    char string[] = "abcdef";
    func_if(string);

    strcpy(string, "abc");
    func_if(string);

    return 0;
}
