#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 500

void heapify(int* arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i)
    {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

void heap_sort(int* arr, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--)
    {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }
}

int main()
{
    int arr[N];
    srand(time(NULL));
    for (size_t i = 0; i < N; i++) arr[i] = rand() % 1000;

    heap_sort(arr, N);
    for (size_t i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\n");
    
    return 0;
}