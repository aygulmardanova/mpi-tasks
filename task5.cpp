//
// Скалярное произведение
//Сдан
//

#include <mpi.h>
#include <time.h>
#include <cstdlib>

#define MASTER 0

int main(int argc, char *argv[]) {
    const int N = 20;
    int rank, size, local_n;
    double a[N], b[N], *local_A, *local_B, local_sum = 0, sum = 0;
    srand(time(0));
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (!rank) {        //r==0
        for (int i = 0; i < N; i++) {
            a[i] = rand() % 10;
            b[i] = rand() % 10;
        }
    }
    int *len = new int[size];       //массив блоков
    int *ind = new int[size];       //массив индексов блоков
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
    local_n = len[rank];
    local_A = new double[local_n];
    local_B = new double[local_n];
    //sendbuf (что отправляем), sendcounts (число отправл элементов), displs (массив смещений, начало блока для каждого процесса); receive params
    MPI_Scatterv(a, len, ind, MPI_DOUBLE, local_A, local_n, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Scatterv(b, len, ind, MPI_DOUBLE, local_B, local_n, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    delete len, ind;
    for (int i = 0; i < local_n; i++)
        local_sum += local_A[i] * local_B[i];
    //sendbuf, recvbuf,count, type, operation
    MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
    delete local_A, local_B;
    if (!rank)
        printf("Scalar multiplication = %.2f\n", sum);
    MPI_Finalize();
}