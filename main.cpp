#include <iostream>
#include <mpi.h>

using namespace std;

//first task
int main() {

    int size, rank, recv;
    MPI_Status st;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (!rank) { //rank == 0

//        printf("Hello world! from process %d of %d\n", rank, size);

        for (int i = 1; i < size; i++) {
            MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st); //в порядке поступления
//            MPI_Recv(&recv, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st); //в порядке
//            MPI_Recv(&recv, 1, MPI_INT, size - i, MPI_ANY_TAG, MPI_COMM_WORLD, &st); //в обратном порядке
            printf("Hello from process %d out of %d\n", recv, size);

        }

    } else {
        //buf - ссылка на начало ячейки
        //count - количество передаваемых элементов (1)
        //datatype - MPI тип данных (MPI_INT / MPI_INT_DOUBLE = int + double)
        //dest - rank кому посылаем
        //tag - метка (integer) для сообщения, чтобы различать однотипные сообщения (от одного отправителя)
        //comm
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); //tag одинаковый, тк разные отправители
    }


    MPI_Finalize();

    return 0;
}