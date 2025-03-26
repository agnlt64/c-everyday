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
    
    return 0;
}