#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 4

int knapsack_rec(int* val, int* weights, int size, int capacity, int level)
{
    if (level == 0 || capacity == 0)
        return 0;

    int pick = 0;

    if (weights[level - 1] <= capacity)
        pick = val[level - 1] + knapsack_rec(val, weights, size, capacity - weights[level - 1], level - 1);

    int no_pick = knapsack_rec(val, weights, size, capacity, level - 1);
    return fmax(pick, no_pick);
}

int knapsack_bot_up(int* val, int* weights, int size, int capacity)
{
    int* dp = calloc((capacity + 1), sizeof(int));

    for (size_t i = 1; i <= size; i++)
    {
        for (int j = capacity; j >= weights[i - 1]; j--)
        {
            dp[j] = fmax(dp[j], dp[j - weights[i - 1]] + val[i - 1]);
        }
    }
    int res = dp[capacity];
    free(dp);
    return res;
}

int main()
{
    int val[SIZE] = {1, 2, 3, 4};
    int weights[SIZE] = {4, 5, 1, 1};
    int capacity = 4;

    printf("%d\n", knapsack_bot_up(val, weights, SIZE, capacity));
    printf("%d\n", knapsack_rec(val, weights, SIZE, capacity, SIZE));

    return 0;
}