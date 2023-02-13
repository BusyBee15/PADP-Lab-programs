#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int i, n, nthreads;
  double x, y, pi, sum, start, end;

  for(nthreads = 1; nthreads <= 8; nthreads*=2) {
    omp_set_num_threads(nthreads);
    
    for(n=10000;n<=100000000;n*=10) { // consider these n as points inside the square
    
        sum = 0.0;
        start = omp_get_wtime();
        #pragma omp parallel for private(i, x, y) reduction(+:sum)
        for (i = 0; i < n; i++) {
            x = (double)rand() / RAND_MAX;
            y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1) {
                sum++;
            }
        }
        pi = 4 * sum / n;
        
        end = omp_get_wtime();
        printf("%d Threads, %d Trials, pi = %f, Time = %lf\n", nthreads, n, pi, end-start);
    }
  }
  return 0;
}
