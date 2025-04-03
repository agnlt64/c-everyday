#include <stdio.h>

#define N 41
#define SKIP 3

int josephus_rec(int n, int skip)
{
    return n > 1 ? (josephus_rec(n - 1, skip) + skip - 1) % n + 1 : 1;
}

int josephus_iter(int n, int k)
{
    k--;
    int arr[n];

    for (size_t i = 0; i < n; i++)
        arr[i] = 1;

    int count = 0, next = 0, num = 1;

    while (count < ( n - 1))
    {
        while (num <= k)
        {
            next = (next + 1) % n;

            if (arr[next] == 1)
                num++;
        }

        num = 1;

        arr[next] = 0;
        count++;
        next = (next + 1) % n;

        while (arr[next] == 0)
            next = (next + 1) % n;
    }
    return next + 1;
}

int josephus_best(int n, int skip)
{
    int res = 0;

    for (size_t i = 1; i <= n; i++)
        res = (res + skip) % i;

    return res + 1;
}

int main()
{
    printf("%d\n", josephus_best(N, SKIP));
    printf("%d\n", josephus_rec(N, SKIP));
    printf("%d\n", josephus_iter(N, SKIP));

    return 0;
}