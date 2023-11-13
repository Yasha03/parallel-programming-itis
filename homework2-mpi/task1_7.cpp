#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

const int MATRIX_SIZE = 10;
const int VECTOR_SIZE = 10;

void show_vector(const std::vector<int>& vec) {
    for (int element : vec) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Размер блока матрицы, который будет обрабатываться каждым процессом
    const int block_size = MATRIX_SIZE / world_size;

    // Создание и инициализация матрицы A и вектора B на нулевом процессе
    std::vector<std::vector<int>> A(MATRIX_SIZE, std::vector<int>(MATRIX_SIZE));
    std::vector<int> B(VECTOR_SIZE);
    std::vector<int> local_result(block_size, 0);
    std::vector<int> result(VECTOR_SIZE, 0);

    if (world_rank == 0) {
        // Инициализация матрицы A случайными значениями
        srand(time(NULL));
        std::cout << "matrix a: \n";
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                A[i][j] = rand() % 100; // Случайное значение от 0 до 99
                std::cout << A[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\nvect b: \n";
        // Инициализация вектора B случайными значениями
        for (int i = 0; i < VECTOR_SIZE; i++) {
            B[i] = rand() % 100; // Случайное значение от 0 до 99
            std::cout << B[i] << " ";
        }
        std::cout << "\n";
    }

    // Рассылка матрицы A и вектора B на все процессы
    for (int i = 0; i < MATRIX_SIZE; i++) {
        MPI_Bcast(A[i].data(), MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(B.data(), VECTOR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Вычисление части вектора C
    for (int i = world_rank * block_size; i < (world_rank + 1) * block_size; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            local_result[i - world_rank * block_size] += A[i][j] * B[j];
        }
    }

    // Сбор результатов на нулевом процессе
    MPI_Gather(local_result.data(), block_size, MPI_INT, result.data(), block_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Вывод результата на нулевом процессе
    if (world_rank == 0) {
        std::cout << "\nРезультат умножения: \n";
        show_vector(result);
    }

    MPI_Finalize();

    return 0;
}