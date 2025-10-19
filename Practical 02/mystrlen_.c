#include <stdio.h>
#include "mystrlen.h"

int mystrlen(char *s) {
   int length = 0;
   while (length < MAXLEN && s[length] != '\0') {
       length++;
    }
    return length;
}

int main(void) {
    char test1[] = "YU-Gi-OH";
    char test2[MAXLEN + 1];  // Test string without a null terminator within MAXLEN

    // Initialize test2 with 'A's and no '\0' in the first MAXLEN positions.
    for (int i = 0; i < MAXLEN; i++) {
        test2[i] = 'A';
    }
    test2[MAXLEN] = '\0';  // Ensure it is null-terminated eventually

    printf("Length of \"%s\" is %d\n", test1, mystrlen(test1));
    printf("Length of test2 (without early null) is %d\n", mystrlen(test2));

    return 0;
}