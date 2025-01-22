#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define eps 1e-8


void read_matrix(int row, int col, char *fileName, double **mat)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        fprintf(stderr, "Error opening file %s\n", fileName);
        exit(1);
    }
    int i, j;
    for (i = 0; i < row; ++i)
    {
        for (j = 0; j < col; ++j)
        {
            if (fscanf(file, "%lf,", &mat[i][j]) != 1)
            {
                fprintf(stderr, "Error reading file %s at row %d, column %d\n", fileName, i, j);
                fclose(file);
                exit(1);
            }
        }
    }

    fclose(file);
}

int read_vector(int size, char *fileName, double *vec)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        return 0;
    }
    int i;
    for (i = 0; i < size; ++i)
    {
        if (fscanf(file, "%lf,", &vec[i]) != 1)
        {
            fprintf(stderr, "Error reading file %s at row %d\n", fileName, i);
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

void mat_vec_multiply(int row, int col, double *x, double *y, double **A)
{
    int i, j;
    for (i = 0; i < row; ++i)
    {
        double sum = 0.0;
        for (j = 0; j < col; ++j)
        {
            sum += A[i][j] * x[j];
        }
        y[i] = sum;
    }
}

double dot_product(int size, double *a, double *b)
{
    double sum = 0.0;
    int i;
    for (i = 0; i < size; ++i)
    {
        sum += a[i] * b[i];
    }
    return sum;
}

int isEigenVector(int row, int col, int vecSize, double *eigVal, double *x, double **A)
{
    if (col != vecSize)
        return 0;

    double *y = (double*)malloc(row * sizeof(double));
    mat_vec_multiply(row, col, x, y, A);

    double lambda = dot_product(vecSize, y, x) / dot_product(vecSize, x, x);
    int i;
    for (i = 0; i < vecSize; ++i)
    {
        if (fabs(y[i] - lambda * x[i]) > eps)
        {
            free(y);
            return 0;
        }
    }

    *eigVal = lambda;
    free(y);
    return 1;
}

int main()
{
    int n, vecnum;
    double **A;
    double *x;
    double eigVal;
    char matFile[100], vecFile[100];
    FILE *inp = fopen("input.in", "r");

    if (!inp)
    {
        fprintf(stderr, "Error opening input file.\n");
        return 1;
    }

    fscanf(inp, "%d", &n);
    fclose(inp);

    A = (double **)malloc(n * sizeof(double *));
    int i;
    for (i = 0; i < n; ++i)
    {
        A[i] = (double *)malloc(n * sizeof(double));
    }
    x = (double *)malloc(n * sizeof(double));

    sprintf(matFile, "inputfiles/mat_%06d.in", n);
    read_matrix(n, n, matFile, A);

    for (vecnum = 1;; ++vecnum)
    {
        sprintf(vecFile, "inputfiles/vec_%06d_%06d.in", n, vecnum);
        if (!read_vector(n, vecFile, x))
        {
            break;
        }

        if (isEigenVector(n, n, n, &eigVal, x, A))
        {
            printf("%s : Yes : %lf\n", vecFile, eigVal);
        }
        else
        {
            printf("%s : Not an eigenvector\n", vecFile);
        }

        printf("\n");
    }

    for (i = 0; i < n; ++i)
    {
        free(A[i]);
    }
    free(A);
    free(x);

    return 0;
}
