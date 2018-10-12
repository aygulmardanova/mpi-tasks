//
// Среднее арифметическое среди положительных чисел массива
//

#include <mpi.h>
#include <time.h>
#include <cstdlib>

#define MASTER 0

int main(int argc, char *argv[]) {
    const int N = 20;
    int rank, size, procN, local_count = 0, count = 0;
    double a[N], *procA, procSum = 0, sum = 0;
    srand(time(0));
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (!rank) {
        for (int i = 0; i < N; i++) {
            a[i] = rand() % 100 - 50;
        }
    }

    int *len = new int[size];
    int *ind = new int[size];
    int rest = N;
    int k = rest / size;
    len[0] = k;
    ind[0] = 0;
    for (int i = 1; i < size; i++) {
        rest -= k;
        k = rest / (size - i);
        len[i] = k;
        ind[i] = ind[i - 1] + len[i - 1];
    }
    procN = len[rank];
    procA = new double[procN];
    MPI_Scatterv(a, len, ind, MPI_DOUBLE, procA, procN, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    delete len, ind;
    for (int i = 0; i < procN; i++)
        if (procA[i] > 0) {
            procSum += procA[i];
            local_count++;
        }

    double *ar = new double[2];
    double *arr = new double[2];

    ar[0] = procSum;
    ar[1] = local_count;

//    MPI_Reduce(&procSum, &sum, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
//    MPI_Reduce(&local_count, &count, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    MPI_Reduce(ar, arr, 2, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);

    delete procA;
    if (!rank) {
        printf("Average of positive numbers = %.2f\n", arr[0] / arr[1]);
    }
    MPI_Finalize();
}