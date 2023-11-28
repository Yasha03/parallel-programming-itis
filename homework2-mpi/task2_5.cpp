#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>

int main(int argc, char** argv) {

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    int count;

    srand(time(nullptr));
    const int N = 10;
    const int elements = 2;

    if (rank == 0) {
        int matrix[N][N];
        int tMatrix[N][N];

        std::cout << "Matrix:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                matrix[i][j] = rand() % 100;
                std::cout << matrix[i][j] << " ";

            }
            std::cout << std::endl;
        }

        int n = 1;
      
        int temp_a[N][elements];
        for (int i = 0; i < N; i += elements, n++) {
            for (int j = 0; j < N; j++) {
                for (int g = 0; g < elements; g++) {
                    temp_a[j][g] = matrix[j][g + elements * (n - 1)];
                }
            }
            MPI_Send(&temp_a[0][0], elements * N, MPI_INT, n, 10, MPI_COMM_WORLD);
        }

        n = 1;
        for (int i = 0; i < N; i += elements, n++) {
            MPI_Recv(&tMatrix[i][0], elements * N, MPI_INT, n, 10, MPI_COMM_WORLD, &status);
        }

        std::cout << "Result:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << tMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else {
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int local_matrix[N][elements];
        int transMatrix[elements][N];

        for (int i = 0; i < elements; i++) {
            for (int j = 0; j < N; j++) {
                transMatrix[i][j] = 0;
            }
        }

        MPI_Recv(&local_matrix[0][0], elements * N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < elements; i++) {
            for (int j = 0; j < N; j++) {
                transMatrix[i][j] = local_matrix[j][i];
            }
        }
        MPI_Send(&transMatrix, elements * N, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}