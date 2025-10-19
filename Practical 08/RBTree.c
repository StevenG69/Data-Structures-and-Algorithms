// Red-Black Tree ADT implementation ... COMP9024 25T1

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "RBTree.h"

#define PRINT_COLOUR_RED "\x1B[31m"
#define PRINT_COLOUR_RESET "\x1B[0m"

#define data(tree) ((tree)->data)
#define left(tree) ((tree)->left)
#define right(tree) ((tree)->right)
#define colour(tree) ((tree)->colour)
#define isRed(tree) ((tree) != NULL && (tree)->colour == RED)

typedef enum
{
    RED,
    BLACK
} Colr;

typedef struct Node
{
    int data;
    Colr colour;
    Tree left, right;
} Node;

Tree insertRB(Tree tree, Item item, bool inRight);

// create a new empty Tree
Tree newTree()
{
    return NULL;
}

// make a new node containing data
Tree newNode(Item it)
{
    Tree new = malloc(sizeof(Node));
    assert(new != NULL);
    data(new) = it;
    colour(new) = RED;
    left(new) = right(new) = NULL;
    return new;
}

Tree rotateRight(Tree);
Tree rotateLeft(Tree);

Tree TreeInsert(Tree t, Item it)
{
    t = insertRB(t, it, false); // Initial call (not right child)
    if (t != NULL)
    {
        colour(t) = BLACK; // Ensure root is black
    }
    return t;
}

// check whether a key is in a Tree
bool TreeSearch(Tree t, Item it)
{
    if (t == NULL)
        return false;
    else if (it < data(t))
        return TreeSearch(left(t), it);
    else if (it > data(t))
        return TreeSearch(right(t), it);
    else // it == data(t)
        return true;
}

Tree rotateRight(Tree n1)
{
    if (n1 == NULL || left(n1) == NULL)
        return n1;
    Tree n2 = left(n1);
    left(n1) = right(n2);
    right(n2) = n1;
    return n2;
}

Tree rotateLeft(Tree n2)
{
    if (n2 == NULL || right(n2) == NULL)
        return n2;
    Tree n1 = right(n2);
    right(n2) = left(n1);
    left(n1) = n2;
    return n1;
}

Tree insertRB(Tree tree, Item item, bool inRight)
{
    if (tree == NULL)
    {
        return newNode(item);
    }
    if (item == data(tree))
    {
        return tree;
    }

    if (isRed(left(tree)) && isRed(right(tree)))
    {
        colour(tree) = RED;
        colour(left(tree)) = BLACK;
        colour(right(tree)) = BLACK;
    }

    if (item < data(tree))
    {
        left(tree) = insertRB(left(tree), item, false);
    }
    else
    {
        right(tree) = insertRB(right(tree), item, true);
    }
    // Case 1: Parent and left child are red
    if (inRight && isRed(tree) && isRed(left(tree)))
    {
        tree = rotateRight(tree);
    }
    // Case 2: Red left child and left-left grandchild
    if (isRed(left(tree)) && isRed(left(left(tree))))
    {
        tree = rotateRight(tree);
        colour(tree) = BLACK;
        colour(right(tree)) = RED;
    }
    // Case 3: Red left child and right-left grandchild
    if (isRed(left(tree)) && isRed(right(left(tree))))
    {
        left(tree) = rotateLeft(left(tree));
        tree = rotateRight(tree);
        colour(tree) = BLACK;
        colour(right(tree)) = RED;
    }
    // Case 4:Red right child and left-right grandchild
    if (isRed(right(tree)) && isRed(left(right(tree))))
    {
        right(tree) = rotateRight(right(tree));
        tree = rotateLeft(tree);
        colour(tree) = BLACK;
        colour(left(tree)) = RED;
    }

    return tree;
}

// free memory associated with Tree
void freeTree(Tree t)
{
    if (t != NULL)
    {
        freeTree(left(t));
        freeTree(right(t));
        free(t);
    }
}

// display Tree sideways
void showTreeR(Tree t, int depth)
{
    if (t != NULL)
    {
        showTreeR(right(t), depth + 1);
        int i;
        for (i = 0; i < depth; i++)
            putchar('\t'); // TAB character
        if (isRed(t))
            printf("%s%d%s\n", PRINT_COLOUR_RED, data(t), PRINT_COLOUR_RESET);
        else
            printf("%d\n", data(t));
        showTreeR(left(t), depth + 1);
    }
}

void showTree(Tree t)
{
    showTreeR(t, 0);
}