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
        int matrix1[N][N];
        int matrix2[N][N];
        int result[N][N];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix1[i][j] = rand() % 100;
                matrix2[i][j] = rand() % 100;
            }
        }

        std::cout << "Matrix 1:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << matrix1[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "Matrix 2:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << matrix2[i][j] << " ";
            }
            std::cout << std::endl;
        }

        int n = 1;
        for (int i = 0; i < N; i += elements, n++) {
            MPI_Send(&matrix1[i][0], elements * N, MPI_INT, n, 10, MPI_COMM_WORLD);
            MPI_Send(&matrix2[0][0], N * N, MPI_INT, n, 10, MPI_COMM_WORLD);
        }

        n = 1;
        for (int i = 0; i < N; i += elements, n++) {
            MPI_Recv(&result[i][0], elements * N, MPI_INT, n, 10, MPI_COMM_WORLD, &status);
        }

        std::cout << "Result:" << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cout << result[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else {
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int local_matrix1[elements][N];
        int local_matrix2[N][N];
        int local_result[elements][N];

        for (int i = 0; i < elements; i++) {
            for (int j = 0; j < N; j++) {
                local_result[i][j] = 0;
            }
        }

        MPI_Recv(&local_matrix1[0][0], elements * N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_matrix2[0][0], N * N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < elements; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    local_result[i][j] += local_matrix1[i][k] * local_matrix2[k][j];
                }
            }
        }
        MPI_Send(&local_result, elements * N, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
