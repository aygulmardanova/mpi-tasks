//
// Created by Aygul Mardanova on 19.04.17.
//

#include <mpi.h>
#include <cstdlib>

int main() {
    int rank, size, r1, s1, r2, s2;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm newComm;

    //color - деление на 3, номер - остаток от деления на 3
    MPI_Comm_split(MPI_COMM_WORLD, rank / 3, rank % 3, &newComm);

    /*  в обратном порядке
      MPI_Comm_split(MPI_COMM_WORLD, rank % 3, rank / 3, &newComm);*/
    MPI_Comm_rank(newComm, &r1);
    MPI_Comm_size(newComm, &s1);

    printf("worldrank-%d   size-%d   newrank-%d\n", rank, s1, r1);


    //работа с группами
    MPI_Group gr1, gr2, grWorld;
    int ranks[3] = {2, 3, 4};

    MPI_Comm_group(MPI_COMM_WORLD, &grWorld);
    //из какой группы, сколько надо убрать из группы элементов, передать список ранков.
    //Из группы grWorld будут убраны элементы 2,3,4 и перемещны в группу gr1
    MPI_Group_excl(grWorld, 3, ranks, &gr1);
//    MPI_Group_incl(grWorld, 3, ranks, &gr1);
    MPI_Comm_create(MPI_COMM_WORLD, gr1, &newComm);

    if (rank < 2 || rank > 4) {
        MPI_Comm_rank(MPI_COMM_WORLD, &r2);
        MPI_Comm_size(MPI_COMM_WORLD, &s2);

        printf("group - worldrank-%d   size-%d   newrank-%d\n", rank, s2, r2);

    }

    MPI_Finalize();
    return 0;
}