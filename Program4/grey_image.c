#include <stdio.h>
#include <gd.h>
#include <omp.h>
#include <string.h>

int main(int argc, char **argv) {

    FILE *fp; 
    gdImagePtr img;
    char *iname, *oname;
    
    int color, x, y, w, h,i=0;
    int avgColor;
    color = x = y = w = h = 0;
    
    if(argc != 3) {
        printf("need 3 arguments\n");
        exit(0);
    }
    else {
        iname = argv[1];
        oname = argv[2];
    }
    
    if((fp = fopen(iname, "r")) == NULL) {
      printf("error opening file\n");
      exit(0);
    }    
    else
        img = gdImageCreateFromPng(fp);
        
    w = gdImageSX(img);
    h = gdImageSY(img);
    
    double t = omp_get_wtime();
    #pragma omp parallel for schedule(guided) private(x,y,color)
    for(x = 0; x < w; x++) {
        for(y = 0; y < h; y++) {
           
            color = gdImageGetPixel(img, x, y);
            avgColor = (gdImageRed(img, color) + gdImageGreen(img, color) + gdImageBlue(img, color))/3;
            color = gdImageColorAllocate(img, avgColor, avgColor, avgColor);
            #pragma omp critical
            gdImageSetPixel(img, x, y, color);
        }
    }
     t = omp_get_wtime() - t;
    if((fp = fopen(oname, "w")) == NULL){
      printf("error opening output file\n");
      exit(0);
    }
    else {
        gdImagePng(img, fp);
        fclose(fp);
    }
   
    gdImageDestroy(img);
    printf("Time taken = %g",t);
    return 0;
}
