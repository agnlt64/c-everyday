#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define len(arr) (sizeof(arr) / sizeof(arr[0]))
#define N 30 // test array size

void generate_array(int* arr, int n)
{
    srand(time(NULL));
    for (size_t i = 0; i < n; i++) arr[i] = rand() % 1000;
}

void merge(int* a, int left, int right, int end, int* b)
{
    int i = left, j = right;

    for (size_t k = left; k < end; k++)
    {
        if (i < right && (j >= end || a[i] <= a[j]))
        {
            b[k] = a[i];
            i++;
        }
        else
        {
            b[k] = a[j];
            j++;
        }
    }
}

void merge_sort(int* a, int* b, int n)
{
    int* src = a;
    int* dest = b;

    for (size_t width = 1; width < n; width *= 2)
    {
        for (size_t i = 0; i < n; i += 2 * width)
        {
            merge(src, i, fmin(i + width, n), fmin(i + 2 * width, n), dest);
        }
        int* temp = src;
        src = dest;
        dest = temp;
    }

    if (src != a) memcpy(a, src, n * sizeof(int));
}

int main()
{
    int arr[N];
    generate_array(arr, N);

    int sorted[N];
    merge_sort(arr, sorted, N);

    for (size_t i = 0; i < N; i++) printf("%d ", sorted[i]);
    printf("\n");

    return 0;
}