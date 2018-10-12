//
// Scatter и Gather через Send и Recv
//

#include <mpi.h>
#include <time.h>
#include <cstdlib>

#define MASTER 0
#define N 10

int scatter(int *send_data, int send_count, MPI_Datatype send_type, int *recv_data, int recv_count,
            MPI_Datatype recv_type, int root, MPI_Comm communicator) {
    int rank;
    int size;
    MPI_Status status;
    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root)
                MPI_Send(&send_data[i * send_count], send_count, send_type, i, 0, communicator);
            else {
                for (int j = 0; j < recv_count; j++) {
                    recv_data[j] = send_data[i * send_count + j];
                }
            }
        }
    } else {
        MPI_Recv(&recv_data[0], recv_count, recv_type, root, 0, communicator, &status);
    }
    return 0;
}

int gather(int *sendbuf, int send_count, MPI_Datatype send_type, int *recv_buf, int recv_count, MPI_Datatype recv_type,
           int root, MPI_Comm comm) {
    int rank;
    int size;
    MPI_Status status;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Recv(&recv_buf[i * recv_count], recv_count, recv_type, i, 0, comm, &status);
            } else {
                for (int j = 0; j < recv_count; j++) {
                    recv_buf[i * recv_count + j] = sendbuf[j];      //copy array
                }
            }
        }
    } else {
        MPI_Send(&sendbuf[0], send_count, send_type, root, 0, comm);
    }
    return 0;
}

int main(int argc, char *argv[]) {

    int rank, size;
    srand(time(0));
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *a = new int[N];
    //initialise
    if (!rank) {
    printf("Array:\n");
        for (int i = 0; i < N; i++) {
            a[i] = rand() % 10;
            printf("%d\t", a[i]);
        }
        printf("\n");
    }

    int *local_A = new int[N / size];       //part of array
    int *b = new int[N];        //array for gather
    scatter(a, N / size, MPI_INT, local_A, N / size, MPI_INT, MASTER, MPI_COMM_WORLD);
    gather(local_A, N / size, MPI_INT, b, N / size, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER) {
    printf("Gathered array:\n");
        for (int j = 0; j < N; j++) {
            printf("%d\t", b[j]);
        }
        printf("\n");
    }

    delete a, b, local_A;
    MPI_Finalize();
}