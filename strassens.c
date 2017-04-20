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
void matrixFree(int n,double *m1[n])
{
	int i;
	for(i = 0; i< n;i++)
	{
		free(m1[i]);
	}
	free(m1);
	return;
}
void strassen(int n,double *a[n],double *b[n],double *p[n])
{
	if(n == 1)
	{
		p[0][0] = a[0][0] * b[0][0];
		return;
	}
	else
	{
		int i,j;
		//Values
		double ** v1 = (double **) malloc((n/2) * sizeof(double *));
		double ** v2 = (double **) malloc((n/2) * sizeof(double *));
		//ALL 8 products
		double ** p1 = (double **) malloc((n/2) * sizeof(double *));
		double ** p2 = (double **) malloc((n/2) * sizeof(double *));
		double ** p3 = (double **) malloc((n/2) * sizeof(double *));
		double ** p4 = (double **) malloc((n/2) * sizeof(double *));
		double ** p5 = (double **) malloc((n/2) * sizeof(double *));
		double ** p6 = (double **) malloc((n/2) * sizeof(double *));
		double ** p7 = (double **) malloc((n/2) * sizeof(double *));
		for(i=0;i<n/2;i++)
		{
			v1[i] = (double *) malloc((n/2) * sizeof(double));
			v2[i] = (double *) malloc((n/2) * sizeof(double));

			p1[i] = (double *) malloc((n/2) * sizeof(double));
			p2[i] = (double *) malloc((n/2) * sizeof(double));
			p3[i] = (double *) malloc((n/2) * sizeof(double));
			p4[i] = (double *) malloc((n/2) * sizeof(double));
			p5[i] = (double *) malloc((n/2) * sizeof(double));
			p6[i] = (double *) malloc((n/2) * sizeof(double));
			p7[i] = (double *) malloc((n/2) * sizeof(double));
		}

		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				v1[i][j] = a[i][j]; //A11
				v2[i][j] = b[i][j+n/2] - b[i+n/2][j+n/2]; //B12 - B22
			}
		}
		strassen(n/2,v1,v2,p1); //p1 =A11x B12-B22

		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				v1[i][j] = a[i][j] + a[i][j+n/2]; //A11+A12 
				v2[i][j] = b[i+n/2][j+n/2]; //B22
			}
		}
		strassen(n/2,v1,v2,p2); //p2=A11+A12 xB22

		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				v1[i][j] = a[i+n/2][j] + a[i+n/2][j+n/2]; //A21+A22
				v2[i][j] = b[i][j]; //B11
			}
		}
		strassen(n/2,v1,v2,p3); //p3=A21+A22 xB11

		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				v1[i][j] = a[i][j] + a[i+n/2][j+n/2]; //A11+A22
				v2[i][j] = b[i][j] + b[i+n/2][j+n/2]; //B11+B22
			}
		}
		strassen(n/2,v1,v2,p5); //p5=A11+A22 x B11+B22

		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				v1[i][j] = a[i][j] - a[i+n/2][j]; //A11 - A21
				v2[i][j] = b[i][j] + b[i][j+n/2]; //B11 + B12
			}
		}
		strassen(n/2,v1,v2,p7); //p7=A11-A21 x B11+B12
		//C12
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				p[i][j+n/2] = p1[i][j] + p2[i][j];
			}
		}
		//C22
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				p[i+n/2][j+n/2] = p1[i][j] + p5[i][j] - p3[i][j] - p7[i][j];
			}
		}
		matrixFree(n/2,p1);
		matrixFree(n/2,p7);

		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				v1[i][j] = a[i+n/2][j+n/2]; //A22
				v2[i][j] = b[i+n/2][j]- b[i][j]; //B21 - B11
 			}
		}
		strassen(n/2,v1,v2,p4); //p4=A22 x B21-B11
		//C21
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				p[i+n/2][j] = p3[i][j] + p4[i][j];
			}
		}
		matrixFree(n/2,p3);

		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				v1[i][j] = a[i][j+n/2] - a[i+n/2][j+n/2];
				v2[i][j] = b[i+n/2][j] + b[i+n/2][j+n/2];
			}
		}
		strassen(n/2,v1,v2,p6); //p6=A12-A22 x B21+B22
		//C11
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				p[i][j] = p5[i][j] + p4[i][j] - p2[i][j] + p6[i][j];
			}
		}
		matrixFree(n/2,v1);
		matrixFree(n/2,v2);
		matrixFree(n/2,p5);
		matrixFree(n/2,p4);
		matrixFree(n/2,p2);
		matrixFree(n/2,p6);
		return;

	}
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
	strassen(N,a,b,c);
	//Divide N Conquer End
	clock_gettime(CLOCK_REALTIME, &en);
	printMatrix(N,c); //print the matrix
	printf("\nExecution Time : Strassen's : %.6lf microseconds\n", time_diff(&st, &en));
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