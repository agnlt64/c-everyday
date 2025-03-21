#include <stdio.h>
#include <math.h>
#include <string.h>

void lcs(const char* x, const char* y)
{
    int m = strlen(x);
    int n = strlen(y);
    int L[m + 1][n + 1];

    for (size_t i = 0; i <= m; i++)
    {
        for (size_t j = 0; j <= n; j++)
        {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (x[i - 1] == y[j - 1])
                L[i][j] = L[i - 1][j - 1] + 1;
            else
                L[i][j] = fmax(L[i - 1][j], L[i][j - 1]);
        }
    }

    int index = L[m][n];
    char lcs[index + 1];
    lcs[index] = '\0';

    int i = m, j = n;
    while (j > 0 && i > 0)
    {
        if (x[i - 1] == y[j - 1])
        {
            lcs[index - 1] = x[i - 1];
            i--;
            j--;
            index--;
        }
        else if (L[i - 1][j] > L[i][j - 1])
            i--;
        else
            j--;
    }
    printf("Longest Common Subsequence is %s\n", lcs);
}

int main()
{
    lcs("AGGTAB", "GXTXAYB");
    return 0;
}
