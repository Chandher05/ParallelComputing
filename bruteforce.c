#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

double time_diff(struct timespec *st, struct timespec *en);
void printMatrix(int n,double *a[n]);

int main()
{
	int N;
	scanf("%d", &N);
	struct timespec st, en;
	//create two matrices of NxN
	double * * a = (double * *) malloc(sizeof(double *) * N); //matrix one
	double * * b = (double * *) malloc(sizeof(double *) * N); //matric 2
	double * * c = (double * *) malloc(sizeof(double *) * N); //product matrix
	int i = 0;	
	for(i=0 ; i< N;i++)
	{
		a[i] = (double *) malloc(sizeof(double) * N);
		b[i] = (double *) malloc(sizeof(double) * N);
		c[i] = (double *) malloc(sizeof(double) * N);
		
	}
	int j;
	//Taking input
	for(i =0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			scanf("%lf",&a[i][j]);
		}
	}
	for(i =0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			scanf("%lf",&b[i][j]);
		}
	}
	clock_gettime(CLOCK_REALTIME, &st);
	//Brute force Method O(n^3)
	int k;
	for(i=0;i<N;i++) //row index
	{
		for(j=0;j<N;j++) //column index
		{
			double sum = 0;
			for(k=0;k<N;k++) //point index
			{
				sum  += (a[i][k] * b[k][j]);
			}
			c[i][j] = sum;
		}
	} 
	//Brute force done
	clock_gettime(CLOCK_REALTIME, &en);
	printMatrix(N,c); //print the matrix
	printf("\nExecution Time : Brute Force: %.6lf seconds\n", time_diff(&st, &en)/1000);
	for(i=0;i<N;i++)
	{
		free(a[i]);
		free(b[i]);
		free(c[i]);
		
	}
	free(a);
	free(b);
	free(c);
}
void printMatrix(int n,double *a[n])
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%.4lf ",a[i][j]);
		}
		printf("\n");
	}
	return;
}
double time_diff(struct timespec *st, struct timespec *en)
{
	double i = (en->tv_sec - st->tv_sec) * 1000;
	i += (en->tv_nsec - st->tv_nsec) * 0.000001;
	return i;
}
