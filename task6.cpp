//
// Maxmin матрицы.
// Для каждой строчки массива найти минимум, среди минимумов найти максимум.
//Сдан
//

#include <mpi.h>
#include <time.h>
#include <cstdlib>

#define MASTER 0
#define N 20        //строк
#define M 10        //столбцов

int main(int argc, char *argv[]) {
    int rank, size, local_n;
    int *local_A, local_max = 0, local_min = 0, max = 0;
    srand(time(0));
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *a = new int[N * M];
    if (!rank) {            //rank==0
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; ++j) {
                a[i * M + j] = rand() % 10;
//                printf("%d\t", a[i*M + j]);
            }
//            printf("\n");
        }
    }
    int *len = new int[size];       //массив блоков (размер блоков)
    int *ind = new int[size];       //массив индексов блоков (начало каждого блока)
    int rest = N;
    int k = rest / size;        //сколько будем передавать элементов
    len[0] = k * M;
    ind[0] = 0;
    for (int i = 1; i < size; i++) {
        rest -= k;      //осталось распределить rest-k элементов
        k = rest / (size - i);
        len[i] = k * M;
        ind[i] = ind[i - 1] + len[i - 1];
    }
    local_n = len[rank];        //блок, соответствующий процессу (элемент массива блоков)
    local_A = new int[local_n];     //массив размера блока
    //что отправляем, массив блоков, массив начала блоков для каждого процесса, куда записывать, сколько принимать
    MPI_Scatterv(a, len, ind, MPI_INT, local_A, local_n, MPI_INT, MASTER, MPI_COMM_WORLD);
    delete len, ind;
    local_max = local_A[0];
    for (int i = 0; i < local_n / M; i++) {
        local_min = local_A[i * M];
        for (int j = 0; j < M; j++) {
            if (local_min > local_A[M * i + j])//если локальный минимум меньше чем остальные элементы строчки {от M*i до M*(i+1)}
                local_min = local_A[M * i + j];
        }
        if (local_min > local_max || i == 0) {//если минимум строчки больше максимума
            local_max = local_min;
        }
    }
    //max - куда записываем максимум, поиск максимума по всем local_max
    MPI_Reduce(&local_max, &max, 1, MPI_INT, MPI_MAX, MASTER, MPI_COMM_WORLD);
    delete local_A, a;
    if (!rank)      //rank==0
        printf("\nmax of min = %d\n", max);
    MPI_Finalize();
}