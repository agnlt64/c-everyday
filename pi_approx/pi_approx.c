#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define ITER_MAX (100*100*100*100)

double wallis_prod()
{
    double res = 1.0f;
    for (size_t i = 1; i < ITER_MAX; i++)
    {
        double n = 4.0f * i * i;
        res *= n / (n - 1);
    }
    return 2 * res;
}

double madhava_prod()
{
    double res = 0.0f;
    double sqrt12 = 2 * 1.7320508076;

    for (size_t i = 0; i < ITER_MAX; i++)
    {
        res += sqrt12 * pow(-1.0f / 3, i) / (2 * i + 1);
    }
    return res;
}

int main()
{
    printf("libc PI:%.15f\n", M_PI);

    double wallis_pi = wallis_prod();
    printf("Wallis PI: %.15f\n", wallis_pi);
    printf("Difference: %.15f\n", M_PI - wallis_pi);

    double madhava_pi = madhava_prod();
    printf("Madhava PI: %.15f\n", madhava_pi);
    printf("Difference: %.15f\n", M_PI - madhava_pi);

    return 0;
}