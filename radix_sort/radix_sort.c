#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 500

void generate_array(int* arr, int n)
{
    srand(time(NULL));
    for (size_t i = 0; i < n; i++) arr[i] = rand() % 1000;
}

int get_max(int* arr, int n)
{
    int m = arr[0];
    for (size_t i = 0; i < n; i++)
        if (arr[i] > m)
            m = arr[i];
    return m;
}

void count_sort(int* arr, int n, int exp)
{
    int output[n];
    int count[10] = {0};

    for (size_t i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (size_t i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--)
    {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (size_t i = 0; i < n; i++)
        arr[i] = output[i];
}

void radix_sort(int* arr, int n)
{
    int m = get_max(arr, n);

    for (int exp = 1; m / exp > 0; exp *= 10)
        count_sort(arr, n, exp);
}

int main()
{
    int arr[N];
    generate_array(arr, N);

    radix_sort(arr, N);

    for (size_t i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}