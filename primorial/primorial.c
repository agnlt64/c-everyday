#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>

bool is_prime(uint64_t n)
{
    if (n < 2) return false;

    for (size_t i = 2; i <= (int)sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

uint64_t primorial(int n)
{
    if (n == 0) return 1;

    uint64_t p = 2;
    for (size_t i = 3; i <= n; i++)
    {
        if (is_prime(i))
            p *= i;
    }
    return p;
}

int main()
{
    int n = 10;
    for (size_t i = 0; i < n; i++)
        printf("primorial(%zu) = %"PRIu64"\n", i, primorial(i));

    printf("-------------\n");
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

    for (size_t i = 0; i < n; i++)
        printf("primorial(%d) = %"PRIu64"\n", primes[i], primorial(primes[i]));

    return 0;
}