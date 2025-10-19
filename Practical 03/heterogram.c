#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool isHeterogram(char A[]) {
    bool word[26] = {false};
    for (int i = 0; A[i] != '\0'; i++) {
        if (A[i] == ' ') {
            continue;
        }
        int index = A[i] - 'a';
        if (word[index]) {
            return false;
        }
        word[index] = true;
    }
    return true;
}

int main() {
    char input[64];
    printf("Enter a word or phrase: ");
    scanf("%[^\n]%*c", input);
    /* Print the Output*/
    if (isHeterogram(input)) {
        printf("\"%s\" is a heterogram\n", input);
    } else {
        printf("\"%s\" is not a heterogram\n", input);
    }
    return 0;
}
