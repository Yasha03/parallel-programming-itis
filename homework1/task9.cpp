#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <ctime>

using namespace std;



int main(int argc, char const *argv[])
{
    int rows = 6;
    int cols = 8;
    int d[rows][cols];

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            d[i][j] = std::rand() % 100;
            cout << d[i][j] << " ";
        }
        cout << endl;
    }

    int minValue = d[0][0];
    int maxValue = d[0][0];

    #pragma omp parallel for num_threads(5)
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int currVal = d[i][j];
            #pragma omp critical
            {
                if (currVal < minValue) minValue = currVal;

                if (currVal > maxValue) maxValue = currVal;
            }
        }
        
    }

    cout << "min: " << minValue << "\nmax: " << maxValue;
    
    return 0;
}


