#include <stdio.h>

#define N 15

int interpolation_search_1(int* arr, int key, int low, int high)
{
    high -= 1;
    int middle;
    int initial_low = low;

    while ((arr[low] < arr[high]) && (arr[low] <= key) && (key <= arr[high]))
    {
        middle = low + ((key - arr[low]) * (high - low)) / (arr[high] - arr[low]);

        if (arr[middle] < key)
            low = middle + 1;
        else if (key < arr[middle])
            high = middle - 1;
        else
            return middle;
    }
    if (key == arr[low])
        return low;
    else
        return initial_low - 1;
}

int interpolation_search_2(int* arr, int key, int low, int high)
{
    high -= 1;
    int middle;

    if (key < arr[low])
        return low - 1;

    if (arr[high] <= key)
        return high;

    while ((arr[low] <= key) && (key < arr[high]))
    {
        middle = low + ((high - low) * (key - arr[low])) / (arr[high] - arr[low]);

        if (key < arr[middle])
            high = middle;
        else
            low = middle + 1;
    }
    return low - 1;
}

int main()
{
    int arr[N] = {2, 4, 8, 12, 12, 18, 20, 45, 52, 74, 84, 99, 101, 123, 142};
    int key = 45;

    int i = interpolation_search_1(arr, key, 0, N);
    printf("version 1 - key %d found at index %d\n", key, i);

    i = interpolation_search_2(arr, key, 0, N);
    printf("version 2 - key %d found at index %d\n", key, i);

    return 0;
}
