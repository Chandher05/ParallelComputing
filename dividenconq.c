#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
void matrixFree(int n,double *m1[n], double *m2[n])
{
	int i;
	for(i = 0; i< n;i++)
	{
		free(m1[i]);
		free(m2[i]);
	}
	free(m1);
	free(m2);
	return;
}

void DNC(int N,int n, double *a[N],double *b[N],double *c[n],int ia,int ja,int ib,int jb)
{
	if(n == 1)
	{
		c[0][0] = a[ia][ja] * b[ib][jb]; //1x1 = 1x1 * 1x1 => c = a*b 
		return; 
	}
	else
	{
		int i,j;
		//ALL 8 products
		double ** c1 = (double **) malloc((n/2) * sizeof(double *));
		double ** c2 = (double **) malloc((n/2) * sizeof(double *));
		for(i=0;i<n/2;i++)
		{
			c1[i] = (double *) malloc((n/2) * sizeof(double));
			c2[i] = (double *) malloc((n/2) * sizeof(double));
		}

		DNC(N,n/2,a,b,c1, ia , ja-n/2 , ib , jb-n/2); 			//c1=A11xB11
		DNC(N,n/2,a,b,c2, ia , ja , ib+n/2 , jb-n/2); 			//c2=A12xB21
		//C11
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				c[i][j] = c1[i][j] + c2[i][j];
			}
		}
		
		DNC(N,n/2,a,b,c1, ia , ja-n/2 , ib , jb); 				//c3=A11xB12
		DNC(N,n/2,a,b,c2, ia , ja , ib+n/2 , jb);				//c4=A12xB22
		//C12
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				c[i][j+n/2] = c1[i][j] + c2[i][j];
			}
		}

		DNC(N,n/2,a,b,c1, ia+n/2 , ja-n/2 , ib , jb-n/2); 		//c5=A21xB11
		DNC(N,n/2,a,b,c2, ia+n/2 , ja , ib+n/2 , jb-n/2); 		//c6=A22xB21
		//C21
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				c[i+n/2][j] = c1[i][j] + c2[i][j];
			}
		}

		DNC(N,n/2,a,b,c1, ia+n/2 , ja-n/2 , ib , jb); 			//c7=A21xB12
		DNC(N,n/2,a,b,c2, ia+n/2 , ja , ib+n/2 , jb); 			//c8=A22xB22
		//C22
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				c[i+n/2][j+n/2] = c1[i][j] + c2[i][j];
			}
		}
		matrixFree(n/2,c1,c2);
		return;

	}
}

void DNCmain(int n, double *a[n],double *b[n],double *c[n])
{
	DNC(n,n,a,b,c,0,n-1,0,n-1);
	return;
}

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
	//Divide N Conquer Start
	DNCmain(N,a,b,c);
	//Divide N Conquer End
	clock_gettime(CLOCK_REALTIME, &en);
	printMatrix(N,c); //print the matrix
	printf("\nExecution Time : Divide N Conquer: %.6lf microseconds\n", time_diff(&st, &en));
	for(i=0 ; i< N;i++)
	{
		free(a[i]);
		free(b[i]);
		free(c[i]);
		
	}
	free(a);
	free(b);
	free(c);
}
