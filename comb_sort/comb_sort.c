#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

void comb_sort(int* arr, int n)
{
    int gap = n;
    float shrink = 1.3;
    bool sorted = false;

    while (!sorted)
    {
        gap = (int)(gap / shrink);
        if (gap < 1)
            break;
        else if (gap == 9 || gap == 10)
            gap = 11;

        size_t i = 0;
        while (i + gap < n)
        {
            if (arr[i] > arr[i + gap])
            {
                swap(&arr[i], &arr[i + gap]);
                sorted = false;
            }
            i++;
        }
    }
}

int main()
{
    int arr[N];
    generate_array(arr, N);

    comb_sort(arr, N);

    for (size_t i = 0; i < N; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}