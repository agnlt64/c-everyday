#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void gen_z_table(const char* txt, int* Z)
{
    int txt_len = strlen(txt);
    int left = 0, right = 0;

    for (size_t i = 1; i < txt_len; i++)
    {
        if (i > right)
        {
            left = right = i;
            while (right < txt_len && txt[right - left] == txt[right])
                right++;

            Z[i] = right - left;
            right--;
        }
        else
        {
            int k = i - left;
            if (Z[k] < right - i + 1)
                Z[i] = Z[k];
            else
            {
                left = i;
                while (right < txt_len && txt[right - left] == txt[right])
                    right++;
                
                Z[i] = right - left;
                right--;
            }
        }
    }
}

void search(const char* txt, const char* pat)
{
    int txt_len = strlen(txt);
    int pat_len = strlen(pat);

    char* concat = malloc(txt_len + pat_len + 2);
    sprintf(concat, "%s$%s", pat, txt);

    int* Z = malloc(strlen(concat) * sizeof(int));
    gen_z_table(concat, Z);

    for (size_t i = 0; i < strlen(concat); i++)
    {
        if (Z[i] == pat_len)
            printf("found occurence at position %d\n", i - pat_len - 1);
    }

    free(concat);
    free(Z);
}

int main()
{
    const char* txt = "this is an example text with example words";
    const char* pat = "example";

    search(txt, pat);

    return 0;
}