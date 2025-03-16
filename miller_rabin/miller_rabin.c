#include <stdio.h>
#include <string.h>
#include <stdlib.h>

long long mulmod(long long a, long long b, long long mod)
{
    long long x = 0, y = a % mod;

    while (b > 0)
    {
        if (b % 2 == 1)
            x = (x + y) % mod;
        y = (2*y) % mod;
        b /= 2;
    }
    return x % mod;
}

long long modulo(long long base, long long exp, long long mod)
{
    long long x = 1;
    long long y = base;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            x = (x*y) % mod;
        y = (y*y) % mod;
        exp /= 2;
    }
    return x % mod;
}

int miller_rabin(long long p, int iterations)
{
    if (p < 2) return 0;

    if (p != 2 && p % 2 == 0) return 0;

    long long s = p - 1;
    while (s % 2 == 0) s /= 2;

    for (int i = 0; i < iterations; i++)
    {
        long long a = rand() % (p - 1) + 1, temp = s;
        long long mod = modulo(a, temp, p);

        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }

        if (mod != p - 1 && temp % 2 == 0) return 0;
    }
    return 1;
}

int main()
{
    int iter = 5;

    printf("is prime %d: %d\n", 5, miller_rabin(5, iter));
    printf("is prime %d: %d\n", 12, miller_rabin(12, iter));
    printf("is prime %d: %d\n", 487, miller_rabin(487, iter));
    printf("is prime %d: %d\n", 5639, miller_rabin(5639, iter));

    return 0;
}