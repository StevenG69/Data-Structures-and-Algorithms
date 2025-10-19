#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int length = atoi(argv[1]);
    int seed = atoi(argv[2]);

    srand(seed);

    char word[length + 1];
    for (int i = 0; i < length; i++)
    {
        word[i] = 'a' + (rand() % 26);
    }
    word[length] = '\0';

    printf("%s\n", word);

    return 0;
}