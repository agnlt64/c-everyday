#include <stdio.h>

float power(float x, int n)
{
    if (n == 0)
        return 1.0f;
    
    if (n == 1)
        return x;
    
    if (x == 0 || x == 1)
        return x;

    int exp = n;
    if (n < 0)
    {
        x = 1.0f/x;
        exp = -n;
    }

    float res = 1.0f;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            res *= x;
        x *= x;
        exp /= 2;
    }
    return res;
}

int main()
{

    printf("%f\n", power(0, 0));
    printf("%f\n", power(1, 0));
    printf("%f\n", power(2, 0));
    printf("%f\n", power(1, 2));
    printf("%f\n", power(1, 3));
    printf("%f\n", power(2, 1));
    printf("%f\n", power(1, 1));

    for (size_t i = 0; i < 12; i++)
        printf("2^%zu = %f\n", i, power(2, i));

    return 0;
}