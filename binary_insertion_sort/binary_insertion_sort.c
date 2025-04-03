#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 500

void generate_array(int* arr, int n)
{
    srand(time(NULL));
    for (size_t i = 0; i < n; i++) arr[i] = rand() % 1000;
}

int binary_search(int* arr, int value, int start, int end)
{
    while (start < end)
    {
        int mid = start + (end - start) / 2;
        if (arr[mid] <= value)
            start = mid + 1;
        else
            end = mid;
    }
    return start;
}

void binary_insertion_sort(int* arr, int size)
{
    for (size_t i = 1; i < size; i++)
    {
        int value = arr[i];
        int pos = binary_search(arr, value, 0, i);

        for (int j = i; j > pos; j--)
            arr[j] = arr[j - 1];
        arr[pos] = value;
    }
}

int main()
{
    int arr[N];
    generate_array(arr, N);

    binary_insertion_sort(arr, N);

    for (size_t i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}