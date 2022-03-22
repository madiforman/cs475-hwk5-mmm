#ifndef MMM_H_
#define MMM_H_

typedef struct thread_args
{
    int i;
    int j;
} thread_args;

extern double **mtrx_a;
extern double **mtrx_b;
extern double **mtrx_c;
extern int SIZE;
extern int t_num;

void test();

double **mmm_init(int size);
double **mmm_reset(int size);
double **create_matrix();
void mmm_freeup();
double mmm_seq(double **MA, double **MB, double **MC, int size);
void *mmm_par(void *args);
double mmm_verify();
void print_boilerplate(char mode, double time, int size);

#endif /* MMM_H_ */
