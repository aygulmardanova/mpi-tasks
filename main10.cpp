//
// Created by Aygul Mardanova on 24.04.17.
// Время передачи для разных Send-oв
//

#include <mpi.h>
#include <time.h>
#include <cstdlib>

#define MASTER 0
#define N 10000

int main(int argc, char *argv[]) {

    int x[N];
    double start, end;
    MPI_Status status;
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL));
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            x[i] = rand() % 100;
        }
    }

    //Send - стандартный
    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Send(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(x, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == 0) {
        end = MPI_Wtime();
        printf("Send time: %.5f\n", end - start);
    }

    //Ssend - синхронный
    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Ssend(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(x, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == 0) {
        end = MPI_Wtime();
        printf("Ssend time: %.5f\n", end - start);
    }

    //Rsend - по готовности
    if (rank == 0) {
        start = MPI_Wtime();

        MPI_Rsend(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(x, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == 0) {
        end = MPI_Wtime();
        printf("Rsend time: %.5f\n", end - start);
    }

    //Bsend - буферизованный
    if (rank == 0) {

        MPI_Buffer_attach(x, N * 6 + MPI_BSEND_OVERHEAD);
        start = MPI_Wtime();
        MPI_Bsend(x, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(x, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    if (rank == 0) {
        end = MPI_Wtime();
        printf("Bsend time: %.5f\n", end - start);
    }

    MPI_Finalize();
    return 0;
}