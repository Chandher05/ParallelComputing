#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
long datasize;

cudaError_t searchKeyword(int *result, char *data, char *keyword);
__global__ void searchKeywordKernel(int *result, char *data, char *keyword,int datasize)
{
 int i = blockIdx.x*threadIdx.x;
 // Detect the first matching character
 if(i<datasize){

 if (data[i] == keyword[0]) {
   // Loop through next keyword character
   for (int j=1; j<3; i++) {
     if (data[i+j] != keyword[j])
       break;
     else
     // Store the first matching character to the result list
       result[i] = 1;
   }
  }
 }
}
int main()
{
    FILE *f = fopen("input.txt", "rb");
    fseek(f, 0, SEEK_END);
    datasize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);
    fclose(f);

     char data[datasize];
     fread(data, datasize, 1, f);
     // char keyword[8]={'S','H','E','R','L','O','C','K'};// char pattern
     char keyword[8]={'s'};
     int result[datasize];
     // Set false value in result array
     memset(result, 0, datasize);
     // Generate input data

    // Search keyword in parallel.
    cudaError_t cudaStatus = searchKeyword(result, data, keyword);
     
    // Print out the string match result position
    int total_matches = 0;
    for (int i=0; i<datasize; i++) {
      if (result[i] == 1) {
        printf("Character found at position % i\n", i);
        total_matches++;
      }
    }
    printf("Total matches = %d\n", total_matches);
    cudaStatus = cudaDeviceReset();
    
    system("pause");
    return 0;
     
    }
    // Helper function for using CUDA to search a list of characters in parallel.
cudaError_t searchKeyword(int *result, char *data, char *keyword)
{
     char *dev_data = 0;
     char *dev_keyword = 0;
     int *dev_result = 0;
     cudaError_t cudaStatus;
     cudaStatus = cudaSetDevice(0);
     cudaStatus = cudaMalloc((void**)&dev_result, datasize * sizeof(int));
     cudaStatus = cudaMalloc((void**)&dev_data, datasize * sizeof(char));
     cudaStatus = cudaMalloc((void**)&dev_keyword, datasize * sizeof(char));
     
     cudaStatus = cudaMemcpy(dev_data, data, datasize * sizeof(char), cudaMemcpyHostToDevice);
     cudaStatus = cudaMemcpy(dev_keyword, keyword, datasize * sizeof(char), cudaMemcpyHostToDevice);
      
      int bk = (int) (datasize/512);
      int gputhreads = 512;
      if (bk > 0) {
        gputhreads = 512;
      } 
      else{
        bk = 1;
        gputhreads = datasize;
      }
      searchKeywordKernel<<<bk,gputhreads >>>(dev_result, dev_data, dev_keyword,datasize);
      cudaStatus = cudaDeviceSynchronize();
      cudaStatus = cudaMemcpy(result, dev_result, datasize * sizeof(int), cudaMemcpyDeviceToHost);
      
        cudaFree(dev_result);
        cudaFree(dev_data);
        cudaFree(dev_keyword);
     
     return cudaStatus;
     }