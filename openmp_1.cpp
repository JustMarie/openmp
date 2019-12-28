#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <ctime>
using namespace std;

const unsigned int DIM1 = 3;  // rows number
const unsigned int DIM2 = 5;  //columns number



float min_max_element(float mtr[DIM1][DIM2])
{
    int num_threads = DIM1 * DIM1;
    omp_set_num_threads(num_threads);
    float mins[DIM1];

    #pragma omp parallel for
    for (int i=0; i<DIM1; i++)
        mins[i] = mtr[i][0];

    #pragma omp parallel for
    for (int i=0; i<DIM1; i++) {
        #pragma omp parallel for reduction(min: minimums)
        for (int j = 0; j < DIM2; j++) {
            mins[i] = min(mins[i], mtr[i][j]);
        }

    }

    float min_max_value = mins[0];

    #pragma omp parallel for reduction(min: minimums)
    for (int i=0; i<DIM1; i++) {
        min_max_value = max(min_max_value, mins[i]);
    }

    return min_max_value;
}

int main() {
    float matrix[DIM1][DIM2];

    std::srand(unsigned(std::time(0)));
    for (int i=0; i<DIM1; i++) {
        for (int j = 0; j < DIM2; j++) {
            matrix[i][j] = rand();
        }

    }
   min_max = min_max_element(matrix);

    return 0;
}

