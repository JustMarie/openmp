#include <iostream>
#include <omp.h>
#include <ctime>
#include  <iomanip>

using namespace std;

const unsigned int N = 4;

void Init(float* Matr, int dim1, int dim2, bool random) {
         // if random=true, initialization by random element, false - by zeros

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

void Serial_Calc(float* Matr_A, float* Matr_B, float* Matr_C) {
        int i, j, k;
        for (i = 0; i<N; i++) {
        for (j = 0; j<N; j++)
        for (k=0; k<N; k++)
                Matr_C[i*N+j] += Matr_A[i*N+k] * Matr_B[k*N+j];
        }
}

void Line_Calc(float* Matr_A, float* Matr_B, float* Matr_C) {
        int i, j, k;
        int NestedThreadsNum = 2;
        omp_set_nested(true);
        omp_set_num_threads (NestedThreadsNum );
#pragma omp parallel for private (j, k)
        for (i=0; i<N; i++)
#pragma omp parallel for private (k)
        for (j=0; j<N; j++)
        for (k=0; k<N; k++)
                Matr_C[i*N+j] += Matr_A[i*N+k] * Matr_B[k*N+j];
}

void Block_Calc(float* Matr_A, float* Matr_B, float* Matr_C) {
        int block_size = 250;
        int grid_size = int (N/float(block_size));
#pragma omp parallel for
        for (int n=0; n<grid_size; n++)
        for (int m=0; m<grid_size; m++)
        for (int iter=0; iter<grid_size; iter++)
        for (int i=n*block_size;i<(n+1)*block_size; i++)
        for (int j=m*block_size;j<(m+1)*block_size; j++)
        for (int k=iter*block_size;k<(iter+1)*block_size; k++)
                Matr_C[i*N+j] += Matr_A[i*N+k] * Matr_B[k*N+j];

}

int main() {
        float A[N*N]; 
        float B[N*N]; 
        float serial_C[N*N]; 
        float line_C[N*N]; 
        float block_C[N*N]; 

        Init(A, N, N, true);
        Init(B, N, N, true);
        Init(serial_C, N, N, false);
        Init(line_C, N, N, false);
        Init(block_C, N, N, false);

        double start_serial = omp_get_wtime();
        Serial_Calc(A, B, serial_C);
        double stop_serial = omp_get_wtime();

        double start_line = omp_get_wtime();
        Line_Calc(A, B, line_C);
        double stop_line = omp_get_wtime();

        double start_block = omp_get_wtime();
        Block_Calc(A, B, block_C);
        double stop_block = omp_get_wtime();

        //cout  << "Matrix:" << endl;
        //Display_matrix(Matrix, N, N);

        //cout << endl << "Vector:" << endl;
        //Display_matrix(vect, 1, N);

        cout << endl << "Serial result with " << stop_serial-start_serial << " seconds:" << endl;
        //Display_matrix(serial_C, N, N);

        cout << endl << "Line calculation result with " << stop_line-start_line << " seconds" << endl;
        //Display_matrix(line_C, N, N);

				cout << endl << "Block calculation result with " << stop_block-start_block << " seconds:"  << endl;
        //Display_matrix(block_C, N, N);

        return 0;

}
