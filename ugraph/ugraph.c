#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

void ugraph_add_edge(ugraph_t* g, char u, char v)
{
    u -= 65;
    v -= 65;
    if (u >= g->size || v >= g->size)
    {
        fprintf(stderr, "Invalid node index!\n");
        return;
    }
    mat_at(g->adj, u, v) = 1;
    mat_at(g->adj, v, u) = 1;
}

void ugraph_remove_edge(ugraph_t* g, char u, char v)
{
    u -= 65;
    v -= 65;
    if (u >= g->size || v >= g->size)
    {
        fprintf(stderr, "Invalid node index!\n");
        return;
    }
    mat_at(g->adj, u, v) = 0;
    mat_at(g->adj, v, u) = 0;
}

bool ugraph_has_edge(ugraph_t* g, char u, char v)
{
    u -= 65;
    v -= 65;
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

void ugraph_bsf(ugraph_t* g, char start)
{
    bool* visited = calloc(g->size, sizeof(bool));
    queue_t* q = queue_alloc();

    start -= 65;
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
        if (mat_at(g->adj, node , i) && !visited[i])
        {
            ugraph_dfs_inner(g, i, visited);
        }
    }
}

void ugraph_dfs(ugraph_t* g, char start)
{
    bool* visited = calloc(g->size, sizeof(bool));
    ugraph_dfs_inner(g, start - 65, visited);
    printf("\n");
    free(visited);
}

void ugraph_free(ugraph_t* g)
{
    mat_free(g->adj);
    free(g);
}

int ugraph_dijkstra_min(int* dist, bool* visited, int n)
{
    int min = INT_MAX;
    int node = -1;

    for (size_t i = 0; i < n; i++)
    {
        if (dist[i] < min && !visited[i])
        {
            min = dist[i];
            node = i;
        }
    }
    return node;
}

void ugraph_dijkstra(ugraph_t* g, char start, char end)
{
    int n = g->size;
    int dist[n];
    int prev[n];
    bool visited[n];
    char orig_start = start;
    char orig_end = end;

    start -= 65;
    end -= 65;

    for (size_t i = 0; i < g->adj->cols; i++)
    {
        dist[i] = INT_MAX;
        prev[i] = -1;
        visited[i] = false;
    }
    dist[start] = 0;

    for (size_t count = 0; count < n; count++)
    {
        int u = ugraph_dijkstra_min(dist, visited, n);
        if (u == -1 || u == end) break;
        visited[u] = true;

        for (size_t v = 0; v < n; v++)
        {
            int weight = mat_at(g->adj, u, v);
            if (!visited[v]
                && ugraph_has_edge(g, u + 65, v + 65)
                && dist[u] != INT_MAX
                && dist[u] + weight < dist[v]
            )
            {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }

    if (dist[end] == INT_MAX)
    {
        printf("No path from %c to %c\n", orig_start, orig_end);
        return;
    }

    printf("Shortest path from %c to %c:\n", orig_start, orig_end);
    int path[n];
    int count = 0;
    for (int at = end; at != -1; at = prev[at])
        path[count++] = at;

    for (int i = count - 1; i >= 0; i--)
        printf("%c%s", path[i] + 65, i == 0 ? "\n" : " -> ");

    printf("Cost: %d\n", dist[end]);
}

int main()
{
    ugraph_t* g = ugraph_alloc(8);

    ugraph_add_edge(g, 'A', 'B');
    ugraph_add_edge(g, 'A', 'E');
    ugraph_add_edge(g, 'A', 'F');

    ugraph_add_edge(g, 'C', 'E');

    ugraph_add_edge(g, 'D', 'E');
    ugraph_add_edge(g, 'D', 'F');
    ugraph_add_edge(g, 'D', 'H');

    ugraph_add_edge(g, 'F', 'G');

    ugraph_add_edge(g, 'G', 'H');

    ugraph_print(g);

    ugraph_dijkstra(g, 'A', 'C');
    ugraph_dijkstra(g, 'A', 'B');
    ugraph_dijkstra(g, 'F', 'B');
    ugraph_dijkstra(g, 'H', 'A');
    ugraph_dijkstra(g, 'E', 'H');

    printf("BFS:\n");
    ugraph_bsf(g, 'A');

    printf("DFS:\n");
    ugraph_dfs(g, 'A');

    ugraph_free(g);
    return 0;
}