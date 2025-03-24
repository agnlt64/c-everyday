#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void init_dp(int x, int y, int** out)
{
    for (size_t i = 0; i < y + 1; i++)
    {
        out[i][0] = i;
        for (size_t j = 0; j < x + 1; j++)
        {
            out[0][j] = j;
        }
    }
}

int lev(const char* a, const char* b)
{
    int m = strlen(a);
    int n = strlen(b);

    int** dp = malloc((m + 1) * sizeof(int*));
    for (size_t i = 0; i < m + 1; i++)
        dp[i] = calloc((n + 1), sizeof(int));

    init_dp(n, m, dp);

    for (size_t i = 1; i <= m; i++)
    {
        for (size_t j = 1; j <= n; j++)
        {
            int cost = a[i - 1] == b[j - 1] ? 0 : 1;
            dp[i][j] = fmin(fmin(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + cost);
        }
    }

    int result = dp[m][n];
    for (size_t i = 0; i < m + 1; i++)
    {
        free(dp[i]);
    }
    free(dp);
    return result;
}

int main()
{
    printf("%d\n", lev("kitten", "sitting"));
    printf("%d\n", lev("flaw", "lawn"));
    printf("%d\n", lev("saturday", "sunday"));
    printf("%d\n", lev("hello", "world"));

    return 0;
}