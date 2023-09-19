#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;

const int rows = 1400;
const int cols = 1400;

void matrixMultiply(int matrix[rows][cols], int vect[cols], int answer[rows]) {
    #pragma omp parallel for
    for (int i = 0; i < rows; i++)
    {
        int rowSum = 0;
        for (int j = 0; j < cols; j++)
        {
            rowSum += matrix[i][j] * vect[j];
        }
        answer[i] = rowSum;
    }

}

int main(int argc, char const *argv[])
{
    int matrix[rows][cols];
    int vect[cols];
    int answer[rows];

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = rand() % 100;
            if (i == 0) vect[j] = rand() % 100;
        }
    }

    double start = omp_get_wtime();
    matrixMultiply(matrix, vect, answer);
    double end = omp_get_wtime();
    cout << "Последовательное выполнение: " << (end - start) << endl;

    start = omp_get_wtime();

    #pragma omp parallel
    {
        matrixMultiply(matrix, vect, answer);
    }

    end = omp_get_wtime();
    cout << "Параллельное выполнение: " << (end - start) << endl;

    cout << endl << "Result vector: ";
    for (int i = 0; i < cols; i++)
    {
        cout << vect[i] << " ";
    }
    
    return 0;
}


