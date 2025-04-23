#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 256

void table(const char* pat, int pat_len, int** d)
{
    for (size_t j = 0; j < pat_len; j++)
    {
        d[j] = malloc(MAX_CHARS * sizeof(int));

        for (size_t k = 0; k < MAX_CHARS; k++)
            d[j][k] = -1;

        for (size_t k = 0; k < j; k++)
            d[j][pat[k]] = k;
    }
}

int shift(int** d, int j, char c)
{
    if (d[j][c] != -1)
        return j - d[j][c];
    else
        return j + 1;
}

void search(const char* pat, const char* txt)
{
    int pat_len = strlen(pat);
    int txt_len = strlen(txt);

    int** d = malloc(pat_len * sizeof(int*));
    table(pat, pat_len, d);

    int i = 0;
    while (i <= txt_len - pat_len)
    {
        int k = 0;
        for (int j = pat_len - 1; j >= 0; j--)
        {
            if (txt[i + j] != pat[j])
            {
                k = shift(d, j, txt[i + j]);
                break;
            }
        }

        if (k == 0)
        {
            printf("found occurence at position %d\n", i);
            k = 1;
        }
        i += k;
    }

    for (size_t j = 0; j < pat_len; j++)
        free(d[j]);
    free(d);
}

int main()
{
    const char* pat = "example";
    const char* txt = "this is an example text with example words";

    search(pat, txt);

    return 0;
}