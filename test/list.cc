#include <cstdio>
#include <cstdlib>

typedef struct Node
{
    struct Node *fwd;
    struct Node *bwd;
    int value;
} Node;

int insert(Node *rootp, int newvalue)
{
    Node *prev;
    Node *next;

    for (prev = rootp; (next = prev->fwd) != NULL; prev = next) {
        if (next->value == newvalue)
            return 0;
        if (next->value > newvalue) {
            break;
        }
    }

    Node *newnode = (Node *) malloc(sizeof(Node));
    if (!newnode)
        return -1;
    newnode->value = newvalue;

    newnode->fwd = next;
    prev->fwd = newnode;

    if (prev != rootp) {
        newnode->bwd = prev;
    } else {
        newnode->bwd = NULL;
    }

    if (next != NULL) {
        next->bwd = newnode;
    } else {
        rootp->bwd = newnode;
    }

    return 1;
}
