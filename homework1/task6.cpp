#include <stdio.h>
#include <iostream>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int a[10] = {5, 2, 3, 7, 12, 43, 43, 55, 2, 1}; 
    int b[10] = {15, 43, 23, 111, 65, 7, 12, 0, -1, 5};
    int size_arr = size(a);

    float sum_a = 0.0f;
    float sum_b = 0.0f;

    #pragma omp parallel for reduction(+ : sum_a, sum_b)
    for (int i = 0; i < size_arr; i++) {
        sum_a += a[i];
        sum_b += b[i];
    }

    float avg_a = sum_a / size_arr;
    float avg_b = sum_b / size_arr;

    cout << "average a: " << avg_a << endl;
    cout << "average b: " << avg_b << endl;

    cout << (avg_a > avg_b ? "avg a > avg b" : "avg b >= avg a") << endl;

    return 0;
}
