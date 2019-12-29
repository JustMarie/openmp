#include "stdafx.h"
#include <iostream>
#include <omp.h>
#include <ctime>
#include  <iomanip>

using namespace std;

const unsigned int N = 4;

void Init(float* Matr, int dim1, int dim2, bool random) {
        // if random=true, initialization by random elemen, false - by zeros

        //std::srand(unsigned(std::time(0)));
        srand(1);
        for (int i = 0; i < dim1; i++) 
                for (int j = 0; j < dim2; j++)
                        if  (random) Matr[i*N + j] = rand() % 10;
                        else Matr[i*dim2 + j] = 0;


}

void Display_matrix(float* Matr, int dim1, int dim2) {
        for (int i = 0; i < dim1; ++i) {
                for (int j = 0; j < dim2; ++j)
                        cout << setw(5) << Matr[i*dim2 + j] << " ";
                cout << endl;
        }
}

void Serial_Calc(float* Matr, float* vect, float* result_vect) {
        int i, j;
        for (i = 0; i<N; i++) {
                for (j = 0; j<N; j++)
                        result_vect[i] += Matr[i*N + j] * vect[j];
        }
}


// Data separation by rows
void Row_Calc(float* Matr, float* vect, float* result_vect) {
        int i, j;
        float sum = 0;

        for (i = 0; i<N; i++) {
                sum = 0;
#pragma omp paralell for redunction(+:sum)
                for (j = 0; j<N; j++)
                        sum += Matr[i*N + j] * vect[j];
                result_vect[i] = sum;
        }
}


// Data separation by columns
void Column_Calc(float* Matr, float* vect, float* result_vect) {
        int thread_num;
        float partial_sums[N*N];

        Init(partial_sums, N, N, false);

#pragma omp parallel shared (thread_num)
        {
                thread_num = omp_get_num_threads();
                int thread_id = omp_get_thread_num();
                int block_size = N / thread_num;
                float IterResult;
                for (int i = 0; i<N; i++) {
                        IterResult = 0;
                        for (int j = 0; j<block_size; j++)
                                IterResult += Matr[i*N + j + thread_id*block_size] *
                                vect[j + thread_id*block_size];
                        partial_sums[N*thread_id + i] = IterResult;
                }
        }
        for (int i = 0; i<N; i++)
                for (int j = 0; j<thread_num; j++)
                        result_vect[i] += partial_sums[j*N + i];
}

//Data separation by blocks
void Block_Calc(float* Matr, float* vect, float* result_vect) {
        int threads_num = 2;
        omp_set_num_threads(threads_num);
        omp_set_nested(true);
#pragma omp parallel for
        for (int i = 0; i<N; i++) {
                float thread_sum = 0;
#pragma omp parallel for reduction(+:thread_sum)
                for (int j = 0; j<N; j++)
                        thread_sum += Matr[i*N + j] * vect[j];
                result_vect[i] = thread_sum;
        }
}

int main() {
        float Matrix[N*N]; 
        float vect[N]; 
        float serial_result[N]; 
        float row_result[N]; 
        float column_result[N]; 
        float block_result[N]; 

        Init(Matrix, N, N, true);
        Init(vect, 1, N, true);
        Init(serial_result, 1, N, false);
        Init(row_result, 1, N, false);
        Init(column_result, 1, N, false);
        Init(block_result, 1, N, false);

        Serial_Calc(Matrix, vect, serial_result);
        Row_Calc(Matrix, vect, row_result);
        Column_Calc(Matrix, vect, column_result);
        Block_Calc(Matrix, vect, block_result);

        cout  << "Matrix:" << endl;
        Display_matrix(Matrix, N, N);

        cout << endl << "Vector:" << endl;
        Display_matrix(vect, 1, N);

        cout << endl << "Serial result:" << endl;
        Display_matrix(serial_result, 1, N);

        cout << endl << "Row result:" << endl;
        Display_matrix(row_result, 1, N);

        cout << endl << "Column result:" << endl;
        Display_matrix(column_result, 1, N);

        cout << endl << "Block result:" << endl;
        Display_matrix(block_result, 1, N);

        return 0;
}
