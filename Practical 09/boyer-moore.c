#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TEXT_LENGTH 1024
#define TEXT_FORMAT_STRING "%[^\n]%*c"

int *lastOccurrence(char *pattern, char *alphabet)
{
    int *L = malloc(256 * sizeof(int));
    if (!L)
        exit(1);
    for (int i = 0; i < 256; i++)
        L[i] = -1;
    int m = strlen(pattern);
    for (int j = 0; j < m; j++)
    {
        unsigned char c = pattern[j];
        L[c] = j;
    }
    return L;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int BoyerMooreMatch(char *T, char *P, int *L)
{
    int n = strlen(T);
    int m = strlen(P);
    int i = m - 1;
    int j = m - 1;
    while (i < n)
    {
        if (T[i] == P[j])
        {
            if (j == 0)
                return i;
            i--;
            j--;
        }
        else
        {
            int lastOccur = L[(unsigned char)T[i]];
            int shift = m - min(j, 1 + lastOccur);
            i += shift;
            j = m - 1;
        }
    }
    return -1;
}

int main()
{
    char alphabet[128];
    char text[MAX_TEXT_LENGTH];
    char pattern[128];

    printf("Enter alphabet: ");
    scanf(TEXT_FORMAT_STRING, alphabet);
    printf("Enter text: ");
    scanf(TEXT_FORMAT_STRING, text);
    printf("Enter pattern: ");
    scanf(TEXT_FORMAT_STRING, pattern);

    int *L = lastOccurrence(pattern, alphabet);

    bool present[256] = {false};
    int s = strlen(alphabet);
    for (int i = 0; i < s; i++)
    {
        present[(unsigned char)alphabet[i]] = true;
    }

    for (int i = 0; i < 256; i++)
    {
        if (present[i])
        {
            printf("\nL[%c] = %d", (char)i, L[i]);
        }
    }

    int index = BoyerMooreMatch(text, pattern, L);
    if (index != -1)
    {
        printf("\n\nMatch found at position %d.\n", index);
    }
    else
    {
        printf("\n\nNo match.\n");
    }

    free(L);
    return 0;
}