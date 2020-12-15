#include <iostream>
#include <omp.h>
#include <ctime>
#include  <iomanip>

using namespace std;

const unsigned int DIM = 100;  // rows number

// Max value in min values of each row

void min_max_element(int **m, int num_threads)
{
	int * mins_parall = (int *) malloc(DIM * sizeof(int));
	int * mins_seq = (int *)malloc(DIM * sizeof(int));

	double start_parall, end_parall;
	double start_seq, end_seq;

	int i;

#pragma omp parallel num_threads(num_threads)
	if (omp_get_thread_num() == 0)
	{
		printf("\n\nParallel execution with %d threads and %d elements\n", omp_get_num_threads(), DIM);
	}


	// Parallel section
	start_parall = omp_get_wtime();
#pragma omp parallel for shared(mins_parall, m) private(i) num_threads(num_threads) 
	for (i = 0; i < DIM; i++)
		mins_parall[i] = m[i][0];

#pragma omp parallel for shared(mins_parall, m) num_threads(num_threads)
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (m[i][j] < mins_parall[i])
#pragma omp critical
			{
				if (m[i][j] < mins_parall[i])
				{
					mins_parall[i] = m[i][j];
				}
				
			}
		}
	}

	int max_of_mins_parall = mins_parall[0];

#pragma omp parallel for shared(mins_parall, max_of_mins_parall) private(i) num_threads(num_threads)
	for (i = 0; i < DIM; i++)
		if (mins_parall[i] > max_of_mins_parall)
		{
#pragma omp critical
			{
				if (mins_parall[i] > max_of_mins_parall) 
				{
					max_of_mins_parall = mins_parall[i];
				}
			}
		}
	end_parall = omp_get_wtime();

	// Sequential section
	start_seq = omp_get_wtime();
	for (i = 0; i < DIM; i++)
		mins_seq[i] = m[i][0];

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (m[i][j] < mins_seq[i])
			{
				mins_seq[i] = m[i][j];

			}
		}
	}

	int max_of_mins_seq = mins_seq[0];
	for (i = 0; i < DIM; i++)
		if (mins_seq[i] > max_of_mins_seq)
		{
			{
				max_of_mins_seq = mins_seq[i];
			}
		}
	end_seq = omp_get_wtime();

	printf("Parallel   : %f seconds\n", end_parall - start_parall);
	printf("Sequential : %f seconds\n", end_seq - start_seq);

	printf("Parallel result   : %d\n", max_of_mins_parall);
	printf("Sequential result : %d\n", max_of_mins_seq);

	
}

void print_matrix(int **m) {

	for (int r = 0; r < DIM; ++r) {
		for (int c = 0; c < DIM; ++c)
			cout << setw(5) << m[r][c] << " ";
		cout << endl;
	}
}

int main() {

	int **matrix = (int **)malloc(DIM * sizeof(int *));
	for (int i = 0; i < DIM; i++)
		matrix[i] = (int *)malloc(DIM * sizeof(int));


	std::srand(unsigned(std::time(0)));
	//srand(1);
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			matrix[i][j] = rand() % 10;
		}
	}

	// only for degug
	// print_matrix(matrix); 

	min_max_element(matrix, 1);
	min_max_element(matrix, 2);
	min_max_element(matrix, 4);
	min_max_element(matrix, 8);
	min_max_element(matrix, 16);
	min_max_element(matrix, 32);

	return 0;
}
