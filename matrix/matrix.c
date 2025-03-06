#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define mat_at(mat, i, j) ((mat)->items[j * (mat)->cols + i])
#define mat_for(mat, ...) \
    for (size_t j = 0; j < (mat)->rows; j++) {\
        for (size_t i = 0; i < (mat)->cols; i++) {\
            __VA_ARGS__;\
        }\
    }

typedef struct matrix {
    int* items;
    size_t rows;
    size_t cols;
} matrix_t;

matrix_t* mat_alloc(size_t rows, size_t cols)
{
    matrix_t* mat = malloc(sizeof(matrix_t));
    mat->rows = rows;
    mat->cols = cols;
    mat->items = calloc(mat->rows * mat->cols, sizeof(int));
    return mat;
}

matrix_t* mat_from_array(int* arr, int rows, int cols)
{
    matrix_t* mat = mat_alloc(rows, cols);
    memcpy(mat->items, arr, rows * cols * sizeof(int));
    return mat;
}

void mat_free(matrix_t* mat)
{
    free(mat->items);
    free(mat);
}

void mat_print(matrix_t* mat)
{
    mat_for(mat,
        printf("%d ", mat_at(mat, i, j));
        if (i == mat->cols - 1) printf("\n");
    );
}

bool is_square(matrix_t* mat)
{
    return mat->rows == mat->cols;
}

void mat_scalar(matrix_t* mat, int k)
{
    mat_for(mat,
        mat_at(mat, i, j) *= k;
    );
}

matrix_t* mat_mul(matrix_t* A, matrix_t* B)
{
    if (A->cols != B->rows)
    {
        fprintf(stderr, "Multiplication: Wrong dimensions\n");
        exit(1);
    }
    matrix_t* res = mat_alloc(A->rows, B->cols);
    mat_for(res,
            mat_at(res, i, j) = 0;
            for (size_t k = 0; k < A->cols; k++)
                mat_at(res, i, j) += mat_at(A, k, j) * mat_at(B, i, k);
    );
    return res;
}

matrix_t* mat_add(matrix_t* A, matrix_t* B)
{
    if (A->cols != B->cols || A->rows != B->rows)
    {
        fprintf(stderr, "Addition: Wrong dimensions\n");
        exit(1);
    }

    matrix_t* res = mat_alloc(A->rows, A->cols);
    mat_for(res,
        mat_at(res, i, j) = mat_at(A, i, j) + mat_at(B, i, j);
    );
    return res;
}

int main()
{
    int arr1[] = {
        2, 9,
        5, 0,
        -1, 3
    };
    int arr2[] = {
        6, -6,
        3, 0
    };
    matrix_t* A = mat_from_array(arr1, 3, 2);
    matrix_t* B = mat_from_array(arr2, 2, 2);

    printf("Matrix A:\n");
    mat_print(A);
    printf("Matrix B (is square: %d):\n", is_square(B));
    mat_print(B);

    printf("-----------\n");
    printf("A x B:\n");
    matrix_t* mul = mat_mul(A, B);
    mat_print(mul);

    printf("-----------\n");
    printf("A + A:\n");
    matrix_t* add = mat_add(A, A);
    mat_print(add);

    printf("-----------\n");
    printf("2A:\n");
    mat_scalar(A, 2);
    mat_print(A);

    mat_free(A);
    mat_free(B);
    mat_free(mul);
    mat_free(add);

    return 0;
}