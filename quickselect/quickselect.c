#include <stdio.h>

#define N 10

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* arr, int left, int right)
{
    int pivot = arr[right];
    int i = left;

    for (size_t j = left; j < right; j++)
    {
        if (arr[j] <= pivot)
        {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[right]);
    return i;
}

int quick_select(int* arr, int left, int right, int k)
{
    if (left <= right)
    {
        int pivot_idx = partition(arr, left, right);
        if (pivot_idx == k)
            return arr[pivot_idx];
        else if (pivot_idx > k)
            return quick_select(arr, left, pivot_idx - 1, k);
        return quick_select(arr, pivot_idx + 1, right, k);
    }
    return -1; // error
}

int main()
{
    int arr[N] = {-2, 1, 2, 4, 7, 9, -1, 6, 7, 0};

    for (size_t i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");

    int k = 3;
    printf("%dth smallest element is %d\n", k + 1, quick_select(arr, 0, N, k));
    
    return 0;
}
