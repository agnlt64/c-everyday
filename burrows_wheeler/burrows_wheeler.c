#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rotation {
    int index;
    char* suffix;
} rotation_t;

int cmpfunc(const void* x, const void* y)
{
    rotation_t* rx = (rotation_t*)x;
    rotation_t* ry = (rotation_t*)y;
    return strcmp(rx->suffix, ry->suffix);
}

void compute_suffix_array(char* input, int len, int* suffix_arr)
{
    rotation_t suff[len];

    for (size_t i = 0; i < len; i++)
    {
        suff[i].index = i;
        suff[i].suffix = input + i;
    }

    qsort(suff, len, sizeof(rotation_t), cmpfunc);

    for (size_t i = 0; i < len; i++)
        suffix_arr[i] = suff[i].index;
}

void find_last_char(char* input, int* suffix_arr, int n, char* bwt_arr)
{
    int i;
    for (i = 0; i < n; i++)
    {
        int j = suffix_arr[i] - 1;
        if (j < 0)
            j += n;

        bwt_arr[i] = input[j];
    }
    bwt_arr[i] = '\0';
}

int main()
{
    char* input = "banana$";
    int len = strlen(input);

    int* suffix_arr = malloc(len * sizeof(int));
    compute_suffix_array(input, len, suffix_arr);

    char* bwt_arr = malloc(len);
    find_last_char(input, suffix_arr, len, bwt_arr);

    printf("input text: %s\n", input);
    printf("transformed text: %s\n", bwt_arr);

    free(suffix_arr);
    free(bwt_arr);

    return 0;
}