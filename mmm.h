#ifndef MMM_H_
#define MMM_H_

typedef struct thread_args
{
    int tid;
    int start; /*start and end of a row*/
    int end;
} thread_args;

extern double **MA;
extern double **MB;
extern double **MC;
extern double **verify_c;
extern int SIZE;
extern int t_num;

double **mmm_init();
void mmm_reset(double **matrix);
double **create_matrix();
void mmm_freeup(char mode);
double mmm_seq(double **matrix);
void *mmm_par(void *args);
double mmm_verify();
void print_boilerplate(char mode, double time, int size);

#endif /* MMM_H_ */
