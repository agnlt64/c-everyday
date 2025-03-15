#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 500

void generate_array(int* arr, int n)
{
    srand(time(NULL));
    for (size_t i = 0; i < n; i++) arr[i] = rand() % 1000;
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int find_max(int* arr, int n)
{
    int idx = 0;

    for (size_t i = 0; i < n; i++)
    {
        if (arr[i] > arr[idx]) idx = i;
    }
    return idx;
}

void flip(int* arr, int k)
{
    int left = 0;
    while (left < k)
        swap(&arr[left++], &arr[k--]);
}

void pancake_sort(int* arr, int n)
{
    while (n > 1)
    {
        int idx = find_max(arr, n);
        if (idx != n - 1)
        {
            if (idx != 0)
                flip(arr, idx);
            flip(arr, n - 1);
        }
        n--;
    }
}

int main()
{
    int arr[N];
    generate_array(arr, N);

    pancake_sort(arr, N);

    for (size_t i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}