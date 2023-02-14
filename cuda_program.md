##### Cuda Program

(Executed this on google colab)

`!nvcc --version`

`!pip install git+https://github.com/andreinechaev/nvcc4jupyter.git`

`%load_ext nvcc_plugin`

```
%%cu
#include <iostream>
int main()
{
	std::cout << "Welcome To Bitonic sort \n";
	return 0;
}
```

<br>
<br>

```
%%cu
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>


#define THREADS 512 // 2^9
#define BLOCKS 32768 // 2^15
#define NUM_VALS THREADS*BLOCKS

void print_elapsed(clock_t start, clock_t stop)
{
  double elapsed = ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("Elapsed time: %.3fs\n", elapsed);
}

float random_float()
{
  return (float)rand()/(float)RAND_MAX;
}

void array_print(float *arr, int length)
{
  int i;
  for (i = 0; i < length; ++i) {
    printf("%1.3f ",  arr[i]);
  }
  printf("\n");
}

void array_fill(float *arr, int length)
{
  srand(time(NULL));
  int i;
  for (i = 0; i < length; ++i) {
    arr[i] = random_float();
  }
}

__global__ void bitonic_sort_step(float *dev_values, int j, int k)
{
  unsigned int i, ixj; /* Sorting partners: i and ixj */
  i = threadIdx.x + blockDim.x * blockIdx.x;
  ixj = i^j;

  /* The threads with the lowest ids sort the array. */
  if ((ixj)>i) {
    if ((i&k)==0) {
      /* Sort ascending */
      if (dev_values[i]>dev_values[ixj]) {
        /* exchange(i,ixj); */
        float temp = dev_values[i];
        dev_values[i] = dev_values[ixj];
        dev_values[ixj] = temp;
      }
    }
    if ((i&k)!=0) {
      /* Sort descending */
      if (dev_values[i]<dev_values[ixj]) {
        /* exchange(i,ixj); */
        float temp = dev_values[i];
        dev_values[i] = dev_values[ixj];
        dev_values[ixj] = temp;
      }
    }
  }
}

/**
 * Inplace bitonic sort using CUDA.
 */
void bitonic_sort(float *values)
{
  float *dev_values;
  size_t size = NUM_VALS * sizeof(float);

  cudaMalloc((void**) &dev_values, size);
  cudaMemcpy(dev_values, values, size, cudaMemcpyHostToDevice);

  dim3 blocks(BLOCKS,1);    /* Number of blocks   */
  dim3 threads(THREADS,1);  /* Number of threads  */

  int j, k;
  /* Major step */
  for (k = 2; k <= NUM_VALS; k <<= 1) {
    /* Minor step */
    for (j=k>>1; j>0; j=j>>1) {
      bitonic_sort_step<<<blocks, threads>>>(dev_values, j, k);
    }
  }
  cudaMemcpy(values, dev_values, size, cudaMemcpyDeviceToHost);
  cudaFree(dev_values);
}

void bitonicsort(float *values){
int i,j;float a;
for (i = 0; i < NUM_VALS; ++i)
            for (j = i + 1; j < NUM_VALS; ++j)
                if (values[i] > values[j])
                {

                    a =  values[i];
                    values[i] = values[j];
                    values[j] = a;

                }
                array_print(values,NUM_VALS);

            }
 void swap(float a, float b) {
    int temp = a;
    a = b;
    b = temp;
}
 void bitonic_merge(float *arr, int N, int dir) {
    if (N <= 1) return;
    int m = N / 2;
    for (int i = 0; i < m; i++) {
        if ((dir == (arr[i] > arr[i + m])) == 1) {
            swap(arr[i], arr[i + m]);
        }
    }
    bitonic_merge(arr, m, dir);
    bitonic_merge(arr + m, m, dir);
}
 void bitonic_sort1(float *arr, int N, int dir) {
    if (N <= 1) return;
    int m = N / 2;
    for (int i = 0; i < m; i++) {
        if ((dir == (arr[i] > arr[i + m])) == 1) {
            swap(arr[i], arr[i + m]);
        }
    }
    bitonic_sort1(arr, m, dir);
    bitonic_sort1(arr + m, m, dir);
    bitonic_merge(arr, N, dir);
}



int main(void)
{
  clock_t start, stop;

  float *values = (float*) malloc( NUM_VALS * sizeof(float));
  array_fill(values, NUM_VALS);

 // std::cout << "Unsorted values \n";
  //array_print(values, NUM_VALS);



    start = clock();
    bitonic_sort1(values,NUM_VALS,1);
    stop = clock();

    /* Get the time taken by program to execute in seconds */
    double duration = ((double)stop - start)/CLOCKS_PER_SEC;

    printf("Time taken ON CPU : %f\n", duration);

   printf("After sorting on GPU");
   start = clock();
   bitonic_sort(values);
   stop = clock();

   duration = ((double)stop - start)/CLOCKS_PER_SEC;

    printf("Time taken ON GPU : %f\n", duration);

  //std::cout << "Sorted values \n";
  //array_print(values, NUM_VALS);


}

```
