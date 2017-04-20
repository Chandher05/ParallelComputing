#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

struct para
{
	int n;
	double ** a;
	double ** b;
	double ** c;
};
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
double ** add(int n, double * a[n],double * b[n])
{
	double ** c = (double **) malloc(n * sizeof(double *));
	int i,j;
	for(i=0;i<n;i++)
	{
		c[i] = (double *) malloc(n * sizeof(double));
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			c[i][j] = a[i][j] + b[i][j];
		}
	}
	return c;
}
double ** sub(int n, double * a[n],double * b[n])
{
	double ** c = (double **) malloc(n * sizeof(double *));
	int i,j;
	for(i=0;i<n;i++)
	{
		c[i] = (double *) malloc(n * sizeof(double));
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			c[i][j] = a[i][j] - b[i][j];
		}
	}
	return c;
}void strassenThread(void * ptr)
{
	struct para * p = (struct para *) ptr;
	strassen(p->n,p->a,p->b,p->c);
	return;
}

int main()
{
	int N;
	scanf("%d", &N);
	struct para p[7];
	pthread_t t1,t2,t3,t4,t5,t6,t7;
	struct timespec st, en;
	//create two matrices of NxN
	double * * a = (double * *) malloc(sizeof(double *) * N); //matrix one
	double * * b = (double * *) malloc(sizeof(double *) * N); //matric 2
	double * * c = (double * *) malloc(sizeof(double *) * N); //product matrix
	int i = 0,j;	
	for(i=0 ; i< N;i++)
	{
		a[i] = (double *) malloc(sizeof(double) * N);
		b[i] = (double *) malloc(sizeof(double) * N);
		c[i] = (double *) malloc(sizeof(double) * N);
		
	}
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
	int n = N;
	//ALL quadrants
	double ** a11 = (double **) malloc((n/2) * sizeof(double *));
	double ** a12 = (double **) malloc((n/2) * sizeof(double *));
	double ** a21 = (double **) malloc((n/2) * sizeof(double *));
	double ** a22 = (double **) malloc((n/2) * sizeof(double *));
	double ** b11 = (double **) malloc((n/2) * sizeof(double *));
	double ** b12 = (double **) malloc((n/2) * sizeof(double *));
	double ** b21 = (double **) malloc((n/2) * sizeof(double *));
	double ** b22 = (double **) malloc((n/2) * sizeof(double *));
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
		a11[i] = (double *) malloc((n/2) * sizeof(double));
		a12[i] = (double *) malloc((n/2) * sizeof(double));
		a21[i] = (double *) malloc((n/2) * sizeof(double));
		a22[i] = (double *) malloc((n/2) * sizeof(double));
		b11[i] = (double *) malloc((n/2) * sizeof(double));
		b12[i] = (double *) malloc((n/2) * sizeof(double));
		b21[i] = (double *) malloc((n/2) * sizeof(double));
		b22[i] = (double *) malloc((n/2) * sizeof(double));
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
			a11[i][j] = a[i][j];
			a12[i][j] = a[i][j+n/2];
			a21[i][j] = a[i+n/2][j];
			a22[i][j] = a[i+n/2][j+n/2];
			b11[i][j] = b[i][j];
			b12[i][j] = b[i][j+n/2];
			b21[i][j] = b[i+n/2][j];
			b22[i][j] = b[i+n/2][j+n/2];
		}
	}
	clock_gettime(CLOCK_REALTIME, &st);
	//strassen(n/2,v1,v2,p1); //p1 =A11x B12-B22
	p[0].n = n/2;
	p[0].a = a11;
	p[0].b = sub(n/2,b12,b22);
	p[0].c = p1;

	pthread_create(&t1,NULL,(void *)strassenThread,(void *)&p[0]);
	//strassen(n/2,v1,v2,p2); //p2=A11+A12 xB22
	p[1].n = n/2;
	p[1].a = add(n/2,a11,a12);
	p[1].b = b22;
	p[1].c = p2;	

	pthread_create(&t2,NULL,(void *)strassenThread,(void *)&p[1]); 
	//strassen(n/2,v1,v2,p3); //p3=A21+A22 xB11
	p[2].n = n/2;
	p[2].a = add(n/2,a21,a22);
	p[2].b = b11;
	p[2].c = p3;	
	
	pthread_create(&t3,NULL,(void *)strassenThread,(void *)&p[2]);
	//strassen(n/2,v1,v2,p5); //p5=A11+A22 x B11+B22
	p[5].n = n/2;
	p[5].a = add(n/2,a11,a22);
	p[5].b = add(n/2,b11,b22);
	p[5].c = p5;

	pthread_create(&t5,NULL,(void *)strassenThread,(void *)&p[5]);
	//strassen(n/2,v1,v2,p7); //p7=A11-A21 x B11+B12
	p[6].n = n/2;
	p[6].a = sub(n/2,a11,a21);
	p[6].b = add(n/2,b11,b12);
	p[6].c = p7;

	pthread_create(&t7,NULL,(void *)strassenThread,(void *)&p[6]);
	//strassen(n/2,v1,v2,p4); //p4=A22 x B21-B11
	p[3].n = n/2;
	p[3].a = a22;
	p[3].b = sub(n/2,b21,b11);
	p[3].c = p4;	

	pthread_create(&t4,NULL,(void *)strassenThread,(void *)&p[3]);
	//strassen(n/2,v1,v2,p6); //p6=A12-A22 x B21+B22
	p[4].n = n/2;
	p[4].a = sub(n/2,a12,a22);
	p[4].b = add(n/2,b21,b22);
	p[4].c = p6;	
		
	pthread_create(&t6,NULL,(void *)strassenThread,(void *)&p[4]); 
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_join(t5, NULL);
	pthread_join(t6, NULL);
	pthread_join(t7, NULL);	
	//C12
	for(i=0;i<n/2;i++)
	{
		for(j=0;j<n/2;j++)
		{
			c[i][j+n/2] = p1[i][j] + p2[i][j];
		}
	}
	//C22
	for(i=0;i<n/2;i++)
	{
		for(j=0;j<n/2;j++)
		{
			c[i+n/2][j+n/2] = p1[i][j] + p5[i][j] - p3[i][j] - p7[i][j];
		}
	}
	matrixFree(n/2,p1);
	matrixFree(n/2,p7);
	//C21
	for(i=0;i<n/2;i++)
	{
		for(j=0;j<n/2;j++)
		{
			c[i+n/2][j] = p3[i][j] + p4[i][j];
		}
	}
	matrixFree(n/2,p3);
	//C11
	for(i=0;i<n/2;i++)
	{
		for(j=0;j<n/2;j++)
		{
			c[i][j] = p5[i][j] + p4[i][j] - p2[i][j] + p6[i][j];
		}
	}
	matrixFree(n/2,p5);
	matrixFree(n/2,p4);
	matrixFree(n/2,p2);
	matrixFree(n/2,p6);

	clock_gettime(CLOCK_REALTIME, &en);

	printMatrix(N,c); //print the matrix
	printf("\nExecution Time : Strassen's : %.6lf seconds\n", time_diff(&st, &en)/1000);
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