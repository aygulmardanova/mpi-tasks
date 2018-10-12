#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int me, size;
    int SOME_TAG = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if ((me % 2) == 0) {
        if ((me+1) < size) {
            MPI_Send();
        }
    } else {

    }

}



