//#include <stdio.h>
#include "mystrlen.h"

int mystrlen(char *s) {
   int length = 0;
   while (length < MAXLEN && s[length] != '\0') {
       length++;
    }
    return length;
}

int main(void) {
    //char test1[] = "YU-Gi-OH";
    //printf("Length of \"%s\" is %d\n", test1, mystrlen(test1));
    //return 0;
}