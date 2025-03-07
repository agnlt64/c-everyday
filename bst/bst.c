#include <stdio.h>
#include <stdlib.h>

typedef struct node_t {
    int value;
    struct node_t* left;
    struct node_t* right;
} node_t;

node_t* node_alloc(int value)
{
    node_t* node = malloc(sizeof(node_t));
    node->value = value;
    node->left = node->right = NULL;
    return node;
}

void node_free(node_t* node)
{
    if (node)
    {
        node_free(node->left);
        node_free(node->right);
        free(node);
    }
}

node_t* node_insert(node_t* root, int value)
{
    if (root == NULL)
    return node_alloc(value);

    // don't add the same value twice
    if (value < root->value)
        root->left = node_insert(root->left, value);
    else if (value > root->value)
        root->right = node_insert(root->right, value);
    return root;
}

node_t* node_delete(node_t* root, int value)
{
    if (!root) return NULL;

    if (value < root->value)
        root->left = node_delete(root->left, value);
    else if (value > root->value)
        root->right = node_delete(root->right, value);
    else
    {
        if (!root->left && !root->right)
        {
            free(root);
            return NULL;
        }

        if (!root->left)
        {
            node_t* temp = root->right;
            free(root);
            return temp;
        }

        while (root->right->left) root->right = root->right->left;
        node_t* temp = root->right;
        root->value = temp->value;
        root->right = node_delete(root->right, temp->value);
    }

    return root;
}

int arity(node_t* root)
{
    int ar = 0;
    if (root->left) ar++;
    if (root->right) ar++;
    return ar;
}

int height(node_t* root)
{
    if (!root) return 0;
    int left = height(root->left);
    int right = height(root->right);
    return (left > right ? left : right) + 1; 
}

int main()
{
    node_t* root = NULL;

    root = node_insert(root, 5);
    root = node_insert(root, 3);
    root = node_insert(root, 7);
    root = node_insert(root, 2);
    root = node_insert(root, 4);
    root = node_insert(root, 6);
    root = node_insert(root, 8);

    printf("Arity:\n");
    printf("%d\n", arity(root));

    printf("Height:\n");
    printf("%d\n", height(root));

    printf("Deleting node 3\n");
    node_delete(root, 3);

    printf("Arity:\n");
    printf("%d\n", arity(root));

    printf("Height:\n");
    printf("%d\n", height(root));

    node_free(root);
    return 0;
}