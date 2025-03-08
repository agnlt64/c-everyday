#include <stdio.h>
#include <stdlib.h>

#define QUEUE_IMPLEMENTATION
#include "queue.h"

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

void inorder_traversal(node_t* root)
{
    if (root)
    {
        inorder_traversal(root->left);
        printf("%d ", root->value);
        inorder_traversal(root->right);
    }
}

void preorder_traversal(node_t* root)
{
    if (root)
    {
        printf("%d ", root->value);
        preorder_traversal(root->left);
        preorder_traversal(root->right);
    }
}

void bfs(node_t* root)
{
    if (!root) return;

    queue_t* q = queue_alloc();

    queue_enqueue(q, root);

    while (!queue_is_empty(q))
    {
        node_t* current = queue_dequeue(q);
        printf("%d ", current->value);
        
        if (current->left) queue_enqueue(q, current->left);
        if (current->right) queue_enqueue(q, current->right);
    }
}

void postorder_traversal(node_t* root)
{
    if (root)
    {
        postorder_traversal(root->left);
        postorder_traversal(root->right);
        printf("%d ", root->value);
    }
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

    printf("BFS:\n");
    bfs(root);
    printf("\n");

    printf("Postorder traversal:\n");
    postorder_traversal(root);
    printf("\n");

    printf("Preorder traversal:\n");
    preorder_traversal(root);
    printf("\n");

    printf("Inorder traversal:\n");
    inorder_traversal(root);
    printf("\n");

    node_free(root);
    return 0;
}