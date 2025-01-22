#include <stdio.h>
#include <stdlib.h>

void save_to_file(int n, double **A, int format_flag)
{
    char filename[50];
    sprintf(filename, "array_%06d%s", n, (format_flag == 0 ? "_asc.out" : "_bin.out"));

    if (format_flag == 0)
    {
        FILE *file = fopen(filename, "w");
        if (!file)
        {
            fprintf(stderr, "Error: Could not open file %s\n", filename);
            return;
        }

        int i, j;
        for (i = 0; i < n; ++i)
        {
            for (j = 0; j < n; ++j)
            {
                fprintf(file, "%.15f ", A[i][j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
        printf("File %s written successfully in ASCII format.\n", filename);
    }
    else if (format_flag == 1)
    {
        FILE *file = fopen(filename, "wb");
        if (!file)
        {
            fprintf(stderr, "Error: Could not open file %s\n", filename);
            return;
        }
        int i;
        for (i = 0; i < n; ++i)
        {
            fwrite(A[i], sizeof(double), n, file);
        }

        fclose(file);
        printf("File %s written successfully in binary format.\n", filename);
    }
    else
    {
        fprintf(stderr, "Invalid format flag! Use 0 for ASCII, 1 for binary.\n");
    }
}

int main()
{
    FILE *input_file = fopen("input.in", "r");
    if (!input_file)
    {
        fprintf(stderr, "Error: Could not open input file.\n");
        return 1;
    }

    int n;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

    double **A = (double **)malloc(n * sizeof(double *));
    int i, j;
    for (i = 0; i < n; ++i)
    {
        A[i] = (double *)malloc(n * sizeof(double));
        for (j = 0; j < n; ++j)
        {
            A[i][j] = i + j;
        }
    }

    save_to_file(n, A, 0);
    save_to_file(n, A, 1);

    for (i = 0; i < n; ++i)
    {
        free(A[i]);
    }
    free(A);

    return 0;
}
