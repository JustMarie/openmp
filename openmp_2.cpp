#include "stdafx.h"
#include <iostream>
#include <omp.h>
#include <ctime>
#include  <iomanip>

using namespace std;

const unsigned int N = 3;  

void print_matrix(float a[N][N]) {

	for (int r = 0; r < N; ++r) {
		for (int c = 0; c < N; ++c)
			cout << setw(5) << a[r][c] << " ";
		cout << endl;
	}
}

int main() {
	float a[N][N];
	float b[N][N];
	float c[N][N];

	//std::srand(unsigned(std::time(0)));
	srand(1);
	for (int i = 0; i < N; i++) 
		for (int j = 0; j < N; j++) {
			a[i][j] = rand() % 10;
			b[i][j] = rand() % 10;
			c[i][j] = 0;
		}
	
#pragma omp parallel for shared(a, b)
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++) {
				c[i][j] += a[i][k] * b[k][j];
				int myID = omp_get_thread_num();
				int threads = omp_get_num_threads();
				std::cout << "Num of thread is " << myID << " from " << threads << endl;

			}
	
	cout << "Matrix A:" << endl;
	print_matrix(a);
	cout << endl << "Matrix B:" << endl;
	print_matrix(b);
	cout << endl << "Matrix C=A*B:" << endl;
	print_matrix(c);
	
	return 0;
}
