#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;



int main(int argc, char const *argv[])
{
    #pragma omp parallel num_threads(6)
    {
        #pragma omp critical
        cout << "Thread num: " << (omp_get_num_threads() - omp_get_thread_num()) << ". Hello world!" << endl;
    }

    return 0;
}


