#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <ctime>
#include <cmath>

using namespace std;



int main(int argc, char const *argv[])
{
    int a[30] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
    int p = 2;
    int dec = 0;

    #pragma omp parallel for reduction(+ : dec)
    for (int i = 0; i < 30; i++) {
        dec += a[i] * pow(p, 29 - i); 
    }

    cout << "res: " << dec << endl;

    return 0;
}


