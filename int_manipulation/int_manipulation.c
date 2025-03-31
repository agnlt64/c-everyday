#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int int_len(int n)
{
    if (n < 10)
        return 1;
    return 1 + int_len(n / 10);
}

char* int_to_bin(int n)
{
    int buf_size = 64;
    char* res = calloc(buf_size, 1);
    int i = 0;

    while (n > 0)
    {
        res[i++] = '0' + (n % 2);
        n /= 2;
    }

    for (size_t j = 0; j < i / 2; j++)
    {
        char temp = res[j];
        res[j] = res[i - 1 - j];
        res[i - 1 - j] = temp;
    }

    res[i] = '\0';
    return strdup(res);
}

int bin_to_int(const char* str)
{
    int res = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '1')
            res = res * 2 + 1;
        else if (str[i] == '0')
            res *= 2;
    }

    return res;
}

int sum_of_digits(int n)
{
    if (n < 10)
        return n;
    return (n % 10) + sum_of_digits(n / 10);
}

int sum_of_digits_iter(int n)
{
    if (n < 10)
        return n;

    int res = n % 10;
    while (n > 10)
    {
        n /= 10;
        res += n % 10;
    }
    return res;
}

int reverse_internal(int n, int res)
{
    if (n == 0)
        return res;
    return reverse_internal(n / 10, res * 10 + n % 10);
}

int reverse(int n)
{
    return reverse_internal(n, 0);
}

int reverse_iter(int n)
{
    int res = 0;
    while (n > 0)
    {
        res = res * 10 + n % 10;
        n /= 10;
    }
    return res;
}

int main()
{
    printf("%d\n", int_len(1));
    printf("%d\n", int_len(10));
    printf("%d\n", int_len(100));
    printf("%d\n", int_len(1000));
    printf("%d\n", int_len(10000));
    printf("%d\n", int_len(100000));
    printf("%d\n", int_len(1000000));

    printf("bin(23) = %s\n", int_to_bin(23));
    printf("int(101101) = %d\n", bin_to_int("101101"));

    int tests[5] = {
        32145,
        321,
        9120,
        4512,
        669
    };

    printf("Sum of digits:\n");
    for (size_t i = 0; i < 5; i++)
    {
        int val = tests[i];
        printf("%d => (%d, %d)\n", val, sum_of_digits(val), sum_of_digits_iter(val));
    }

    printf("Reversed:\n");
    for (size_t i = 0; i < 5; i++)
    {
        int val = tests[i];
        printf("%d => (%d, %d)\n", val, reverse(val), reverse_iter(val));
    }
    
    return 0;
}