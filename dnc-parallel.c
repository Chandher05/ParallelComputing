#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h> 
#include <errno.h>


void DNC(int N,int n, double *a[N],double *b[N],double *c[n],int ia,int ja,int ib,int jb);
void DNCrecursion(int N,int n, double *a[N],double *b[N],double *c[n],int ia,int ja,int ib,int jb);

struct para
{
	int N;
	int n;
	double ** a;
	double ** b;
	double ** c;
	int ia;
	int ja;
	int ib;
	int jb;
	int tn;

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


void DNCthread(void  * ptr)
{
	struct para * p = (struct para *) ptr;
	DNCrecursion(p->N,p->n,p->a,p->b,p->c,p->ia,p->ja,p->ib,p->jb);
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
		pthread_t t1,t2,t3,t4,t5,t6,t7,t8;
		//ALL 8 products
		struct para p[8];
		double ** c1 = (double **) malloc((n/2) * sizeof(double *));
		double ** c2 = (double **) malloc((n/2) * sizeof(double *));
		double ** c3 = (double **) malloc((n/2) * sizeof(double *));
		double ** c4 = (double **) malloc((n/2) * sizeof(double *));
		double ** c5 = (double **) malloc((n/2) * sizeof(double *));
		double ** c6 = (double **) malloc((n/2) * sizeof(double *));
		double ** c7 = (double **) malloc((n/2) * sizeof(double *));
		double ** c8 = (double **) malloc((n/2) * sizeof(double *));

		for(i=0;i<n/2;i++)
		{
			c1[i] = (double *) malloc((n/2) * sizeof(double));
			c2[i] = (double *) malloc((n/2) * sizeof(double));
			c3[i] = (double *) malloc((n/2) * sizeof(double));
			c4[i] = (double *) malloc((n/2) * sizeof(double));
			c5[i] = (double *) malloc((n/2) * sizeof(double));
			c6[i] = (double *) malloc((n/2) * sizeof(double));
			c7[i] = (double *) malloc((n/2) * sizeof(double));
			c8[i] = (double *) malloc((n/2) * sizeof(double));
		}

		p[0].N = N;
		p[0].n = n/2;
		p[0].a = a;
		p[0].b = b;
		p[0].c = c1;	
		p[0].ia = ia;
		p[0].ja = ja-n/2;
		p[0].ib = ib;
		p[0].jb = jb-n/2;
		p[0].tn = 1;

		p[1].N = N;
		p[1].n = n/2;
		p[1].a = a;
		p[1].b = b;
		p[1].c = c2;	
		p[1].ia = ia;
		p[1].ja = ja;
		p[1].ib = ib+n/2;
		p[1].jb = jb-n/2;
		p[1].tn = 2;

		p[2].N = N;
		p[2].n = n/2;
		p[2].a = a;
		p[2].b = b;
		p[2].c = c3;	
		p[2].ia = ia;
		p[2].ja = ja-n/2;
		p[2].ib = ib;
		p[2].jb = jb;
		p[2].tn = 3;
		
		p[3].N = N;
		p[3].n = n/2;
		p[3].a = a;
		p[3].b = b;
		p[3].c = c4;	
		p[3].ia = ia;
		p[3].ja = ja;
		p[3].ib = ib+n/2;
		p[3].jb = jb;
		p[3].tn = 4;

		p[4].N = N;
		p[4].n = n/2;
		p[4].a = a;
		p[4].b = b;
		p[4].c = c5;	
		p[4].ia = ia+n/2;
		p[4].ja = ja-n/2;
		p[4].ib = ib;
		p[4].jb = jb-n/2;
		p[4].tn = 5;
		
		p[5].N = N;
		p[5].n = n/2;
		p[5].a = a;
		p[5].b = b;
		p[5].c = c6;	
		p[5].ia = ia+n/2;
		p[5].ja = ja;
		p[5].ib = ib+n/2;
		p[5].jb = jb-n/2;
		p[5].tn = 6;

		p[6].N = N;
		p[6].n = n/2;
		p[6].a = a;
		p[6].b = b;
		p[6].c = c7;	
		p[6].ia = ia+n/2;
		p[6].ja = ja-n/2;
		p[6].ib = ib;
		p[6].jb = jb;
		p[6].tn = 7;
		
		p[7].N = N;
		p[7].n = n/2;
		p[7].a = a;
		p[7].b = b;
		p[7].c = c8;	
		p[7].ia = ia+n/2;
		p[7].ja = ja;
		p[7].ib = ib+n/2;
		p[7].jb = jb;
		p[7].tn = 8;

		pthread_create(&t1,NULL,(void *)DNCthread,(void *)&p[0]);
		pthread_create(&t2,NULL,(void *)DNCthread,(void *)&p[1]);
		pthread_create(&t3,NULL,(void *)DNCthread,(void *)&p[2]);
		pthread_create(&t4,NULL,(void *)DNCthread,(void *)&p[3]);
		pthread_create(&t5,NULL,(void *)DNCthread,(void *)&p[4]);
		pthread_create(&t6,NULL,(void *)DNCthread,(void *)&p[5]);
		pthread_create(&t7,NULL,(void *)DNCthread,(void *)&p[6]);
		pthread_create(&t8,NULL,(void *)DNCthread,(void *)&p[7]);

		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
		pthread_join(t3, NULL);
		pthread_join(t4, NULL);
		pthread_join(t5, NULL);
		pthread_join(t6, NULL);
		pthread_join(t7, NULL);
		pthread_join(t8, NULL);

		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				c[i][j] = c1[i][j] + c2[i][j];
				c[i][j+n/2] = c3[i][j] + c4[i][j];
				c[i+n/2][j] = c5[i][j] + c6[i][j];
				c[i+n/2][j+n/2] = c7[i][j] + c8[i][j];
			}
		}

		matrixFree(n/2,c1,c2);
		matrixFree(n/2,c3,c4);
		matrixFree(n/2,c5,c6);
		matrixFree(n/2,c7,c8);
		return;

	}
}

