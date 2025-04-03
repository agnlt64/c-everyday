#include <stdio.h>
#include <string.h>

#define D 256
#define Q 101

void search(char* pattern, char* txt)
{
    int m = strlen(pattern);
    int n = strlen(txt);
    size_t i, j;

    int p = 0;
    int t = 0;
    int h = 1;

    for (i = 0; i < m - 1; i++)
        h = (h * D) % Q;

    for (i = 0; i < m; i++)
    {
        p = (D * p + pattern[i]) % Q;
        t = (D * t + txt[i]) % Q;
    }

    for (i = 0; i <= n - m; i++)
    {
        if (p == t)
        {
            for (j = 0; j < m; j++)
            {
                if (txt[i + j] != pattern[j])
                    break;
            }

            if (j == m)
                printf("pattern found at index %zu\n", i);
        }

        if (i < n - m)
        {
            t = (D * (t - txt[i] * h) + txt[i + m]) % Q;
            if (t < 0)
                t += Q;
        }
    }
}

int main()
{
    char* txt = "ABCCDDAEFG";
    char* pattern = "CDD";
    search(pattern, txt);

    return 0;
}