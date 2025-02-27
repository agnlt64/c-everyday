#include <stdio.h>

int gcd_iter(int a, int b)
{
    while (b != 0)
    {
        int r = b;
        b = a % b;
        a = r;
    }
    return a;
}

int gcd_rec(int a, int b)
{
    if (b == 0) return a;
    return gcd_rec(b, a % b);
}

int lcm(int a, int b)
{
    return a / gcd_iter(a, b) * b;
}

int main()
{
    int a = 20;
    int b = 16;

    printf("GCD(%d, %d) = %d\n", a, b, gcd_iter(a, b));
    printf("GCD(%d, %d) = %d\n", a, b, gcd_rec(a, b));
    printf("LCM(%d, %d) = %d\n", a, b, lcm(a, b));

    return 0;
}