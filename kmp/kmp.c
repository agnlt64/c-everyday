#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prefix(const char* p, int n, int* out)
{
    out[0] = 0;
    int k = 0;

    for (size_t q = 1; q < n; q++)
    {
        while (k > 0 && p[k] != p[q])
            k = out[k - 1];

        if (p[k] == p[q])
            k++;
        
        out[q] = k;
    }
}

void kmp(const char* text, const char* pattern)
{
    int txt_len = strlen(text);
    int pat_len = strlen(pattern);
    int* pref = malloc(sizeof(int) * pat_len);
    prefix(pattern, pat_len, pref);

    int q = 0;
    for (size_t i = 0; i < txt_len; i++)
    {
        while (q > 0 && pattern[q] != text[i])
            q = pref[q - 1];

        if (pattern[q] == text[i])
            q++;

        if (q == pat_len)
        {
            printf("pattern appears in position %d\n", i - pat_len + 1);
            q = pref[q - 1];
        }
    }

    free(pref);
}

int main()
{
    const char* text = "sigma sigma boy\n";
    const char* pattern = "sigma";

    kmp(text, pattern);

    return 0;
}