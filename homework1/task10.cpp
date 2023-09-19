#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;



int main(int argc, char const *argv[])
{
    int size = 30;
    int a[size];
    int trueAnswer = 0;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 100;
        if (a[i] % 9 == 0) trueAnswer++;
        cout << a[i] << " ";
    }

    int answer = 0;
    
    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < size; i++)
    {
        if (a[i] % 9 == 0) {
            #pragma omp atomic
            answer++;
        }
    }

    int answerNoAtomic = 0;

    #pragma omp parallel for num_threads(8)
    for (int i = 0; i < size; i++)
    {
        if (a[i] % 9 == 0) {
            answerNoAtomic++;
        }
    }
    
    
    cout << endl << "Result: " << answer;
    cout << endl << "Result without atomic: " << answerNoAtomic;
    cout << endl << "Correct answer: " << trueAnswer;

    return 0;
}


