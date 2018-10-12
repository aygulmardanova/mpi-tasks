//
// MPI
//mpicc - компиляция
//mpirun -np 4 - запуск, 4 процесса
//mpi работает с памятью, которая своя для каждого процесса
//процессы объединены в группы, которые

#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int numtasks, rank, dest, src, rc, tag=1;
    char inmsg, outmsg='x';
    MPI_Status Stat;
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        dest = 1;
        src = 1;
        rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        rc = MPI_Recv(&inmsg, 1, MPI_CHAR, src, tag, MPI_COMM_WORLD, &Stat);
    }
    else if (rank == 1) {
        dest = 0;
        src = 0;
        rc = MPI_Recv(&inmsg, 1, MPI_CHAR, src, tag, MPI_COMM_WORLD, &Stat);
        rc = MPI_Send(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}


