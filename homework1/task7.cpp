#include <stdio.h>
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int arr_size = 12;
    int a[arr_size];
    int b[arr_size];
    int c[arr_size];

    #pragma omp parallel num_threads(3)
    {
        int threads_all = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        #pragma omp for schedule(static, 2)
        for (int i = 0; i < arr_size; i++)
        {
            a[i] = i * 1;
            b[i] = i * 2;
        }
        
        printf("Thread %d/%d  has finished initialization array a & b \n", thread_id, threads_all);
    }

    #pragma omp parallel num_threads(4)
    {
        int threads_all = omp_get_num_threads();
        int thread_id = omp_get_thread_num();

        #pragma omp for schedule(dynamic, arr_size / 8)
        for (int i = 0; i < arr_size; i++)
        {
            c[i] = a[i] + b[i];
        }
        printf("Thread %d/%d  has finished initialization array c \n", thread_id, threads_all);
    }

    cout << "Array a: ";
    for (int i = 0; i < arr_size; i++)
    {
        cout << a[i] << " ";
    }
    cout << "\nArray b: ";
    for (int i = 0; i < arr_size; i++)
    {
        cout << b[i] << " ";
    }
    cout << "\nArray c: ";
    for (int i = 0; i < arr_size; i++)
    {
        cout << c[i] << " ";
    }

    return 0;
}

/*
Динамическое выделение блоков по k итераций
Потоки получают по k итераций, по окончанию их обработки
запрашивают еще k итераций и т.д.
Заранее неизвестно какие итерации достанутся потокам
(зависит от порядка и длительности их выполнения)


*/