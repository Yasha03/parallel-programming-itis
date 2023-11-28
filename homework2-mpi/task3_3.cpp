#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>
#include <cmath>

int main(int argc, char** argv) {

    int N = 100;
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(nullptr));

    double matrix[N][N];
    double vector[N];
    double local_result[N] = { 0 };

    if (rank == 0) {
        
        std::cout << "Matrix: " << std::endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                matrix[i][j] = rand() % 100;
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "Vector: " << std::endl;
        for (int i = 0; i < N; ++i) {
            vector[i] = rand() % 100;
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;
    }

    
    MPI_Bcast(vector, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int local_rows = N / size;

    double local_matrix[N/size][N];

    
    MPI_Scatter(matrix, local_rows * N, MPI_DOUBLE, local_matrix, local_rows * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    
    for (int i = 0; i < local_rows; ++i) {
        for (int j = 0; j < N; ++j) {
            local_result[i] += local_matrix[i][j] * vector[j];
        }
    }
    
    double global_result[N];
    MPI_Gather(local_result, local_rows, MPI_DOUBLE, global_result, local_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        
        std::cout << "Result:" << std::endl;
        for (int i = 0; i < N; ++i) {
            std::cout << global_result[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
