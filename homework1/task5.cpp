#include <stdio.h>
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int d[6][8];

    for (int i = 0; i < size(d); i++) {
        for (int j = 0; j < size(d[0]); j++) {
            d[i][j] = rand() % 100;
        }
    }

    /* print matrix */
    for (int i = 0; i < size(d); i++) {
        for (int j = 0; j < size(d[0]); j++) {
            cout << d[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n---------------\n\n";

    float avg = 0.0f;
    int min_val = d[0][0];
    int max_val = d[0][0];
    int count_m3 = 0;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            int sum = 0.0f;

            for (int i = 0; i < size(d); i++) {
                for (int j = 0; j < size(d[0]); j++) {
                    sum += d[i][j];
                }
            }

            #pragma omp critical
            avg += sum;

            // cout << "Thread 1) " << omp_get_thread_num() << " end work" << endl;
            printf("Thread 1) %d end work.\n", omp_get_thread_num());
        }

        #pragma omp section
        {
            for (int i = 0; i < size(d); i++) {
                for (int j = 0; j < size(d[0]); j++) {
                    min_val = min(min_val, d[i][j]);
                    max_val = max(max_val, d[i][j]);
                }
            }
            // cout << "Thread 2) " << omp_get_thread_num() << " end work" << endl;
            printf("Thread 2) %d end work.\n", omp_get_thread_num());
        }

        #pragma omp section
        {
            for (int i = 0; i < size(d); i++) {
                for (int j = 0; j < size(d[0]); j++) {
                    if (d[i][j] % 3 == 0) count_m3++;
                }
            }
            // cout << "Thread 3) " << omp_get_thread_num() << " end work" << endl;
            printf("Thread 3) %d end work.\n", omp_get_thread_num());
        }
    }
    
    cout << "\n---------------\n\n";
    cout << "average: " << avg / (size(d) * size(d[0])) << endl;
    cout << "max value: " << max_val << endl;
    cout << "min value: " << min_val << endl;
    cout << "numbers % 3 == 0: " << count_m3 << endl;

    return 0;
}

/*
Секции выполняются параллельно

#pragma omp critical - устраняет состояние гонки. (одновременно выполняется только одним потоком)
*/