void DNCrecursion(int N,int n, double *a[N],double *b[N],double *c[n],int ia,int ja,int ib,int jb)
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

		DNCrecursion(N,n/2,a,b,c1, ia , ja-n/2 , ib , jb-n/2); 			//c1=A11xB11
		DNCrecursion(N,n/2,a,b,c2, ia , ja , ib+n/2 , jb-n/2); 			//c2=A12xB21
		//C11
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				c[i][j] = c1[i][j] + c2[i][j];
			}
		}
		
		DNCrecursion(N,n/2,a,b,c1, ia , ja-n/2 , ib , jb); 				//c3=A11xB12
		DNCrecursion(N,n/2,a,b,c2, ia , ja , ib+n/2 , jb);				//c4=A12xB22
		//C12
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				c[i][j+n/2] = c1[i][j] + c2[i][j];
			}
		}

		DNCrecursion(N,n/2,a,b,c1, ia+n/2 , ja-n/2 , ib , jb-n/2); 		//c5=A21xB11
		DNCrecursion(N,n/2,a,b,c2, ia+n/2 , ja , ib+n/2 , jb-n/2); 		//c6=A22xB21
		//C21
		for(i=0;i<n/2;i++)
		{
			for(j=0;j<n/2;j++)
			{
				c[i+n/2][j] = c1[i][j] + c2[i][j];
			}
		}

		DNCrecursion(N,n/2,a,b,c1, ia+n/2 , ja-n/2 , ib , jb); 			//c7=A21xB12
		DNCrecursion(N,n/2,a,b,c2, ia+n/2 , ja , ib+n/2 , jb); 			//c8=A22xB22
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
	printf("\nExecution Time : Divide N Conquer - Parallel: %.6lf seconds\n", time_diff(&st, &en)/1000);
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