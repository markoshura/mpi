#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>

int main (int argc, char* argv[])
{
// Умножение матрицы на вектор
    int procs_rank, procs_count;
    int i, j, n = 3, local_n;
    int *local_a, *b, *local_c, *a, *c;
    srand(time(0));
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &procs_rank);
    local_n = n / procs_count;
    local_a = (int *) malloc((local_n * n) * sizeof(int));
    b = (int *) malloc(n * sizeof(int));
    local_c = (int *) malloc(local_n * sizeof(int));
    c = (int *) malloc(n * sizeof(int));
    a = (int *) malloc((n * n) * sizeof(int));

//инициализируем матрицу в нулевом процессе
// и рассылаем её части по процессам
    if (procs_rank == 0)
    {
//матрицу удобнее рассматривать как одномерный массив но с индексами i*n+j
        printf("INITIAL MATRIX\n");
        for (i=0; i<n; i++)
            for (j=0; j<n; j++){
                a[i*n+j]=rand()%2+1;
                printf("%d\n",a[i*n+j]);}

        printf("INITIAL COLUMN\n");
        for (i=0; i<n; i++){
            b[i] = rand()%2+1;//генерим рандомный вектор
            printf("%d\n", b[i]);}}
//рассылаем вектор b
    MPI_Bcast(b, n, MPI_INT, 0, MPI_COMM_WORLD);
//разделяем матрицу на горизонтальные полосы
//шириной local_a и отпраляем процессам
    MPI_Scatter(a, n * local_n, MPI_INT, local_a, n * local_n, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < local_n; i++)
        for (j = 0; j < n; j++)
            local_c[i] += local_a[i*n+j] * b[j];
//собираем результат в нулевом процессе
    MPI_Gather(local_c, local_n, MPI_INT, c, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    if (procs_rank == 0)
    { printf ("FINAL COLUMN\n");
        for (i=0; i<n; i++)
            printf("%d\n",c[i]);
    }
    MPI_Finalize();
    return 0;
}
