#include <stdio.h>
#include <stdlib.h>

#define MATRIX_IMPLEMENTATION
#include "matrix.h"

#define QUEUE_IMPLEMENTATION
#include "queue.h"

typedef struct ugraph {
    matrix_t* adj;
    size_t size;
} ugraph_t;

ugraph_t* ugraph_alloc(size_t size)
{
    ugraph_t* g = malloc(sizeof(ugraph_t));
    g->size = size;
    g->adj = mat_alloc(g->size, g->size);
    return g;
}

void ugraph_add_edge(ugraph_t* g, size_t u, size_t v)
{
    if (u >= g->size || v >= g->size)
    {
        fprintf(stderr, "Invalid node index!\n");
        return;
    }
    mat_at(g->adj, u, v) = 1;
    mat_at(g->adj, v, u) = 1;
}

void ugraph_remove_edge(ugraph_t* g, size_t u, size_t v)
{
    if (u >= g->size || v >= g->size)
    {
        fprintf(stderr, "Invalid node index!\n");
        return;
    }
    mat_at(g->adj, u, v) = 0;
    mat_at(g->adj, v, u) = 0;
}

bool ugraph_has_edge(ugraph_t* g, size_t u, size_t v)
{
    if (u >= g->size || v >= g->size) return false;
    return mat_at(g->adj, u, v) == 1;
}

void ugraph_print(ugraph_t* g)
{
    printf("Adjacency matrix:\n");
    printf("  ");
    int c = 65; // A in ascii
    for (size_t i = 0; i < g->size; i++) printf("%c ", c++);
    printf("\n");
    c = 65;
    mat_for(g->adj,
        if (i == 0) printf("%c ", c++);
        printf("%d ", mat_at(g->adj, i, j));
        if (i == g->adj->cols - 1) printf("\n");
    );
}

void ugraph_bsf(ugraph_t* g, int start)
{
    bool* visited = calloc(g->size, sizeof(bool));
    queue_t* q = queue_alloc();

    visited[start] = true;
    queue_enqueue(q, start);

    while (!queue_is_empty(q))
    {
        int node = queue_dequeue(q);
        printf("%c ", node + 65);

        for (size_t i = 0; i < g->size; i++)
        {
            if (mat_at(g->adj, node, i) && !visited[i])
            {
                visited[i] = true;
                queue_enqueue(q, i);
            }
        }
    }
    printf("\n");

    queue_free(q);
    free(visited);
}

void ugraph_dfs_inner(ugraph_t* g, int node, bool* visited)
{
    visited[node] = true;
    printf("%c ", node + 65);
    for (size_t i = 0; i < g->size; i++)
    {
        if (mat_at(g->adj, node, i) && !visited[i])
        {
            ugraph_dfs_inner(g, i, visited);
        }
    }
}

void ugraph_dfs(ugraph_t* g, int start)
{
    bool* visited = calloc(g->size, sizeof(bool));
    ugraph_dfs_inner(g, start, visited);
    printf("\n");
    free(visited);
}

void ugraph_free(ugraph_t* g)
{
    mat_free(g->adj);
    free(g);
}

int main()
{
    ugraph_t* g = ugraph_alloc(6);

    ugraph_add_edge(g, 0, 1);
    ugraph_add_edge(g, 0, 4);

    ugraph_add_edge(g, 1, 2);
    ugraph_add_edge(g, 1, 4);

    ugraph_add_edge(g, 2, 3);
    ugraph_add_edge(g, 2, 4);
    ugraph_add_edge(g, 2, 5);

    ugraph_add_edge(g, 3, 4);
    ugraph_add_edge(g, 3, 5);

    ugraph_add_edge(g, 4, 5);

    ugraph_print(g);

    printf("BFS:\n");
    ugraph_bsf(g, 0);

    printf("DFS:\n");
    ugraph_dfs(g, 0);

    ugraph_free(g);
    return 0;
}