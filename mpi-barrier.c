#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void barrier(int rank, int nprocs) {
    int s = 2; // stage
    int i = rank;
    int *g = malloc(32768 * sizeof(int));

    while (s <= nprocs) {
        const int partner = rank ^ (s / 2);
        if (rank < partner) {
            MPI_Send(g, 32768, MPI_INT, partner, 0, MPI_COMM_WORLD);
            MPI_Recv(g, 32768, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(g, 32768, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(g, 32768, MPI_INT, partner, 0, MPI_COMM_WORLD);
        }
        s *= 2;
    }
    printf("Hello world: %d\n", rank);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Starting: %d\n", rank);

    barrier(rank, nprocs);

    MPI_Finalize();
    return 0;

}
