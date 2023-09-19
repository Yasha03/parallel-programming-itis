#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;



int main(int argc, char const *argv[])
{
    int maxThreads = 7;

    #pragma omp parallel num_threads(maxThreads)
    {
        for (int i = omp_get_num_threads(); i >= 0; i--)
        {
            #pragma omp barrier // ждем пока все потоки дойдут до сюда
            {
                if (i == omp_get_thread_num()) {
                    #pragma omp critical
                    cout << "Thread num: " << omp_get_thread_num() << ". Hello world!" << endl;
                }
            }
        }
        
    }


    return 0;
}


