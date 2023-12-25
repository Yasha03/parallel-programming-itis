#include <stdio.h>
#include <mpi.h>
#include <cstdlib>

void pingPongBidirectional(int rank, int messageSize) {
    int *message = new int[messageSize];
    for (int i = 0; i < messageSize; ++i) {
        message[i] = i + 1;
    }
    if (rank == 0) {
        double startTime = MPI_Wtime();
        MPI_Send(message, messageSize, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(message, messageSize, MPI_INT, 1, 1, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        double finishTIme = MPI_Wtime();
        double resultTime = finishTIme - startTime;
        printf("Time for bidirectional ping pong with size %d: %f\n", messageSize, resultTime);
    } else {
        int *received = new int[messageSize];
        MPI_Recv(received, messageSize, MPI_INT, 0, 1, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Send(received, messageSize, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    delete[] message;
}

void pingPingOneDirectional(int rank, int messageSize) {
    int *message = new int[messageSize];
    for (int i = 0; i < messageSize; ++i) {
        message[i] = i + 1;
    }
    double startTime = MPI_Wtime();
    if (rank == 0) {
        MPI_Send(message, messageSize, MPI_INT, 1, 1, MPI_COMM_WORLD);
    } else {
        MPI_Send(message, messageSize, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        int *received = new int[messageSize];
        MPI_Recv(received, messageSize, MPI_INT, 1, 1, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    } else {
        int *message2 = new int[messageSize];
        MPI_Recv(message2, messageSize, MPI_INT, 0, 1, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
    double finishTime = MPI_Wtime();
    double resultTime = finishTime - startTime;
    printf("Time for one directional ping pong with size %d: %f\n", messageSize, resultTime);
    delete[] message;
}

int main(int argc, char **argv) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 2) {
        printf("Should be 2 threads!");
        MPI_Finalize();
        exit(1);
    }
    int messageSize = 5000;

    for (int i = 0; i < 4; messageSize += 5000, i++) {
        pingPingOneDirectional(rank, messageSize);
    }
    messageSize = 5000;
    for (int i = 0; i < 4; messageSize += 5000, i++) {
        pingPongBidirectional(rank, messageSize);
    }

    MPI_Finalize();
    return 0;
}
