#include <stdio.h>
#include <omp.h>
#include <string.h>
#define COUNT 8


char search_words[COUNT][20] = {"The", "Hey", "mY", "ipsum", "present", "dolor", "sit", "in"};
long counts[COUNT];
char buffer[50];


void checkIfPresent(char *buffer){ 
   for(int i=0; i<COUNT; i++){
       if((strcasecmp(buffer, search_words[i]))==0) counts[i]++;
       //strcmp is case sensitive
   }
}
int main() 
{
 int i, threads ;
 char choice, buf;
 FILE *fp;
 for (threads = 1; threads <= 16; threads *= 2)
 {
  for(i = 0; i < COUNT; i++) 
  { 
   counts[i] = 0; 
  }
	
  fp = fopen("words.txt", "r");
  
  double t = omp_get_wtime();
  #pragma omp parallel shared(counts, search_words) private(i) num_threads(threads)
  { 
  
    while(fscanf(fp, "%s", buffer)==1) // reads word by word
    {
      // printf("%s\n", buffer);
       checkIfPresent(buffer); 
    }
  
  }
  t = omp_get_wtime() - t;

  for(i = 0; i < COUNT; i++) 
  { 
   printf("\n%s: %ld",search_words[i], counts[i]); 
  }
  printf("\n%d Threads, time = %lf\n", threads, t);
 }
}
