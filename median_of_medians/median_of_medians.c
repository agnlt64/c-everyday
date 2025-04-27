#include <stdio.h>
#include <stdlib.h>

#define GROUP_SIZE 5
#define N 10

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

int find_median(int* arr, int len, int left, int right)
{
    bubble_sort(arr, len);
    int n = right - left + 1;
    return arr[left + n / 2];
}

int partition(int* arr, int left, int right, int  pivot)
{
    int pivot_idx;
    for (pivot_idx = left; pivot_idx <= right; pivot_idx++)
    {
        if (arr[pivot_idx] == pivot)
            break;
    }
    swap(&arr[pivot_idx], &arr[right]);

    int store_idx = left;
    for (size_t i = left; i < right; i++)
    {
        if (arr[i] < pivot)
        {
            swap(&arr[store_idx], &arr[i]);
            store_idx++;
        }
    }
    swap(&arr[store_idx], &arr[right]);
    return store_idx;
}

int select_kth(int* arr, int len, int left, int right, int k)
{
    if (left == right)
        return arr[left];

    int n = right - left + 1;
    int i, med_count = (n + GROUP_SIZE - 1) / GROUP_SIZE;
    int* meds = malloc(med_count * sizeof(int));

    for (i = 0; i < med_count; i++)
    {
        int group_left = left + i * GROUP_SIZE;
        int group_right = group_left + GROUP_SIZE - 1;

        if (group_right > right)
            group_right = right;
        
        meds[i] = find_median(arr, group_left, group_right, len);
    }

    int med_of_meds;
    if (med_count == 1)
        med_of_meds = meds[0];
    else
        med_of_meds = select_kth(meds, len, 0, med_count - 1, med_count / 2);
    free(meds);

    int pivot_idx = partition(arr, left, right, med_of_meds);

    int rank = pivot_idx - left + 1;
    if (k == rank)
        return arr[pivot_idx];
    else if (k < rank)
        return select_kth(arr, len, left, pivot_idx - 1, k);
    else
        return select_kth(arr, len, pivot_idx + 1, right, k - rank);
}

void print_arr(int* arr, int len)
{
    for (size_t i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    int arr[N] = {12, 3, 5, 7, 4, 19, 26, 23, 2, 8};
    int k = 5;

    printf("original array: \n");
    print_arr(arr, N);

    int kth = select_kth(arr, N, 0, N - 1, k);
    printf("%dth smallest element is %d\n", k, kth);

    return 0;
}