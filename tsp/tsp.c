#define MATRIX_IMPLEMENTATION
#include "matrix.h"

#include <stdio.h>
#include <math.h>

void total_cost(matrix_t* cost, bool* visited, int curr_pos, int n, int count, int cost_so_far, int* ans)
{
    if (count == n && mat_at(cost, curr_pos, 0))
    {
        *ans = fmin(*ans, cost_so_far + mat_at(cost, curr_pos, 0));
        return;
    }

    for (size_t i = 0; i < n; i++)
    {
        if (!visited[i] && mat_at(cost, curr_pos, i))
        {
            visited[i] = true;
            total_cost(cost, visited, i, n, count + 1, cost_so_far + mat_at(cost, curr_pos, i), ans);
            visited[i] = false;
        }
    }
}

int tsp(matrix_t* cost)
{
    if (!is_square(cost))
    {
        fprintf(stderr, "Cost matrix must be square\n");
        return -1;
    }

    int n = cost->rows;
    bool* visited = calloc(n, sizeof(bool));
    visited[0] = true;
    int ans = INT_MAX;
    total_cost(cost, visited, 0, n, 1, 0, &ans);
    free(visited);
    return ans;
}

int main()
{
    matrix_t* cost = mat_from_array(
        (int[]){
            0, 10, 15, 20,
            10, 0, 35, 25,
            15, 35, 0, 30,
            10, 25, 30, 0
        },
        4, 4
    );
    printf("Minimal cost is: %d\n", tsp(cost));
    mat_free(cost);
    return 0;
}