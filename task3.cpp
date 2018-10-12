//
// Вычисление числа Пи методом Монте-Карло
//

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#define ITERS 100000
#define MASTER 0

int main(int argc, char *argv[]) {
    srand(time(NULL));
    double local_pi, pisum, pi, x_coord, y_coord, r;
    int world_rank, world_size, score = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    for (int i = 1; i <= ITERS; i++) {
        r = rand() % 10000 / (double) 10000;
        x_coord = (2.0 * r) - 1.0;
        r = rand() % 10000 / (double) 10000;
        y_coord = (2.0 * r) - 1.0;
        if ((x_coord * x_coord + y_coord * y_coord) <= 1.0)
            score++;
    }
    local_pi = 4.0 * (double) score / ITERS;
    MPI_Reduce(&local_pi, &pisum, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
    if (world_rank == MASTER) {
        pi = pisum / world_size;
        printf("Monte-Karlo pi = %10.8f\n", pi);
        printf("Real value of PI: 3.14159265 \n");
    }
    MPI_Finalize();
    return 0;
}