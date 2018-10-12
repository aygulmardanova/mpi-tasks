//
// Умножение матрицы на вектор при разделении данных по столбцам
//

#include <mpi.h>
#include <time.h>
#include <cstdlib>

#define MASTER 0
#define N 5
#define M 5

int main(int argc, char *argv[]) {
    int rank, size, local_n;
    int *local_A, *local_V, sum = 0;
    srand(time(0));
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *a, *v, *arr;
    if (!rank) {        //r==0
        a = new int[N * M];     //матрица
        arr = new int[M * N];     //result vector
        v = new int[M];     //vector
        printf("Matrix a:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; ++j) {
                a[i * M + j] = rand() % 10;
                printf("%d\t", a[i * M + j]);
            }
            printf("\n");
        }

        //reverse matrix
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                arr[i * N + j] = a[j * M + i];
            }
        }

        //vector
        printf("Vector v:\n");
        for (int i = 0; i < M; i++) {
            v[i] = rand() % 10;
            printf("%d\t", v[i]);
        }
        printf("\n");
    }
    int *len = new int[size];       //массив длин блоков для matrix
    int *ind = new int[size];       //массив индексов блоков для matrix
    int *len1 = new int[size];      //массив длин блоков для vector
    int *ind1 = new int[size];      //массив индексов блоков для vector
    int rest = M;       //число столбцов
    int k = rest / size;        //сколько столбцов каждому потоку
    len[0] = k * N;     //сколько элементов матрицы 0 потоку
    ind[0] = 0;
    ind1[0] = 0;
    len1[0] = k;
    for (int i = 1; i < size; i++) {
        rest -= k;
        k = rest / (size - i);
        len[i] = k * N;
        len1[i] = k;
        ind[i] = ind[i - 1] + len[i - 1];
        ind1[i] = len1[i - 1] + ind1[i - 1];
    }
    local_n = len[rank];        //длина массива для текущего потока
    local_A = new int[local_n];     //массив длины для текущего потока для получения элементов массива
    local_V = new int[local_n / N];     //массив для получения элементов вектора
    MPI_Scatterv(arr, len, ind, MPI_INT, local_A, local_n, MPI_INT, MASTER, MPI_COMM_WORLD);      //для матрицы
    MPI_Scatterv(v, len1, ind1, MPI_INT, local_V, local_n / N, MPI_INT, MASTER, MPI_COMM_WORLD);       //для вектора
    delete ind1, len1;

    int buf = 0;
    //mult and sum elements
    for (int i = 0; i < local_n / N; i++) {
        for (int j = 0; j < N; j++) {
            buf = local_A[i * N + j] * local_V[i];
            local_A[j] += buf;
            local_A[i * N + j] = 0;
        }
    }
    int *result = new int[N];       //result vector
    MPI_Reduce(local_A, result, N, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);
    delete ind, len, local_A, local_V;
    if (!rank) {
        delete a, v, arr;
        printf("result: \n");
        for (int i = 0; i < N; i++) {
            printf("%d\t", result[i]);
        }
        printf("\n");
    }
    delete result;
    MPI_Finalize();
}