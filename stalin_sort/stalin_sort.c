#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define N 500

void generate_array(int* arr, int n)
{
    srand(time(NULL));
    for (size_t i = 0; i < n; i++) arr[i] = rand() % 1000;
}

void stalin_sort(int* arr, int* n)
{
    int last_idx = 0;

    for (int i = 1; i < *n; i++)
    {
        if (arr[i] >= arr[last_idx])
        {
            last_idx++;
            arr[last_idx] = arr[i];
        }
    }

    *n = last_idx + 1;

    for (int i = *n; i < N; i++)
        arr[i] = 0;
}

int main()
{
    size_t avg_size = 0;
    for (size_t i = 0; i < N; i++)
    {
        int arr[N];
        generate_array(arr, N);

        int size = N;
        stalin_sort(arr, &size);

        avg_size += size;
    }
    printf("average sorted array size is: %zu\n", avg_size / N);

    return 0;
}