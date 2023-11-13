#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

const int ARR_SIZE = 40; 
const int ROOT = 0;
const int a = 3;
const int b = 4;

void show_vec(int* arr, int size) {
    std::cout << "[";
    for (int i = 0; i < size; i++) {
        if (i < size - 1) {
            std::cout << arr[i] << ", ";
        } else {
            std::cout << arr[i];
        }
    }
    std::cout << "]\n";
}

void mult_vec(int* arrX, int size, int* arrY) {
    for (int i = 0; i < size; i++) {
        arrX[i] = a*arrX[i] + b*arrY[i];
    }
}

int main(int argc, char* argv[]) {
    int size, rank;
    int rc;
    MPI_Status mpi_stat;

    if (rc = MPI_Init(&argc, &argv)) {
        std::cerr << "Ошибка запуска, выполнение остановлено " << std::endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Проверяем, что количество процессов не больше ARR_SIZE
    if (size > ARR_SIZE) {
        if (rank == ROOT) {
            std::cerr << "Количество процессов больше, чем ARR_SIZE. Уменьшите количество процессов." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    size--;

    int countSendElements = ARR_SIZE / size;
    int lastCount = ARR_SIZE % size;

    srand(time(NULL));

    if (rank == ROOT) {
        // Счетчик потоков
        int thread_id = 1;
        int x[ARR_SIZE];
        int y[ARR_SIZE];

        for (int i = 0; i < ARR_SIZE; ++i) {
            x[i] = rand() % 10;
            y[i] = rand() % 10;
        }

        std::cout << "vect x:\n";
        show_vec(x, ARR_SIZE);
        std::cout << "vect y:\n";
        show_vec(y, ARR_SIZE);

        if (size > 0) {
            int interval = countSendElements;
            int index = 0;
            for (int i = 0; i < size; i++) {
                if (i == size - 1 && lastCount != 0) {
                    interval = lastCount;
                }

                int* x_buf = new int[interval];
                int* y_buf = new int[interval];

                for (int j = 0; j < interval; j++, index++) {
                    x_buf[j] = x[index];
                    y_buf[j] = y[index];
                }

                MPI_Send(x_buf, interval, MPI_INT, thread_id, 11000, MPI_COMM_WORLD);
                MPI_Send(y_buf, interval, MPI_INT, thread_id, 11000, MPI_COMM_WORLD);

                delete[] x_buf;
                delete[] y_buf;

                thread_id++;
            }
        }

        // Получение результатов от процессов
        int interval = countSendElements;
        int index = 0;
        for (int i = 1; i <= size; i++) {
            if (i == size && lastCount != 0) {
                interval = lastCount;
            }

            int* buf = new int[interval];
            MPI_Recv(buf, interval, MPI_INT, i, 203023024, MPI_COMM_WORLD, &mpi_stat);

            for (int j = 0; j < interval; j++, index++) {
                if (index < ARR_SIZE) {
                    y[index] = buf[j];
                }
            }
            delete[] buf;
        }
        std::cout << "\nresult:\n";
        show_vec(y, ARR_SIZE);
    } else {
        // Остальные процессы
        MPI_Probe(ROOT, 11000, MPI_COMM_WORLD, &mpi_stat);
        int buf_size;
        MPI_Get_count(&mpi_stat, MPI_INT, &buf_size);

        int* x_buf = new int[buf_size];
        int* y_buf = new int[buf_size];
        MPI_Recv(x_buf, buf_size, MPI_INT, ROOT, 11000, MPI_COMM_WORLD, &mpi_stat);
        MPI_Recv(y_buf, buf_size, MPI_INT, ROOT, 11000, MPI_COMM_WORLD, &mpi_stat);

        mult_vec(x_buf, buf_size, y_buf);

        MPI_Send(x_buf, buf_size, MPI_INT, ROOT, 203023024, MPI_COMM_WORLD);

        delete[] x_buf;
        delete[] y_buf;
    }

    MPI_Finalize();

    return MPI_SUCCESS;
}
