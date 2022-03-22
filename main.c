
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "rtclock.h"
#include "mmm.h"

/* globals (anything here would be shared with all threads) */
double **mtrx_a;
double **mtrx_b;
double **mtrx_c;
int SIZE;
int t_num;

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
	// else if (strcmp(argv[1], "S"))
	// {
	// 	SIZE = atoi(argv[2]);
	// 	mtrx_a = mmm_init(SIZE);
	// 	mtrx_b = mmm_init(SIZE);
	// 	mtrx_c = mmm_init(SIZE);

	// 	//  mmm_reset(MC, SIZE);
	// 	double time = mmm_seq(mtrx_a, mtrx_b, mtrx_c, SIZE);
	// 	print_boilerplate('S', time, SIZE);
	// }
	else
	{
		t_num = atoi(argv[2]);
		SIZE = atoi(argv[3]);
		if (t_num >= SIZE)
		{
			t_num = SIZE - 1;
		}
		printf("%d\n", SIZE);
		mtrx_a = mmm_init(SIZE);
		printf("**************\n");
		mtrx_b = mmm_init(SIZE);
		printf("**************\n");
		mtrx_c = create_matrix();
		printf("**************\n");

		thread_args args;

		pthread_t threads[t_num];
		int portion = SIZE / t_num;
		args.j = portion;

		for (int i = 0; i < t_num; i++)
		{
			pthread_create(&threads[i], NULL, mmm_par, &args);
			args.i = args.j;
			args.j = portion * (i + 2);
		}

		for (int i = 0; i < t_num; i++)
		{
			pthread_join(threads[i], NULL);
		}

		printf("Resulting matrix: \n");
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				printf("%lf \t", mtrx_c[i][j]);
			}
			printf("\n");
		}
	}
}
