#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void manacher_odd(const char* s, int* result, int* result_size)
{
    int n = strlen(s);
    char* modified_s = malloc(n + 3);

    modified_s[0] = '$';
    strcpy(modified_s + 1, s);
    modified_s[n + 1] = '^';
    modified_s[n + 2] = '\0';

    int* p = calloc(n + 2, sizeof(int));
    int l = 0, r = 1;

    for (size_t i = 1; i <= n; i++)
    {
        if (r > i)
        {
            if (r - i < p[l + (r - i)])
                p[i] = r - i;
            else
                p[i] = p[l + (r - i)];
        }
        else
            p[i] = 0;

        while (modified_s[i - p[i]] == modified_s[i + p[i]])
            p[i]++;

        if (i + p[i] > r)
        {
            l = i - p[i];
            r = i + p[i];
        }
    }

    for (size_t i = 0; i < n; i++)
        result[i] = p[i + 1];

    free(modified_s);
    free(p);

    *result_size = n;
}

void manacher(const char* s, int* result, int* result_size)
{
    int n = strlen(s);
    char* t = malloc(2 * n + 2);
    
    t[0] = '#';
    for (size_t i = 0; i < n; i++)
    {
        t[2 * i + 1] = s[i];
        t[2 * i + 2] = '#';
    }
    t[2 * n + 1] = '\0';

    int odd_res_size;
    int* odd_res = malloc((2 * n + 2) * sizeof(int));
    manacher_odd(t, odd_res, &odd_res_size);

    for (size_t i = 0; i < odd_res_size - 2; i++)
        result[i] = odd_res[i + 1];

    free(t);
    free(odd_res);

    *result_size = odd_res_size - 2;
}

int main()
{
    const char* input = "abacaba";
    int n = strlen(input);

    int res_sz = 2*n;
    int* res = malloc(res_sz * sizeof(int));

    manacher(input, res, &res_sz);

    printf("All palindromes lengths:\n");
    for (size_t i = 0; i < res_sz; i++)
        printf("%d ", res[i]);
    printf("\n");

    free(res);

    return 0;
}