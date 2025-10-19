#include <stdio.h>
#include <string.h>  // For strlen
#include "mystrlen.h"

int main(void) {
    char s[MAXLEN];
    int len;
    
    while(1) {
        printf("Enter a string: ");
        if (fgets(s, MAXLEN, stdin) == NULL) {
            printf("Error reading input.\n");
            break;
        }

        // Remove newline character added by fgets
        size_t input_len = strlen(s);
        if (input_len > 0 && s[input_len - 1] == '\n') {
            s[input_len - 1] = '\0';
        }

        // Check for empty string
        if (s[0] == '\0') {
            printf("Goodbye!\n");
            break;
        }

        // Calculate string length using mystrlen
        len = mystrlen(s);
        printf("%d\n", len);
    }   
    return 0;
}


