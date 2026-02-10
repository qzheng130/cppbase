#include <cstdio>
#include <cstdlib>

typedef struct Node
{
    struct Node *link;
    int value;
} Node;

bool insert(Node **linkp, int new_value)
{
    Node *current = *linkp;
    Node *new_node;

    while (current && current->value < new_value) {
        linkp = &current->link;
        current = *linkp;
    }

    new_node = (Node *) malloc(sizeof(Node));
    if (!new_node) {
        return false;
    }

    new_node->value = new_value;
    new_node->link = current;

    *linkp = new_node;

    return true;
}
