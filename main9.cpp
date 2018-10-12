//
// Created by Aygul Mardanova on 24.04.17.
// Инвертировать массив
// contains errors (undeclared variables and methods)
//

#include <mpi.h>
#include <time.h>
#include <cstdlib>

#define MASTER 0
#define N 10

int main(int argc, char *argv[]) {

    int n, size, i, dest, rank;
    int a[N], arr[N];
    MPI_Status status;
    //init
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


//    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        //initialisation
        for (int i = 0; i < N; i++) {
            a[i] = i;
            printf("%d ", a[i]);
        }
        printf("\n");

        int k, mid;

        k = N / size;
        mid = N % size;

        int *len = new int[size];//массив блоков
        int *ind = new int[size];//массив индексов блоков
        int rest, k;

        rest = N;//сколько осталось элементов
        k = rest / size;//
        len[0] = k;
        ind[0] = 0;

        for (int i = 1; i < size; i++) {
            rest -= k;//осталось распределить rest-k (N-k) элементов
            k = rest / (size - i);
            len[i] = k;
            ind[i] = ind[i - 1] + len[i - 1];
            if (i > size / 2) {
                ind[i] += mid;      //смещение индексов (средние элементы остаются у 0 потока)
            }
        }

        procN = len[rank];
        procA = new double[procN];
        MPI_Scatterv(a, len, ind, MPI_DOUBLE, procA, procN, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    } else {

//        MPI_Recv(a, n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);


    }

    int i1 = size / 2 + 1;

//    int temp;
//    for (i = 0; i < n / 2; i++) {
//        temp = a[n - i - 1];
//        a[n - i - 1] = a[i];
//        a[i] = temp;
//    }

    MPI_Gatherv(a, 1, MPI_INT, arr, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gatherv(a, N, MPI_INT, arr, N, MPI_INT, 0, MPI_COMM_WORLD);

    printf("\n");
    if (rank == 0) {
        for (int i = 0; i < 10; i++) {
            printf("a[%d] = %d \n", i, a[i]);
        }

    }

    MPI_Finalize();

    return 0;
}