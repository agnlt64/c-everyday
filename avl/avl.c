#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define QUEUE_IMPLEMENTATION
#include "queue.h"

node_t* node_alloc(int value)
{
    node_t* node = malloc(sizeof(node_t));
    node->value = value;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

int get_height(node_t* node)
{
    return (node == NULL) ? 0 : node->height;
}

int get_balance(node_t* node)
{
    return (node == NULL) ? 0 : get_height(node->left) - get_height(node->right);
}

node_t* rotate_right(node_t* node)
{
    node_t* new_root = node->left;
    node_t* right = new_root->right;

    new_root->right = node;
    node->left = right;

    node->height = 1 + fmax(get_height(node->left), get_height(node->right));
    new_root->height = 1 + fmax(get_height(new_root->left), get_height(new_root->right));

    return new_root;
}

node_t* rotate_left(node_t* node)
{
    node_t* new_root = node->right;
    node_t* left = new_root->left;

    new_root->left = node;
    node->right = left;

    node->height = 1 + fmax(get_height(node->left), get_height(node->right));
    new_root->height = 1 + fmax(get_height(new_root->left), get_height(new_root->right));

    return new_root;
}

node_t* avl_insert(node_t* node, int value)
{
    if (node == NULL)
        return node_alloc(value);

    if (value < node->value)
        node->left = avl_insert(node->left, value);
    if (value > node->value)
        node->right = avl_insert(node->right, value);
    else
        return node;

    node->height = 1 + fmax(get_height(node->left), get_height(node->right));

    int balance = get_balance(node);

    if (balance > 1 && value < node->left->value) return rotate_right(node);
    
    if (balance < -1 && value > node->right->value) return rotate_left(node);

    if (balance > 1 && value > node->left->value)
    {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    if (balance < -1 && value < node->right->value)
    {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

node_t* avl_delete(node_t* node, int value)
{
    if (node == NULL)
        return node;

    if (value < node->value)
        node->left = avl_delete(node->left, value);
    else if (value > node->value)
        node->right = avl_delete(node->right, value);
    else
    {
        if (node->left == NULL || node->right == NULL)
        {
            node_t* temp = node->left ? node->left : node->right;
            free(node);
            return temp;
        }

        node_t* temp = node->right;
        while (temp->left != NULL) temp = temp->left;

        node->value = temp->value;
        node->right = avl_delete(node->right, temp->value);
    }

    node->height = 1 + fmax(get_height(node->left), get_height(node->right));

    int balance = get_balance(node);
    int balance_left = get_balance(node->left);
    int balance_right = get_balance(node->right);

    if (balance > 1 && balance_left >= 0)
        return rotate_right(node);

    if (balance > 1 && balance_left < 0)
    {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    if (balance < -1 && balance_right <= 0)
        return rotate_left(node);

    if (balance < -1 && balance_right > 0)
    {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    return node;
}

void avl_free(node_t* node)
{
    if (node)
    {
        avl_free(node->left);
        avl_free(node->right);
        free(node);
    }
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

void postorder_traversal(node_t* root)
{
    if (root)
    {
        postorder_traversal(root->left);
        postorder_traversal(root->right);
        printf("%d ", root->value);
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

int main()
{
    node_t* root = NULL;
    root = avl_insert(root, 50);
    root = avl_insert(root, 17);
    root = avl_insert(root, 9);
    root = avl_insert(root, 23);
    root = avl_insert(root, 14);
    root = avl_insert(root, 12);
    root = avl_insert(root, 19);
    root = avl_insert(root, 76);
    root = avl_insert(root, 54);
    root = avl_insert(root, 72);
    root = avl_insert(root, 67);

    printf("AVL height: %d\n", get_height(root));
    root = avl_delete(root, 54);
    printf("AVL height after deletion: %d\n", get_height(root));

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

    avl_free(root);

    return 0;
}