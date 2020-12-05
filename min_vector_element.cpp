#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char** argv)
{
	const int N = atoi(argv[1]);
	double arr[N];
	
	double max_val_paral = 0.0;
	double max_val_seq = 0.0;
	int i;

	double start1;  // parallel execution ,easure
	double end1;
	double start2;   // sequence execution measure
	double end2;


	std::srand(1);
	for (i = 0; i < N; i++)
		arr[i] = rand() % 1000;

	omp_set_dynamic(0);

#pragma omp parallel num_threads(4)
	/*if (omp_get_thread_num() == 0)
	{
		printf("\n\nParallel execution with %d threads and %d elements\n ", omp_get_num_threads(), N);
	}*/

	start1 = omp_get_wtime();
#pragma omp parallel for reduction(max : max_val) num_threads(omp_get_max_threads())
		for (i = 0;i < N; i++)
		{
			if (arr[i] > max_val_paral)
			{
				max_val_paral = arr[i];
			}
		}

// OR 

//#pragma omp parallel for shared(max_val_paral) num_threads(4)
//		for (i = 0;i < N; i++)
//		{
//			if (arr[i] > max_val_paral)
//			{
//#pragma omp critical
//				max_val_paral = arr[i];
//			}
//		}

	end1 = omp_get_wtime();

	printf("Max element with parallel execution = %f\n", max_val_paral);
	printf("Work took %f seconds\n", end1 - start1);

	start2 = omp_get_wtime();
	for (i = 0;i < N; i++)
	{
		if (arr[i] > max_val_seq)
		{
			max_val_seq = arr[i];
		}
	}
	end2 = omp_get_wtime();

	printf("Max element with sequential execution = %f\n", max_val_seq);
	printf("Work took %f seconds\n", end2 - start2);

	if (max_val_paral = max_val_seq)
	{
		printf("Parallel and sequential sections got the same result\n");
	}
	else
	{
		printf("Error! Parallel and sequential sections got a different results\n");
	}
}
