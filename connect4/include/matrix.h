#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

// Allocates a new matrix_t
matrix_t* mat_alloc(size_t rows, size_t cols);

// Allocates a new matrix from a one-dimensional C array
matrix_t* mat_from_array(int* arr, int rows, int cols);

// Frees a matrix_t
void mat_free(matrix_t* mat);

// Prints the content of the matrix
void mat_print(matrix_t* mat);

// Returns true if mat is square, false otherwise
bool is_square(matrix_t* mat);

// Realises in-place multiplication of all
// the coefficients of mat by k
void mat_scalar(matrix_t* mat, int k);

// Computes the product of A and B in a new matrix
matrix_t* mat_mul(matrix_t* A, matrix_t* B);

// Computes the sum of A and B in a new matrix
matrix_t* mat_add(matrix_t* A, matrix_t* B);

#ifdef MATRIX_IMPLEMENTATION
matrix_t* mat_alloc(size_t rows, size_t cols)
{
    matrix_t* mat = malloc(sizeof(matrix_t));
    mat->rows = rows;
    mat->cols = cols;
    mat->items = calloc(mat->cols * mat->rows, sizeof(int));
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
#endif