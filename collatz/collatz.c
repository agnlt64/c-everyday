#include <stdio.h>
#include <stdlib.h>

#define N 100000

int collatz(int n)
{
    return (n % 2 == 0) ? n / 2 : 3 * n + 1;
}

int collatz_length(long n, int* map)
{
    if (n == 1) return 1;
    if (n <= N && map[n] != 0) return map[n];

    long next = collatz(n);
    int length = 1 + collatz_length(next, map);

    if (n <= N) map[n] = length;
    return length;
}

int main()
{
    int* map = calloc(N + 1, sizeof(int));
    if (!map)
    {
        fprintf(stderr, "error: no memory for the map\n");
        return 1;
    }

    size_t correct = 0;
    for (size_t i = 1; i <= N; i++)
    {
        int length = collatz_length(i, map);
        printf("length for %zu is %d\n", i, length);
        correct++;
    }

    printf("%zu of %d numbers verify the Collatz conjecture\n", correct, N);

    free(map);
    return 0;
}