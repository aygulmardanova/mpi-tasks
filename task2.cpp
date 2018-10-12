//
// Max вектора
//Сдан
//

#include <mpi.h>
#include <stdio.h>
#include <cstdlib>

#define MASTER 0
#define SIZE 10

int main(int argc, char **argv) {
// Initialize the MPI environment
    int world_rank, world_size, max = 0;
    int *a = new int[SIZE];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Status status;
    int buf_size = SIZE / world_size;
    int *buf = new int[buf_size];
    if (world_rank == MASTER) {
        //array initialising
        for (int i = 0; i < SIZE; ++i) {
            a[i] = rand() % 100;
        }
        //send part of an array to threads
        for (int j = 1; j < world_size; ++j) {
            //sendbuf, count, datatype, dest, tag, comm
            MPI_Send(&a[(j - 1) * buf_size], buf_size, MPI_INT, j, 0, MPI_COMM_WORLD);
        }
        //serach max for master thread
        for (int i = (world_size - 1) * SIZE; i < SIZE; ++i) {
            if (a[i] > max)
                max = a[i];
        }
        //find the real max
        int local_max = max;
        for (int j = 1; j < world_size; ++j) {
            //receive max from each thread and compare local_max with max
            MPI_Recv(&local_max, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            if (local_max > max) {
                max = local_max;
            }
        }
        printf("max = %d\n", max);
    } else {
        //not-master threads search for max
        MPI_Recv(&buf[0], buf_size, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
        for (int i = 0; i < buf_size; i++) {
            if (buf[i] > max) {
                max = buf[i];
            }
        }
        //send max to master thread
        MPI_Send(&max, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
    }
    delete (a, buf);
    MPI_Finalize();
}
