#include <stdio.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>
#include <cmath>

using namespace std;

bool isPrime(int n)
{
    if (n <= 1) return false;

    for (int i = 2; i <= n/2; i++) {
        if (n % i == 0) return false;
    }
    return true;
}


int main(int argc, char const *argv[])
{
    int start;
    int end;

    cout << "start: ";
    cin >> start;
    cout << "end: ";
    cin >> end;

    if (end <= start) {
        cout << "end < start";
        return 2;
    }


    cout << "primt numbers from " << start << " to " << end << endl;
    #pragma omp parallel for num_threads(5)
    for (int i = start; i <= end; i++) {
        if (isPrime(i)) {
            #pragma omp critical
            {
                cout << i << " ";
            }
        }
    }


    return 0;
}


