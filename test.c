#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, int* argv[])
{
   int nthreads, tid;
   
   //omp_set_num_threads(8);
   
   // begin of parallel region
   #pragma omp parallel private(nthreads, tid)
   {
     // getting thread number 
      tid = omp_get_thread_num();
      printf("Welcome to GFG fromthread = %d\n", tid);
      
      if(tid==0)
      { 
        // only master thread does this 
          nthreads = omp_get_num_threads();
          printf("Number of threads = %d\n", nthreads);
         
      }
   }
}
