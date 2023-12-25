#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int countElements = 12;
    int array[countElements];
    const int blockSize = countElements / size + 1;
    if (rank == 0) {
        for (int i = 0; i < countElements; ++i) {
            array[i] = i + 1;
        }
        for (int i = 1; i < size; ++i) {
            int countForSend;
            if (i < countElements % size) {
                countForSend = blockSize;
            } else {
                countForSend = blockSize - 1;
            }
            MPI_Send(array + i * (blockSize - 1), countForSend, MPI_INT, i, 0,
                     MPI_COMM_WORLD);
        }
    } else {
        int countReceive;
        if (rank < countElements % size) {
            countReceive = blockSize;
        } else {
            countReceive = blockSize - 1;
        }
        MPI_Recv(array, countReceive, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    printf("Thread %d: ", rank);
    int count;
    if (rank < countElements % size) {
        count = blockSize;
    } else {
        count = blockSize - 1;
    }
    for (int i = 0; i < count; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}