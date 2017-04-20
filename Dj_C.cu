#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define inf 9999
#define N 1000





int main(int argc, char **argv) {

    struct timeval first, second, lapsed, third;
    struct timezone tzp, tzp2;
    float *host_A;
    int *host_Q;
    float *dev_x;
    int *dev_qx;
    float *A;
    int *Q;
    float *D;
    

    int i, j, bk;
    int k = 0;
    //int n = atoi(argv[1]);
    int n = N;

    cudaEvent_t start,stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float milliseconds = 0;

    printf("\n");
    printf("RUNNING WITH %d VERTICES \n", n);
    printf("\n");

    cudaMalloc(&dev_x, n * n * sizeof (float));
    cudaMalloc(&dev_qx, n * n * sizeof (float));

    //CPU arrays
    A = (float *) malloc(n * n * sizeof (float)); //arxikos pinakas A
    D = (float *) malloc(n * n * sizeof (float)); //arxikos pinakas D
    Q = (int *) malloc(n * n * sizeof (int)); //arxikos pinakas Q

    //GPU arrays
    host_A = (float *) malloc(n * n * sizeof (float));
    host_Q = (int *) malloc(n * n * sizeof (int));

    srand(time(NULL));
    
    gettimeofday(&third, &tzp2);
    ////////////////////////////First Mem Copy////////////////////
    gettimeofday(&first, &tzp);
    cudaMemcpy(dev_x, host_A, n * n * sizeof (float), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_qx, host_Q, n * n * sizeof (int), cudaMemcpyHostToDevice);
    gettimeofday(&second, &tzp);
    if (first.tv_usec > second.tv_usec) {
        second.tv_usec += 1000000;
        second.tv_sec--;
    }
    lapsed.tv_usec = second.tv_usec - first.tv_usec;
    lapsed.tv_sec = second.tv_sec - first.tv_sec;
    printf("First Transfer CPU to GPU  Time elapsed: %lu, %lu s\n", lapsed.tv_sec, lapsed.tv_usec);
    ////////////////////////////////////////////////////GPU Calculation////////////////////////////////

    bk = (int) (n * n / 512);
    int gputhreads = 512;
    if (bk > 0) {
        gputhreads = 512;
    } else {
        bk = 1;
        gputhreads = n*n;
    }
    printf(" \n");
    printf("BLOCKS :   %d      GPU THREADS:     %d \n", bk, gputhreads);
    printf(" \n");
    //gettimeofday(&first, &tzp);
    cudaEventRecord(start); 
    funct << <bk, gputhreads>>>(n, k, dev_x, dev_qx);
    cudaThreadSynchronize();
    //gettimeofday(&second, &tzp);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&milliseconds, start, stop);
    if (first.tv_usec > second.tv_usec) {
        second.tv_usec += 1000000;
        second.tv_sec--;
    }

    lapsed.tv_usec = second.tv_usec - first.tv_usec;
    lapsed.tv_sec = second.tv_sec - first.tv_sec;
    printf("GPU Calculation Time elapsed: %.20f\n", milliseconds * .0001);
    printf("\n");

    
    printf("ALL OK WE ARE DONE \n");
    return 0;
}