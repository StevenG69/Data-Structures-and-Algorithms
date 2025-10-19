#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
} NodeT;

void freeLL(NodeT *list) {
    NodeT *p, *temp;
    p = list;
    while (p != NULL) {
        temp = p->next;
        free(p);
        p = temp;
    }
}

void showLL(NodeT *list) {
    NodeT *p;
    for (p = list; p != NULL; p = p->next) {
        printf("%d", p->data);
        if (p->next != NULL) {
            printf("-->");
        }
    }
}

NodeT *joinLL(NodeT *list, int v) {
    static NodeT *tail = NULL;
    NodeT *newNode = malloc(sizeof(NodeT));
    assert(newNode != NULL);
    newNode->data = v;
    newNode->next = NULL;

    if (list == NULL) {
        tail = newNode;
        return newNode;
    }
    tail->next = newNode;
    tail = newNode;
    return list;
}

int main(void) {
    NodeT *LL = NULL;
    int num;

    while (1) {
        printf("Enter an integer: ");
        int result = scanf("%d", &num);
        if (result != 1) {
            break;
        }
        LL = joinLL(LL, num);
    }

    printf("Done.");
    if (LL != NULL) {
        printf(" List is ");
        showLL(LL);
    }
    printf("\n");
    freeLL(LL);
    return 0;
}