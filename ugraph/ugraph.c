#include <stdio.h>
#include <stdlib.h>

#define MATRIX_IMPLEMENTATION
#include "matrix.h"

typedef struct upgraph {
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
    mat_print(g->adj);
}

void ugraph_free(ugraph_t* g)
{
    mat_free(g->adj);
    free(g);
}

int main()
{
    ugraph_t* g = ugraph_alloc(5);

    ugraph_add_edge(g, 0, 1);
    ugraph_add_edge(g, 0, 2);
    ugraph_add_edge(g, 1, 3);
    ugraph_add_edge(g, 3, 4);
    ugraph_add_edge(g, 2, 4);

    ugraph_print(g);
    printf("Edge (0, 1) exists ? %s\n", ugraph_has_edge(g, 0, 1) ? "Yes" : "No");
    printf("Edge (1, 4) exists ? %s\n", ugraph_has_edge(g, 1, 4) ? "Yes" : "No");

    ugraph_remove_edge(g, 0, 1);
    printf("After removing edge (0, 1): \n");
    printf("Edge (0, 1) exists ? %s\n", ugraph_has_edge(g, 0, 1) ? "Yes" : "No");
    ugraph_print(g);

    ugraph_free(g);
    return 0;
}