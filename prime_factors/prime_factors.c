#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

bool is_prime(int n)
{
    if (n < 2)
        return false;

    for (size_t i = 2; i <= (int)sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int prime_factors(int n, int* out)
{
    int i = 2;
    int j = 0;

    while (i * i <= n)
    {
        if (n % i)
            i++;
        else
        {
            n /= i;
            out[j++] = i;
        }
    }

    if (n > 1)
        out[j++] = n;

    return j;
}

int count_occ(int k, int* arr, int len)
{
    int occ = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (arr[i] == k)
            occ++;
    }
    return occ;
}

void print_factors(int n, int* arr, int len)
{
    printf("%d = ", n);

    bool printed[1000] = {0};
    for (size_t i = 0; i < len; i++)
    {
        int val = arr[i];

        if (!printed[val])
        {
            int pow = count_occ(val, arr, len);
            if (pow == 1)
                printf("%d", val);
            else
                printf("%d^%d", val, pow);

            if (i != len - 1)
                printf(" x ");
            printed[val] = true;
        }
    }
    printf("\n");
}

int main()
{
    int n = 456;
    int* factors = malloc(n * sizeof(int));

    int nb_factors = prime_factors(n, factors);
    print_factors(n, factors, nb_factors);

    return 0;
}
