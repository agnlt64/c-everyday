#include <stdio.h>

int exp_naive(int x, int y, int p)
{
    int res = 1;
    for (size_t i = 0; i < y; i++)
        res = (x * res) % p;
    return res % p;
}

int exp_best(long long x, unsigned int y, int p)
{
    int res = 1;

    x %= p;

    if (x == 0) return 0;

    while (y > 0)
    {
        if (y % 2 == 1)
            res = (x * res) % p;
        
        y /= 2;
        x = (x * x) % p;
    }
    return res;
}

int main()
{
    int b = 4;
    int e = 13;
    int m = 497;

    printf("Power is: %d\n", exp_naive(b, e, m));
    printf("Power is: %d\n", exp_best(b, e, m));

    return 0;
}