#include <stdio.h>
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int a[10] = {5, 2, 3, 7, 12, 43, 43, 55, 2, 1}; 
    int b[10] = {15, 43, 23, 111, 65, 7, 12, 0, -1, 5};

    int min_a = a[0];
    int max_b = b[0];

    #pragma omp parallel num_threads(2)
    {
        if (omp_get_thread_num() == 0) {
            for (size_t i = 0; i < size(a); i++)
            {
                min_a = min(min_a, a[i]);
            }
            
        }
        if (omp_get_thread_num() == 1) {
            for (size_t i = 0; i < size(b); i++)
            {
                max_b = max(max_b, b[i]);
            }
        }
    }

    cout << "min a: " << min_a << endl;
    cout << "max b: " << max_b << endl;

    return 0;
}
