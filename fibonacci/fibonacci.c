#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint64_t fib_rec(int n)
{
    if (n < 2) return n;
    return fib_rec(n - 1) + fib_rec(n - 2);
}

uint64_t fib_memo(int n)
{
    uint64_t* arr = (uint64_t*)malloc(sizeof(uint64_t) * (n + 1));
    arr[0] = 0;
    arr[1] = 1;
    for (int i = 2; i <= n; i++) arr[i] = arr[i - 1] + arr[i - 2];
    uint64_t res = arr[n];
    free(arr);
    return res;
}

uint64_t fib_dyn(int n)
{
    if (n < 2) return n;

    uint64_t a = 0, b = 1, temp;
    for (int i = 2; i<= n; i++)
    {
        temp = a+b;
        a = b;
        b = temp;
    }
    return b;
}

int main(int argc, char* argv[])
{
    const char* perf_options = "[--dyn | --memo | --slow]";
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <n> %s\n", argv[0], perf_options);
        return 1;

    }
    uint64_t (*fib)(int n);
    int n = atoi(argv[1]);

    if (n > 92)
    {
        fprintf(stderr, "fib(%d) overflows if n is greater than 92.\n", n);
        return 1;
    }

    if (strcmp(argv[2], "--memo") == 0)
    {
        printf("Memoized version: \n");
        fib = &fib_memo;
    }
    else if (strcmp(argv[2], "--dyn") == 0)
    {
        printf("Dynamic version: \n");
        fib = &fib_dyn;
    }
    else if (strcmp(argv[2], "--slow") == 0)
    {
        printf("Slow version: \n");
        fib = &fib_rec;
    }
    else
    {
        fprintf(stderr, "Invalid perf option specified. Valid options are: %s\n", perf_options);
        return 1;
    }
    printf("fib(%d) = %llu\n", n, fib(n));

    return 0;
}