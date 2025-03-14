#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define N 500

void generate_array(int* arr, int n)
{
    srand(time(NULL));
    for (size_t i = 0; i < n; i++) arr[i] = rand() % 1000;
}

void insertion_sort(int* arr, int begin, int end)
{
    for (int i = begin + 1; i <= end; i++)
    {
        int k = arr[i];
        int j = i - 1;

        while (j >= begin && arr[j] > k)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = k;
    }
}

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

void heap_sort(int* arr, int begin, int n)
{
    int* subarray = arr + begin;

    for (int i = n / 2 + 1; i >= 0; i--)
        heapify(subarray, n, i);

    for (int i = n - 1; i > 0; i--)
    {
        int temp = subarray[0];
        subarray[0] = subarray[i];
        subarray[i] = temp;
        heapify(subarray, i, 0);
    }
}

int partition(int* arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (size_t j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

void introsort_inner(int* arr, int begin, int end, int max_depth)
{
    int size = end - begin + 1;

    if (size < 16)
    {
        insertion_sort(arr, begin, end);
        return;
    }
    else if (max_depth == 0)
    {
        heap_sort(arr, begin, end);
    }

    int pivot = partition(arr, begin, end);
    introsort_inner(arr, begin, pivot - 1, max_depth - 1);
    introsort_inner(arr, pivot + 1,  end, max_depth - 1);
}

void introsort(int* arr, int n)
{
    int max_depth = (int)log2(n) * 2;
    introsort_inner(arr, 0, n - 1, max_depth);
}

int main()
{
    int arr[N];
    generate_array(arr, N);

    introsort(arr, N);

    for (size_t i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}