#include <stdio.h>
#include <limits.h>

#define N 10

void find_min_max(int* arr, int n, int* min, int* max)
{
    *min = INT_MAX;
    *max = INT_MIN;
    for (size_t i = 0; i < n; i++)
    {
        if (arr[i] < *min)
            *min = arr[i];
        else if (arr[i] > *max)
            *max = arr[i];
    }
}

void inverse(int* arr, int n)
{
    for (size_t i = 0; i < n / 2; i++)
    {
        int temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(int* arr, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
    }
}


void remove_duplicates(int* arr, int* n)
{
    bubble_sort(arr, *n);

    int last_idx = 0;
    for (size_t i = 1; i < *n; i++)
    {
        if (arr[i] != arr[last_idx])
            arr[++last_idx] = arr[i];
    }

    *n = last_idx + 1;
}

void print_arr(int* arr, int n)
{
    for (size_t i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    int arr[N] = {-1, 3, 9, 10, 2, 4, 9, -3, 0, 20};
    int min, max;
    find_min_max(arr, N, &min, &max);

    printf("min: %d, max: %d\n", min, max);

    printf("Normal array:\n");
    print_arr(arr, N);
    inverse(arr, N);
    printf("Reversed array:\n");
    print_arr(arr, N);

    int size = N;
    remove_duplicates(arr, &size);
    printf("Removing duplicates\n");
    printf("new size is: %d\n", size);
    print_arr(arr, size);

    return 0;
}