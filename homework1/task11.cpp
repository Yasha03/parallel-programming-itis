#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;



int main(int argc, char const *argv[])
{
    int size = 1000;
    int arr[size];

    int correntAnswer = INT16_MIN;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % 100;
        cout << arr[i] << " ";
        if (arr[i] % 7 == 0) {
            if (arr[i] > correntAnswer) correntAnswer = arr[i];
        }
    }

    
    int noAtomicAnswer = arr[0];
    int answer = arr[0];

    // правильное решение
    #pragma omp parallel for num_threads(4) shared(answer)
    for (int i = 0; i < size; i++)
    {
        if (arr[i] % 7 == 0) {
            #pragma omp critical
            {
                if (arr[i] > answer) {
                    answer = arr[i];
                }
            }
        }
    }
    
    // без критической секции
    #pragma omp parallel for num_threads(7) shared(noAtomicAnswer)
    for (int i = 0; i < size; i++)
    {
        if (arr[i] % 7 == 0) {
            if (arr[i] > noAtomicAnswer) {
                noAtomicAnswer = arr[i];
            }
        }
    }
    
    cout << endl << "Result: " << answer;
    cout << endl << "Result without atomic: " << noAtomicAnswer;
    cout << endl << "Correct answer: " << correntAnswer;

    return 0;
}


