#include <stdio.h>
#include <stdlib.h>

long long mult(long long a, long long b, long long mod)
{
    return a * b % mod;
}

long long f(long long x, long long c, long long mod)
{
    return (mult(x, x, mod) + c) % mod;
}

int gcd(int a, int b)
{
    while (b != 0)
    {
        int r = b;
        b = a % b;
        a = r;
    }
    return a;
}

long long rho(long long n, long long x0, long long c)
{
    long long x = x0;
    long long y = x0;
    long long g = 1;
    
    while (g == 1)
    {
        x = f(x, c, n);
        y = f(y, c, n);
        y = f(y, c, n);
        g = gcd(abs(x - y), n);
    }
    return g;
}

int main()
{
    int n = 8051;
    printf("%llu\n", rho(n, 2, 1));
    return 0;
}