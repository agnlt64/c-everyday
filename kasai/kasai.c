#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void compute_lcp(const char* s, int* pos, int* lcp, int n)
{
    int rank[n];

    for (size_t i = 0; i < n; i++)
        rank[pos[i]] = i;

    int h = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (rank[i] > 0)
        {
            int k = pos[rank[i] - 1];
            while (i + h < n && k + h < n && s[i + h] == s[k + h])
                h++;
            lcp[rank[i]] = h;
            if (h > 0)
                h--;
        }
        else
            lcp[rank[i]] = 0;
    }
}

int main()
{
    const char* s = "banana";
    int n = strlen(s);
    int pos[] = {5, 3, 1, 0, 4, 2};
    int lcp[n];

    compute_lcp(s, pos, lcp, n);

    printf("lcp array:\n");
    for (size_t i = 0; i < n; i++)
        printf("%d ", lcp[i]);
    printf("\n");

    return 0;
}