#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>

using namespace std;

int const ARR_SIZE = 10;
const int MIN_VALUE = 0; // минимальное значение элементов матрицы
const int MAX_VALUE = 20; // максимальное значение

void show_arr(int* arr, int size)
{
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
}

void show_vec(int* arr, int size)
{
    string str = "[";
    for (int i = 0; i < size; i++) {
        if (i < size - 1) {
            str = str + std::to_string(arr[i]) + ", ";
        }
        else {
            str = str + std::to_string(arr[i]);
        }
    }

    str += "]";
    cout << str << endl;
}

void show_matrix(int** arr, int x, int y)
{
    int* buf = new int[y];
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {

            buf[j] = arr[i][j];
        }
        show_vec(buf, y);
        buf = new int[y];
    }
    delete[]buf;
}

void mult_vec(int* arrX, int size, int* arrY)
{
    for (int i = 0; i < size; i++) arrX[i] = arrX[i] * arrY[i];
}

void reverce_vecS(int* arrX, int size, int* arrY)
{
    int q;
    for (int i = 0; i < size; i++) {
        q = arrX[i];
        arrX[i] = arrY[i];
        arrY[i] = q;
    }
}

int main(int argc, char** argv)
{
    int size = 4, rank;
    int rc;
    int const len = 9;
    MPI_Status mpi_stat;

    if (rc = MPI_Init(&argc, &argv))
    {
        cout << "Ошибка запуска, выполнение остановлено " << endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = size;
    N--;

    // убираем root поток
    size--;

    srand(time(NULL));

    // главный поток == root
    if (rank == 0) {
        cout << " MPI - TASK 8 \n\n";
        // счетчик потоков
        int thread_id = 1;
        const int* q = new int[len];

        int** x = new int* [N];
        int** y = new int* [N];
        int** c = new int* [N];
        for (int i = 0; i < N; ++i) {
            x[i] = new int[len];
            y[i] = new int[len];
            c[i] = new int[len];
        }

        //заполенение векторов
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < len; ++j) {
                x[i][j] = rand() % 10;
                y[i][j] = rand() % 10;
            }
        }

        cout << "A: \n";
        show_matrix(x, N, len);
        cout << "\nB \n";
        show_matrix(y, N, len);
        cout << "\n";

        for (int i = 1; i <= size; i ++) {
            MPI_Send(x[i-1], len, MPI_INT, i, 11000, MPI_COMM_WORLD);
            MPI_Send(y[i-1], len, MPI_INT, i, 11001, MPI_COMM_WORLD);
        }

        for (int i = 1; i <= size; i ++) {

            int* buf = new int[len];

            MPI_Recv(&buf[0], len, MPI_INT, i, 11002, MPI_COMM_WORLD, &mpi_stat);

            c[i-1] = buf;
        }

        cout << "\nC: \n";
        show_matrix(c, N, len);
    }
    else {

        int* x = new int[len];
        int* y = new int[len];
        MPI_Recv(x, len, MPI_INT, 0, 11000, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(y, len, MPI_INT, 0, 11001, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        mult_vec(x, len, y);

        MPI_Send(x, len, MPI_INT, 0, 11002, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return MPI_SUCCESS;
}
