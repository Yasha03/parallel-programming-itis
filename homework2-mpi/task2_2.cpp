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
    const int part = 10 / 5;

    if (rank == 0) {
        int matrix[N][N];
        int vector[N];
        int localResult[N];

        std::cout << "Matrix: " << std::endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = rand() % 100;
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Vector: ";
        for (int i = 0; i < N; i++) {
            vector[i] = rand() % 100;
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;

        int n = 1;
        for (int i = 0; i < N; i += 2, n++) {
            MPI_Send(&matrix[i][0], part * N, MPI_INT, n, 10, MPI_COMM_WORLD);
            MPI_Send(&vector[0], N, MPI_INT, n, 10, MPI_COMM_WORLD);
        }

        n = 1;
        for (int i = 0; i < N; i += 2, n++) {
            MPI_Recv(&localResult[i], part, MPI_INT, n, 10, MPI_COMM_WORLD, &status);
        }

        std::cout << "Result: ";
        for (int i = 0; i < N; i++) {
            std::cout << localResult[i] << " ";
        }
        std::cout << std::endl;
    }
    else {
        MPI_Probe(0, 10, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int local_matrix[part][N];
        int local_vector[N];
        int local_result[part];

        for (int i = 0; i < part; i++) {
            local_result[i] = 0;
        }

        MPI_Recv(&local_matrix[0][0], part * N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_vector[0], N, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);

        for (int i = 0; i < part; i++) {
            for (int j = 0; j < N; j++) {
                local_result[i] += local_matrix[i][j] * local_vector[j];
            }
        }
        MPI_Send(&local_result, part, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}

