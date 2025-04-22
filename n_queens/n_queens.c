#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int place_queens(int i, int* cols, int col_size, int* left_diag, int* right_diag, int* cur, int* out)
{
    if (i == col_size)
    {
        memcpy(out, cur, col_size * sizeof(int));
        return 1;
    }

    for (size_t j = 0; j < col_size; j++)
    {
        if (cols[j] || right_diag[i + j] || left_diag[i - j + col_size - 1])
            continue;
        
        cols[j] = 1;
        right_diag[i + j] = 1;
        left_diag[i - j + col_size - 1] = 1;
        cur[i] = j + 1;

        if (place_queens(i + 1, cols, col_size, left_diag, right_diag, cur, out))
            return 1;

        cols[j] = 0;
        right_diag[i + j] = 0;
        left_diag[i - j + col_size - 1] = 0;
    }
    return 0;
}

int n_queens(int* out, int n)
{
    int* cols = calloc(n, sizeof(int));
    int* left_diag = calloc(2 * n, sizeof(int));
    int* right_diag = calloc(2 * n, sizeof(int));
    int* cur = calloc(n, sizeof(int));

    int result = place_queens(0, cols, n, left_diag, right_diag, cur, out);

    free(cols);
    free(left_diag);
    free(right_diag);
    free(cur);

    return result;
}

int main()
{
    int n = 4;
    int* ans = malloc(n * sizeof(int));
    if (n_queens(ans, n))
    {
        for (size_t i = 0; i < n; i++)
            printf("%d ", ans[i]);
        printf("\n");
    }
    else
        printf("No solution exists\n");

    free(ans);

    return 0;
}