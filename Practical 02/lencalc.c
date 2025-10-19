#include <stdio.h>
#include "mystrlen.h"

int main(void) {
    char s[MAXLEN];
    int len;

    while(1) {
        printf("Enter a string: ");
        fgets(s, MAXLEN, stdin);
        printf("You entered: %s", s);

        if (s[0] == '\n') {
            printf("Goodbye!\n");
            break;
        }

        // Calculate string length using mystrlen
        len = mystrlen(s);
        printf("%d\n", len);
    }

    return 0;
}