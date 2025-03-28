#include <stdio.h>
#include <math.h>

int base_n(int n)
{
    int b = 10;

    while (b < n)
        b *= 10;

    return b / 10;
}

int rest(int number, int n, int b)
{
    return number - (n * b);
}

int karatsuba(int a, int b)
{
    if (a < 10 || b < 10)
        return a * b;

    int base = base_n(a > b ? a : b);

    int high_a = fmax(a, base);
    int high_b = fmax(b, base);

    int high_prod = high_a * high_b;
    int low_prod = rest(a, base, high_a) * rest(b, base, high_b);
    int cross_prod = (high_a + rest(a, base, high_a)) * (high_b + rest(b, base, high_b));

    cross_prod = cross_prod - high_prod - low_prod;
    return high_prod * base * base + cross_prod * base + low_prod;
}

int main()
{
    int a = 884845;
    int b = 12453;
    printf("%d x %d = %d\n", a, b, karatsuba(a, b));

    return 0;
}