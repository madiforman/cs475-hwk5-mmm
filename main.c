
/*
 ============================================================================
 Name        : Paralellized Matrix Multplication
 Author      : Madison Sanchez-Forman
 Version     : 3.23.22
 Description : Performs parallel or sequential matrix multiplication, and
 computes the speedup of parallelized algorithm over sequential.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

/* globals (anything here would be shared with all threads) */
double **MA; /*matrixes A, B, and C*/
double **MB;
double **MC;
double **verify_c; /*for verifying C*/
int SIZE;		   /*dimension of square matrix*/
int t_num;		   /*num threads if parallel*/

int main(int argc, char *argv[])
{

	if (argc == 1 || argc == 2)
	{
		printf("Usage: ./mmm <mode> [num threads] <size>\n");
	}
	else if ((strcmp(argv[1], "P") == 0) && argc == 3)
	{
		printf("Error: parallel mode requires <size>\n");
	}
	else
	{

		if (strcmp(argv[1], "S") == 0)
		{
			SIZE = atoi(argv[2]);
			MA = mmm_init();
			MB = mmm_init();
			MC = create_matrix();
			printf("========\n");
			printf("mode: sequential\n");
			printf("thread count: 1\n");
			printf("size: %d\n", SIZE);
			printf("========\n");
			double time = mmm_seq(MC); /*mmm_seq returns time taken*/
			printf("Sequential Time: %f\n", time);
			mmm_freeup('S');
		}
		else if (strcmp(argv[1], "P") == 0)
		{
			SIZE = atoi(argv[3]);
			MA = mmm_init();
			MB = mmm_init();
			MC = create_matrix();
			verify_c = mmm_init();

			t_num = atoi(argv[2]);
			printf("========\n");
			printf("mode: parallel\n");
			printf("thread count: %d\n", t_num);
			printf("size: %d\n", SIZE);
			printf("========\n");

			double start = rtclock();

			pthread_t threads[t_num]; /*create array of threads and thread arguments*/
			thread_args t_args[t_num];

			double portion = SIZE / t_num; /*dividing up portions of matrix*/

			for (int i = 0; i < t_num; i++)
			{
				if (i == t_num - 1)
				{
					t_args[i].end = SIZE;
				}
				t_args[i].start = portion * i;
				t_args[i].end = portion * (i + 1);
				t_args[i].tid = i;

				pthread_create(&threads[i], NULL, mmm_par, &t_args[i]);
			}
			for (int i = 0; i < t_num; i++)
			{
				pthread_join(threads[i], NULL);
			}
			double end = rtclock();

			double par_time = end - start;
			double seq_time = mmm_seq(verify_c);
			double speedup = seq_time / par_time;

			printf("Sequential Time: %lf\n", seq_time);
			printf("Parallel Time: %lf\n", par_time);
			printf("Speedup: %lf\n", speedup);
			printf("Verifying... largest error between parallel and sequential matrix: %lf\n", mmm_verify());
			mmm_freeup('P');
		}
	}
}
