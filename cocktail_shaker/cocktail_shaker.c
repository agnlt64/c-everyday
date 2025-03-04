#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define N 40 // test array size

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

void cocktail_shaker(int* arr, int n)
{
    bool swapped = true;
    int begin = 0;
    int end = n - 1;

    while (swapped)
    {
        swapped = false;

        for (size_t i = begin; i < end; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                swap(&arr[i], &arr[i + 1]);
                swapped = true;
            }
        }
        end--;

        for (size_t i = end; i > begin; i--)
        {
            if (arr[i - 1] > arr[i])
            {
                swap(&arr[i], &arr[i - 1]);
                swapped = true;
            }
        }
        begin++;
    }
}

int main()
{
    int arr[N];
    generate_array(arr, N);

    cocktail_shaker(arr, N);
    for (size_t i = 0; i < N; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}