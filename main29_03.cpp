#include <iostream>
#include <mpi.h>
#include <time.h>


using namespace std;

int main(int argc, char *argv[]) {

    const int N = 20;
    int rank, size, procN;
    double a[N], *procA, sum = 0, sum1 = 0, procSum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (!rank) {
        for (int i = 0; i < N; i++) {
            a[i] = i;
            sum1+=a[i];
        }
        printf("\n");
    }


    procN = N / size; //размер интервала
    procA = new double[procN]; //кусок от массива, который получит каждый процесс
    MPI_Scatter(a, procN, MPI_DOUBLE, procA, procN, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //Scatterv - various length. Написать, сколько раздавать каждому процессу
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
        ind[i] = ind[i-1] + len[i-1];
    }

    procN = len[rank];
    procA = new double[procN];
    MPI_Scatterv(a, len, ind, MPI_DOUBLE, procA, procN, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    delete len;
    delete ind;


    for (int i = 0; i < procN; i++) {
        procSum += procA[i];
    }

    delete procA;

    if (!rank) {
        printf("sum=%f,sum1=%f", sum, sum1);
    }

//    MPI_Allgather();

    //результат собирается у всех
//    MPI_Allreduce(&procSum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    for (int i = 0; i < procN; i++) {
        printf("%.2f ", procA[i]);
    }

    printf(" - %d \n", rank);

    MPI_Finalize();
    return 0;

}