#include <iostream>
#include <omp.h>
#include <ctime>
#include  <iomanip>

using namespace std;

const unsigned int DIM = 4;  // rows number

// Max value in min values of each row

void min_max_element(int **m, int num_threads)
{
	int * mins_parall_crit = (int *)malloc(DIM * sizeof(int));
	int * mins_parall_red = (int *)malloc(DIM * sizeof(int));
	int * mins_seq = (int *)malloc(DIM * sizeof(int));

	double start_parall_crit, end_parall_crit;
	double start_parall_red, end_parall_red;
	double start_seq, end_seq;

	int i;

#pragma omp parallel num_threads(num_threads)
	if (omp_get_thread_num() == 0)
	{
		printf("\n\nParallel execution with %d threads and %dx%d matrix\n", omp_get_num_threads(), DIM, DIM);		
	}


	// Parallel section with CRITICAL directive
	start_parall_crit = omp_get_wtime();
#pragma omp parallel for shared(mins_parall_crit, m) private(i) num_threads(num_threads) 
	for (i = 0; i < DIM; i++)
		mins_parall_crit[i] = m[i][0];

#pragma omp parallel for shared(mins_parall_crit, m) num_threads(num_threads)
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (m[i][j] < mins_parall_crit[i])
#pragma omp critical
			{
				if (m[i][j] < mins_parall_crit[i])
				{
					mins_parall_crit[i] = m[i][j];
				}

			}
		}
	}

	int max_of_mins_parall_crit = mins_parall_crit[0];

#pragma omp parallel for shared(mins_parall_crit, max_of_mins_parall_crit) private(i) num_threads(num_threads)
	for (i = 0; i < DIM; i++)
		if (mins_parall_crit[i] > max_of_mins_parall_crit)
		{
#pragma omp critical
			{
				if (mins_parall_crit[i] > max_of_mins_parall_crit)
				{
					max_of_mins_parall_crit = mins_parall_crit[i];
				}
			}
		}
	end_parall_crit = omp_get_wtime();


	// REDUCTION section
	start_parall_red = omp_get_wtime();
#pragma omp parallel for shared(mins_parall_red, m) private(i) num_threads(num_threads) 
	for (i = 0; i < DIM; i++)
		mins_parall_red[i] = m[i][0];

#pragma omp parallel for shared(mins_parall_red, m) reduction(min: mins_parall_red)  num_threads(num_threads)
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (m[i][j] < mins_parall_red[i])
			{
				mins_parall_red[i] = m[i][j];
			}
		}
	}

	int max_of_mins_parall_red = mins_parall_red[0];

#pragma omp parallel for shared(mins_parall_red, max_of_mins_parall_red) reduction(max: max_of_mins_parall_red)  private(i) num_threads(num_threads)
	for (i = 0; i < DIM; i++)
		if (mins_parall_red[i] > max_of_mins_parall_red)
		{
			max_of_mins_parall_red = mins_parall_red[i];
		}
	end_parall_red = omp_get_wtime();


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

	printf("Time for critical, reduction and sequential sections:\n");
	printf("%f\n", end_parall_crit - start_parall_crit);
	printf("%f\n", end_parall_red - start_parall_red);
	printf("%f\n", end_seq - start_seq);

	printf("Critical result    : %d\n", max_of_mins_parall_crit);
	printf("Reduction result   : %d\n", max_of_mins_parall_red);
	printf("Sequential result  : %d\n", max_of_mins_seq);


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
	 print_matrix(matrix); 

	min_max_element(matrix, 1);
	min_max_element(matrix, 2);
	min_max_element(matrix, 4);
//	min_max_element(matrix, 8);
//	min_max_element(matrix, 16);
//	min_max_element(matrix, 32);

	return 0;

}
