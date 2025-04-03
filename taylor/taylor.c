#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ITER 100

double fact_iter(int n)
{
    if (n < 2)
        return 1;
    double f = 1.0;

    for (size_t i = 1; i <= n; i++)
        f *= i;
    return f;
}

double fastexp(double x, int n)
{
    if (n == 0)
        return 1.0;
    if (n == 1)
        return x;
    if (n % 2 == 0)
        return fastexp(x*x, n/2);
    return x * fastexp(x*x, (n - 1)/2);
}

double exp_approx(int x)
{
    if (x == 0)
        return 1.0;

    double res = 1.0;
    for (size_t i = 1; i < MAX_ITER; i++)
        res += fastexp(x, i) / fact_iter(i);

    return res;
}

double ln_1_plus_x(double x)
{
    if (x <= -1)
    {
        fprintf(stderr, "x must be > -1\n");
        exit(1);
    }
    double res = 0.0;

    for (size_t i = 1; i < MAX_ITER; i++)
    {
        int sign = (i % 2 == 0) ? -1 : 1;
        res += sign * fastexp(x, i) / i;
    }
    return res;
}

double ln_1_minus_x(double x)
{
    if (x >= 1 || x <= -1)
    {
        fprintf(stderr, "error: -1 < x < 1\n");
        exit(1);
    }

    double res = 0.0;
    for (size_t i = 1; i < MAX_ITER; i++)
        res -= fastexp(x, i) / i;

    return res;
}

int main()
{
    int n = 3;
    printf("exp(%d) = %lf\n", n, exp(n));
    printf("exp_approx(%d) = %lf\n", n, exp_approx(n));

    double x = 0.5;
    printf("ln(1 + %lf) = %lf\n", x, log(1 + x));
    printf("ln_1_plus_x(%lf) = %lf\n", x, ln_1_plus_x(x));

    printf("ln(1 - %lf) = %lf\n", x, log(1 - x));
    printf("ln_1_minus_x(%lf) = %lf\n", x, ln_1_minus_x(x));

    return 0;
}