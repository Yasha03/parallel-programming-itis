#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <ctime>
#include <cmath>

using namespace std;



int main(int argc, char const *argv[])
{
    int N = 210;
    long ans = 0;
    long trueAnswer = N * N;

    #pragma omp parallel for reduction(+ : ans)
    for (int i = 1; i < N * 2; i += 2)
    {
        ans += i;
    }
    

    cout << "right answer: " << trueAnswer << endl;
    cout << "omp answer: " << ans << endl;

    return 0;
}


