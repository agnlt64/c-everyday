#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

uint64_t fact_rec(uint64_t n)
{
    if (n == 0) return 1;
    return n * fact_rec(n - 1);
}

uint64_t fact_iter(uint64_t n)
{
    uint64_t f = 1;
    for (size_t i = 1; i <= n; i++)
    {
        f *= i;
    }
    return f;
}

int main()
{
    for (size_t i = 0; i < 20; i++)
    {
        printf("fact_iter(%zu) = %" PRIu64 "\n", i, fact_iter(i));
        printf("fact_rec(%zu) = %" PRIu64 "\n", i, fact_rec(i));
    }

    return 0;
}