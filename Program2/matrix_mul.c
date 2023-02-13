
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 200

int A[N][N];
int B[N][N];
int C[N][N];

int main() 
{
   
    int i,j,k;
 
double start, end, elapsed; 
for(int t=1;t<=16;t=t*2)
{
    omp_set_num_threads(t);
    for (i= 0; i< N; i++)
        for (j= 0; j< N; j++)
	{
            A[i][j] = 2;
            B[i][j] = 2;
	}
    start = omp_get_wtime();
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            for (k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    end = omp_get_wtime();
    elapsed = end - start;
    printf("Size=%d \t Threads=%d\t elapsed time = %f seconds.\n",N,t,elapsed);
   }
}
