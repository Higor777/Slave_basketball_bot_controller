#include<math.h>
#define DF float
#define NULL 0
//float* C,aa[100];
//#include "Include.h"

//-------------------------------------------//
//函数名称：MatrixAdd
//函数功能：矩阵加法
//函数描述：
//参数说明：
//返回值：
//创建时间：2004.05.18
//修改时间：2004.05.28
//测试时间：2004.07.28
//测试方法：功能测试
//-------------------------------------------//
/*
void MatrixAdd( float* fMatrixA,float* fMatrixB,float* Result,
		unsigned int m,unsigned int n )
{
	unsigned int index_i = 0;
	unsigned int index_j = 0;
	unsigned int itemp = 0;

	for (;index_i<m;index_i++)
		for (index_j=0;index_j<n;index_j++)
		{
			itemp = index_i*n+index_j;
			*(Result+itemp) = *(fMatrixA+itemp) + *(fMatrixB+itemp);
		}
} 	*///void MatrixAdd

//-------------------------------------------//
//函数名称：MatrixSub
//函数功能：矩阵减法
//函数描述:
//参数说明:
//返回值：
//创建时间：2004.05.18
//修改时间：2004.05.28
//测试时间：2004.07.28
//测试方法：功能测试
//-------------------------------------------//
/**/
void MatrixSub( float* fMatrixA,float* fMatrixB,float* Result,
		unsigned int m,unsigned int n )
{
	unsigned int index_i = 0;
	unsigned int index_j = 0;
	unsigned int itemp = 0;
	for (index_i=0;index_i<m;index_i++)
		for (index_j=0;index_j<n;index_j++)
		{
			itemp = index_i*n+index_j;
			*(Result+itemp) = *(fMatrixA+itemp) - *(fMatrixB+itemp);
		}
}	
//void MatrixSub

//-------------------------------------------//
//函数名称：MatrixMultiply
//函数功能：矩阵乘法
//函数描述：
//参数说明：
//返回值：
//创建时间：2004.05.18
//修改时间：2004.05.28(改为指针操作)、2004.06.02(加入乘前的判断条件)
//测试时间：2004.07.28
//测试方法：功能测试
//-------------------------------------------//
void MatrixMultiply( 	float* fMatrixA,unsigned int uRowA,unsigned int uColA,
			float* fMatrixB,unsigned int uRowB,unsigned int uColB,
			float* MatrixResult )
{
	unsigned int index_i = 0;
	unsigned int index_j = 0;
	unsigned int index_l = 0;
	unsigned int index_u = 0;
	unsigned int index_k = 0;
	unsigned int index_v = 0;
	uRowB = uRowB;
	for(index_i=0;index_i<uRowA;index_i++)
		for(index_j=0;index_j<uColB;index_j++)
		{
			index_u = index_i*uColB + index_j;
			MatrixResult[index_u] = 0.0;
			for(index_l=0;index_l<uColA;index_l++)
			{
				index_k = index_i*uColA+index_l;
				index_v = index_l*uColB+index_j;

				if ((((fMatrixA[index_k]))!=0.0f) && (((fMatrixB[index_v]))!=0.0f))
					(MatrixResult[index_u]) += ((fMatrixA[index_k])) * ((fMatrixB[index_v]));

			}
		 }
}	

//-------------------------------------------//
//函数名称：MatrixTranspose
//函数功能：矩阵转置
//函数描述：
//参数说明：
//返回值：
//创建时间：2004.05.18
//修改时间?2004.05.28?//测试时间：2004.07.28
//测试方法：功能测试
//-------------------------------------------//
void MatrixTranspose(float* fMatrixA,unsigned int m,unsigned n,float* fMatrixB)
{
	unsigned int index_i = 0;
	unsigned int index_j = 0;
	unsigned int index_u = 0;
	unsigned int index_v = 0;

	for (index_i=0;index_i<m;index_i++)
		for (index_j=0;index_j<n;index_j++)
		{
			index_u = index_j*m+index_i;
			index_v = index_i*n+index_j;
			fMatrixB[index_u] = fMatrixA[index_v];
		}
}	//MatrixTranspose

//-------------------------------------------//
//函数名称：MatrixE
//函数功能：单位矩阵生成?//函数描述：
//参数说明：
//返回值：
//创建时间：2004.05.19
//修改时间：
//测试时间：2004.07.28
//测试方法：功能测试
//-------------------------------------------//
/*
void MatrixE(float* fMatrixA,unsigned int n)
{
	unsigned int index_i = 0;
	unsigned int index_j = 0;

	for (;index_i<n;index_i++)
		for (index_j=0;index_j<n;index_j++)
		{
			if (index_i==index_j)
				*(fMatrixA+index_i*n+index_j) = 1;
			else
				*(fMatrixA+index_i*n+index_j) = 0;
		}
}	*///MatrixE


//求矩阵的行列式值
//全选主元高斯消去法
//a-长度为n*n的数组
//n-矩阵的阶数
void dhdet(float *a,int n,float det)
{
int i,j,k,is,js,l,u,v;
    float f,q,d;
    f=1.0; det=1.0;
    for (k=0; k<=n-2; k++)
{
  q=0.0;
        for (i=k; i<=n-1; i++)
   for (j=k; j<=n-1; j++)
   {
    l=i*n+j; d=fabs(a[l]);
    if (d>q)
    {
     q=d;
     is=i;
     js=j;
    }
   }
   if (q+1.0f==1.0f)
   {
    det=0.0;
   
   }
   if (is!=k)
   {
    f=-f;
    for (j=k; j<=n-1; j++)
    {
     u=k*n+j; v=is*n+j;
     d=a[u]; a[u]=a[v]; a[v]=d;
    }
   }
   if (js!=k)
   {
    f=-f;
    for (i=k; i<=n-1; i++)
    {
     u=i*n+js; v=i*n+k;
     d=a[u]; a[u]=a[v]; a[v]=d;
    }
   }
   l=k*n+k;
   det=det*a[l];
   for (i=k+1; i<=n-1; i++)
   {
    d=a[i*n+k]/a[l];
    for (j=k+1; j<=n-1; j++)
    {
     u=i*n+j;
     a[u]=a[u]-d*a[k*n+j];
    }
   }
}
    det=f*det*a[n*n-1];
    
} 
//-------------------------------------------//
//函数名称：MatrixDet2
//函数功能：2阶矩阵行列式的值
//函数描述：
//参数说明：
//返回值：
//创建时间：2004.05.19
//修改时间：
//测试时间：
//测试方法：
//-------------------------------------------//
double MatrixDet2(float* fMatrixA)
{
	return (*fMatrixA)*(*(fMatrixA+3))-(*(fMatrixA+1))*(*(fMatrixA+2));
}	//MatrixDet2

//-------------------------------------------//
//函数名称：MatrixInverse2
//函数功能：2阶矩阵求逆
//函数描述：
//参数说明
//返回值：
//创建时间：2004.05.18
//修改时间：
//测试时间:
//测试方法：
//-------------------------------------------//
/*
int MatrixInverse2(float* fMatrixA,float* fMatrixB)
{
	float fmatrix_vlaue = 0.0;

	fmatrix_vlaue = MatrixDet2(fMatrixA);

	if (fmatrix_vlaue == 0.0)
	{
		#ifdef _DEBUG_
		printf("The inverse of fMatrixA is not exist!!!");
		#endif
		return 1;
	}

	*fMatrixB = *(fMatrixA+3)/fmatrix_vlaue;
	*(fMatrixB+1) = -(*(fMatrixA+2)/fmatrix_vlaue);
	*(fMatrixB+2) = -(*(fMatrixA+1)/fmatrix_vlaue);
	*(fMatrixB+3) = -(*(fMatrixA)/fmatrix_vlaue);
	return 0;
}	*///MatrixInverse2

//-------------------------------------------//
//函数名称：
//函数功能：矩阵求逆
//函数描述：
//参数说明：
//返回值：
//创建时间：004.05.18
//修改时间：?//测试时间：2004.07.28
//测试方法：功能测试
//-------------------------------------------//
int MatrixInverse(float* fMatrixA,int n,float ep)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int u = 0;
	int v = 0;
	int temp_row[10],temp_col[10];
	int * prow = temp_row,urow = 0;		//找到的最大值的行数
	int * pcol = temp_col,ucol = 0;			//找到的最大值的列数	
	float ftemp_max = 0.0;
	float ftemp = 0.0;

	ep = ep;					//防止ep没有使用的警告

	for (i=0;i<10;i++)
	{
		temp_row[i] = 0;
		temp_col[i] = 0;
	}		

	for (k=0;k<n;k++)
	{
		ftemp_max =0.0;		//找到剩余矩阵项中的最大值
		for (i=k;i<n;i++)
			for (j=k;j<n;j++)
			{
				l = i*n+j;
				ftemp = fabs(*(fMatrixA+l));
				if (ftemp_max<ftemp)
				{
					ftemp_max = ftemp;
					*(prow+k) = urow = i;
					*(pcol+k) = ucol= j;
				}
			}


		if (urow!=k)			//如果当前行不包含最大值换行
		{
			for (j=0;j<n;j++)
			{
				u = k*n +j;
				v = urow*n+j;
				ftemp = *(fMatrixA+u);
				*(fMatrixA+u) = *(fMatrixA+v);
				*(fMatrixA+v) = ftemp;
			}
		}

		if (ucol!=k)			//如果当前列不包含最大值换列
		{
			for (i=0;i<n;i++)
			{
				u = i*n+k;
				v = i*n+ucol;
				ftemp = *(fMatrixA+u);
				*(fMatrixA+u) = *(fMatrixA+v);
				*(fMatrixA+v) = ftemp;
			}
		}

		l = k*n+k;
		ftemp = *(fMatrixA+l) = 1.0f/(*(fMatrixA+l));

		for (j=0;j<n;j++)
		{
			if (j!=k)
			{
				u = k*n+j;
				*(fMatrixA+u) *= *(fMatrixA+l);
			}
		}

		for (i=0;i<n;i++)			//还不知道为什么
		{
			if (i!=k)
			{
				for (j=0;j<n;j++)
				{
					if (j!=k)
					{
						u = i*n+j;
						*(fMatrixA+u) -= (*(fMatrixA+i*n+k))*(*(fMatrixA+k*n+j));
					}
				}
			}
		}

		for (i=0;i<n;i++)
		{
			if (i!=k)
			{
				u = i*n+k;
				*(fMatrixA+u) = -(*(fMatrixA+u))*(*(fMatrixA+l));
			}
		}
	}

	for (k=n-1;k>=0;k--)
	{
		if ((*(pcol+k))!=k)
		{
			for (j=0;j<n;j++)
			{
				u = k*n+j;
				v = (*(pcol+k))*n+j;
				ftemp = *(fMatrixA+u);
				*(fMatrixA+u) = *(fMatrixA+v);
				*(fMatrixA+v) = ftemp;
			}
		}

		if ((*(prow+k))!=k)
		{
			for (i=0;i<n;i++)
			{
				u = i*n+k;
				v = i*n+(*(prow+k));
				ftemp = *(fMatrixA+u);
				*(fMatrixA+u) = *(fMatrixA+v);
				*(fMatrixA+v) = ftemp;
			}
		}
	}
	return 1;
}	//MatrixInverse2

//-------------------------------------------//
//函数名称：Norm
//函数功能：求矩阵范数
//函数描述：||A||
//参数说明：
//返回值：
//创建时间：2004.07.28
//修改时间：?//测试时间：2004.07.28
//测试方法：功能测试
//-------------------------------------------//
/*
float Norm(float *fMatrixA,int iRow,int iCol)
{
	int i = 0;
	int j = 0;
	int index = 0;
	float local_result = 0.0;

	for (i=0;i<iRow;i++)
		for (j=0;j<iCol;j++)
		{
			index = i * iCol +j;
			local_result += (*(fMatrixA+index)) * (*(fMatrixA+index));
		}

	local_result = sqrt(local_result);

	return local_result;
}*/
/*

float Surplus(float* A,int m,int n) //求矩阵行列式
{
int i,j,k,p,r;
float X,temp=1,temp1=1,s=0,s1=0;
if(n==2)
{for(i=0;i <m;i++)
for(j=0;j <n;j++)
if((i+j)%2) temp1*=A[i*n+j];
else temp*=A[i*n+j];
X=temp-temp1;}
else{
for(k=0;k <n;k++)
{for(i=0,j=k;i <m,j <n;i++,j++)
temp*=A[i*n+j];
if(m-i)
{for(p=m-i,r=m-1;p> 0;p--,r--)
temp*=A[r*n+p-1];}
s+=temp;
temp=1;
}
for(k=n-1;k>=0;k--)
{for(i=0,j=k;i<m,j>=0;i++,j--)
temp1*=A[i*n+j];
if(m-i)
{for(p=m-1,r=i;r <m;p--,r++)
temp1*=A[r*n+p];}
s1+=temp1;
temp1=1;
}
X=s-s1;}
return X;
}

float *MatrixOpp(float* A,int m,int n,float* C) //矩阵求逆
{
int i,j,x,y,k;
float *SP=NULL,*AB=NULL,*B=NULL,X; 
SP=(float *)malloc(m*n*sizeof(float));
AB=(float *)malloc(m*n*sizeof(float));
B=(float *)malloc(m*n*sizeof(float));
X=Surplus(A,m,n);
X=1/X;
for(i=0;i<m;i++) //求矩阵伴随矩阵
for(j=0;j<n;j++)
{
for(k=0;k<m*n;k++)
B[k]=A[k]; //B[]变成A[];
{
for(x=0;x<n;x++)
B[i*n+x]=0;
for(y=0;y<m;y++)
B[m*y+j]=0;
B[i*n+j]=1;
SP[i*n+j]=Surplus(B,m,n);
AB[i*n+j]=X*SP[i*n+j];
}
}
MatrixTranspose(AB,m,n,C); //MatrixInver 求倒置

}

*/
/*
///下面的cholesky分解可能有问题
void   cholesky(float* a,int n,float* L)   
{   
 int   i,j,m,k;   
 //float* L;   
 //L=(float *)malloc(n*sizeof(float));   
 //for(i=0;i<n;i++)   
  //L[i]=(float   *)malloc(n*sizeof(float));   
 for(i=0;i<n;i++)   
  for(j=0;j<n;j++)   
  L[i][j]=0;   
  for(k=0;k<n;k++)   
  {   
   L[k][k]=a[k][k];   
   for(m=0;m<k;m++)   
    L[k][k]-=L[k][m]*L[k][m];   
   L[k][k]=sqrt(L[k][k]);   
   for(i=k+1;i<n;i++)   
   {   
    L[i][k]=a[i][k];   
    for(m=0;m<k;m++)   
     L[i][k]-=L[i][m]*L[k][m];   
    L[i][k]/=L[k][k];   
   }   
  }   
  
}   
*/



/*******************************************************************/
/*********************把32位二进制数转换为浮点型数据*******************/
//************函数名：D_changeinto_F()
//**********入口参数：char D_data[4] 32位二进制放在此数组
//**** 31 30~~~~~~~~~~~~24 23~~~~~~~~~~~0
//***符号位（0：正数，1：负数） 整数位（8位） 小数位（24位）
//********函数返回值： 返回浮点型数据
//**IEEE754单精度浮点格式共32位，
//**包含三个构成字段：23位小数f，8位偏置指数e，1位符号s。
//将这些字段连续存放在一个32位字里，并对其进行编码。
//**其中0:22位包含23位的小数f； 23:30位包含8位指数e；第31位包含符号s。
// 一般地，32位字的第0位存放小数f的最低有效位LSB（the least significant bit），
//第22位存放小数f的最高有效位MSB（the most significant bit）；
//第23位存放偏置指数的最低有效位LSB，
//第30位存放偏置指数的最高有效位MSB；
//最高位，第31位存放符号s。
//**计算公式: FData=(-1)^S * (1 + F) * 2^(E - 127)
/*

*/
/*************************************************************************************************/
//**************把浮点型数据按照IEEE754单精度浮点格式（32位）存储D_data[4]数组中*****************/
/**/
int chol (double a[],int n, double *det)
{
	int i=0,j=0,k=0,u=0,l=0;
	double d=0.0;
	
	if((a[0]+1.0==1.0)||(a[0]<0.0))
	{
		//Uart_Printf(" FAIL 111\n");
		return(-2);
		//a[0]=0.01;
	}
	a[0]=sqrt(a[0]);
	d=a[0];
	for(i=1;i<=n-1;i++)
	{
		u=i*n;
		a[u]=a[u]/a[0];
	}
	for(j=1;j<=n-1;j++)
	{
		l=j*n+j;
		for(k=0;k<=j-1;k++)
		{
			u=j*n+k;
			a[l]=a[l]-a[u]*a[u];
		}
		
		if((a[l]+1.0==1.0)||(a[l]<0.0))
		{
			//Uart_Printf(" FAIL 222\n");
			//Uart_Printf(" a[%d]=%e\n",l,a[l]);
			return(-2);
			//a[l]=0.01;
		}
		a[l]=sqrt(a[l]);
		d=d*a[l];
		for(i=j+1;i<=n-1;i++)
		{
			u=i*n+j;
			for(k=0;k<=j-1;k++)
			{
				a[u]=a[u]-a[i*n+k]*a[j*n+k];
				
			}
			
			
			a[u]=a[u]/a[l];
			
		}
	}
	*det=d*d;
	for(i=0;i<=n-2;i++)
	 for(j=i+1;j<=n-1;j++)
	 {
	 	a[i*n+j]=0.0;
	 }
	return(2);

}

/*
void ukf_cholesky_decomposition(double A[], int n, double sigma[] )
{
        int i,j,k;
        double t;
		
        for(i = 0 ; i < n; i++)
        {
                if(i > 0)
                {
                        for(j = i; j < n; j++)
                        {
                                t = 0.0;
                                for(k = 0 ; k < i  ; k++)
                                        {t += A[j * n + k] * A[i * n + k];}

                                A[j * n + i] -= t;
                        }
                }

                if(A[i * n + i] <= 0.0)
                {
                      
                      if(A[i * n + i]==0)
                      {A[i * n + i]=0.01;}
                      A[i * n + i]=-A[i * n + i];
                       //Uart_Printf("Matrix is not positive definite\r\n");
                        //return;
                }

                sigma[i] = sqrt(A[i * n + i]);
                t = 1.0 / sigma[i];

                for(j = i ; j < n ; j++)
                        {A[j * n + i] *= t;}
                        
        }
        for(i = 0 ; i < n; i++)
        	for(j = 0 ; j < n; j++)
        	{
        		if(i<j)
        		{
        			A[i * n + j]=0.0;
        		}
        	}
}
 
void ukf_cholesky_invert(double A[], int n, double sigma[])
{
        double t;
        int i,j,k;

        for(i = 0 ; i < n ; i++)
        {
                A[i * n + i] = 1.0 / sigma[i];

                for(j = i + 1 ; j < n ; j++)
                {
                        t = 0.0;
                        for(k = i ; k < j ; k++)
                                t -= A[j * n + k] * A[k * n + i];

                        A[j * n + i] = t / sigma[j];
                }
        }
}
*/
 /*
int bchol(float* a,int n,float *det)
{ 
   int i,j,k,u,l;
    float d;
    if ((a[0]+1.0==1.0)||(a[0]<0.0))
      { return(-2);}
    a[0]=sqrt(a[0]);
    d=a[0];
    for (i=1; i<=n-1; i++)
      { u=i*n; a[u]=a[u]/a[0];}
    for (j=1; j<=n-1; j++)
      { l=j*n+j;
        for (k=0; k<=j-1; k++)
          { u=j*n+k; a[l]=a[l]-a[u]*a[u];}
        if ((a[l]+1.0==1.0)||(a[l]<0.0))
          {  return(-2);}
        a[l]=sqrt(a[l]);
        d=d*a[l];
        for (i=j+1; i<=n-1; i++)
          { u=i*n+j;
            for (k=0; k<=j-1; k++)
              a[u]=a[u]-a[i*n+k]*a[j*n+k];
            a[u]=a[u]/a[l];
          }
      }
    *det=d*d;
    for (i=0; i<=n-2; i++)
    for (j=i+1; j<=n-1; j++)
      a[i*n+j]=0.0;
    return(2);
  }



int rinv(float* a,int n)   
{ int *is,*js,i=0,j=0,k=0,l=0,u=0,v=0;   
  float d,p;   
  //is=malloc(n*sizeof(int));   
  //js=malloc(n*sizeof(int));   
  for (k=0; k<=n-1; k++)   
    { d=0.0;   
      for (i=k; i<=n-1; i++)   
      for (j=k; j<=n-1; j++)   
        { l=i*n+j; p=fabs(a[l]);   
          if (p>d) { d=p; is[k]=i; js[k]=j;}   
        }   
      if (d+1.0==1.0)   
        { 
          //free(is); free(js); 
          return(0);   
        }   
      if (is[k]!=k)   
        for (j=0; j<=n-1; j++)   
          { u=k*n+j; v=is[k]*n+j;   
            p=a[u]; a[u]=a[v]; a[v]=p;   
          }   
      if (js[k]!=k)   
        for (i=0; i<=n-1; i++)   
          { u=i*n+k; v=i*n+js[k];   
            p=a[u]; a[u]=a[v]; a[v]=p;   
          }   
      l=k*n+k;   
      a[l]=1.0/a[l];   
      for (j=0; j<=n-1; j++)   
        if (j!=k)   
          { u=k*n+j; a[u]=a[u]*a[l];}   
      for (i=0; i<=n-1; i++)   
        if (i!=k)   
          for (j=0; j<=n-1; j++)   
            if (j!=k)   
              { u=i*n+j;   
                a[u]=a[u]-a[i*n+k]*a[k*n+j];   
              }   
      for (i=0; i<=n-1; i++)   
        if (i!=k)   
          { u=i*n+k; a[u]=-a[u]*a[l];}   
    }   
  for (k=n-1; k>=0; k--)   
    { if (js[k]!=k)   
        for (j=0; j<=n-1; j++)   
          { u=k*n+j; v=js[k]*n+j;   
            p=a[u]; a[u]=a[v]; a[v]=p;   
          }   
      if (is[k]!=k)   
        for (i=0; i<=n-1; i++)   
          { u=i*n+k; v=i*n+is[k];   
            p=a[u]; a[u]=a[v]; a[v]=p;   
          }   
    }   
  //free(is); free(js);   
  return(1);   
}   

*/
void UD(float * A,int n,float * U,float * D)
{
	int i,j,k;
	int m;
	float sum1=0;
	float sum2=0;

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(i==j)
			{
				U[i*n+j]=1;
				D[i*n+j]=0;
			}
			else
			{
				D[i*n+j]=0;
				U[i*n+j]=0;
			}
		}
	}

	 m=n-1;
	D[m*n+m]=A[m*n+m];


	for(i=0;i<n;i++)
	{
		U[i*n+m]=A[i*n+m]/D[m*n+m];
	}

	
	for(j=n-2;j>=0;j--)
	{

		for(k=j+1;k<n;k++)
		{
			sum2=sum2+D[k*n+k]*U[j*n+k]*U[j*n+k];
		} 

		D[j*n+j]=A[j*n+j]-sum2;
		sum2=0;

		for(i=0;i<j;i++)
		{
			for(k=j+1;k<n;k++)
			{
			   	sum1=sum1+D[k*n+k]*U[i*n+k]*U[j*n+k];
			} 
		
			U[i*n+j]=(A[i*n+j]-sum1)/D[j*n+j];
			sum1=0;
		}

		
	}

}



/*
void LD(float * A,int n,float * U,float * D)
{
	int i,j,k;
	float sum1=0;
	float sum2=0;

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(i==j)
			{
				U[i*n+j]=1;
				D[i*n+j]=0;
			}
			else
			{
				D[i*n+j]=0;
				U[i*n+j]=0;
			}
		}
	}
	//int m=n-1;
	D[0]=A[0];

	//U[0]=1;

	for(i=0;i<n;i++)
	{
		U[i*n+0]=A[i*n+0]/A[0];
	}


	 
	
	
	for(j=0;j<n;j++)
	{

		for(k=0;k<j;k++)//
		{
			sum2=sum2+D[k*n+k]*U[j*n+k]*U[j*n+k];
		} 

		D[j*n+j]=A[j*n+j]-sum2;
		sum2=0;

		for(i=j+1;i<n;i++)//
		{
			for(k=0;k<j;k++)
			{
			   	sum1=sum1+D[k*n+k]*U[i*n+k]*U[j*n+k];
			} 
		
			U[i*n+j]=(A[i*n+j]-sum1)/D[j*n+j];
			sum1=0;
		}

		
	}

}
void QR(float *a,float* q,float *r1)
{
 //QR分解
 int i,j,k,r,m;
 float temp,sum,dr,cr,hr;
 int a_row=3;
 int a_colunm=3;
 float ur[9];
 float pr[9];
 float wr[9];
 float q1[9];
 float emp[9];
 for(i=0;i<a_row;i++)//将a放入temp中
  for(j=0;j<a_colunm;j++)
  {
   emp[i*a_colunm+j]=a[i*a_colunm+j];
  };
 for(i=0;i<a_row;i++)//定义单位矩阵
  for(j=0;j<a_row;j++)
  {
   if(i==j)q[i*a_row+j]=1;
   else q[i*a_row+j]=0;
  };
 for(r=0;r<a_colunm;r++)
 {
  temp=0;
  for(k=r;k<a_row;k++)
   temp+=fabs(a[k*a_colunm+r]);
  if(temp>=0.0)
  {
   sum=0;
   for(k=r;k<a_row;k++)
    sum+=a[k*a_colunm+r]*a[k*a_colunm+r];
   dr=sqrt(sum);
   if(a[r*a_colunm+r]>0.0)m=-1;
   else m=1;
   cr=m*dr;
   hr=cr*(cr-a[r*a_colunm+r]);
   for(i=0;i<a_row;i++)//定义ur
   {
    if(i<r)ur[i]=0;
    if(i==r)ur[i]=a[r*a_colunm+r]-cr;
    if(i>r)ur[i]=a[i*a_colunm+r];
   };
   for(i=0;i<a_row;i++)//定义wr
   {
    sum=0;
    for(j=0;j<a_row;j++)
     sum+=q[i*a_row+j]*ur[j];
    wr[i]=sum;
   };
   for(i=0;i<a_row;i++)//定义qr
    for(j=0;j<a_row;j++)
    {
     q1[i*a_row+j]=q[i*a_row+j]-wr[i]*ur[j]/hr;
    };
   for(i=0;i<a_row;i++)//定义qr+1
    for(j=0;j<a_row;j++)
    {
     q[i*a_row+j]=q1[i*a_row+j];
    };
   for(i=0;i<a_row;i++)//定义pr
   {
    sum=0;
    for(j=0;j<a_row;j++)
     sum+=a[j*a_colunm+i]*ur[j];
    pr[i]=sum/hr;
   };
   for(i=0;i<a_row;i++)
    for(j=0;j<a_colunm;j++)
    {
     a[i*a_colunm+j]=a[i*a_colunm+j]-ur[i]*pr[j];
    };
  };
 };
 for(i=0;i<a_row;i++)
  for(j=0;j<a_colunm;j++)
  {
   if(fabs(a[i*a_colunm+j])<0.0)a[i*a_colunm+j]=0;
  };
 for(i=0;i<a_row;i++)
  for(j=0;j<a_colunm;j++)
  {
   r1[i*a_colunm+j]=a[i*a_colunm+j];
  };
 for(i=0;i<a_row;i++)//将a取出
  for(j=0;j<a_colunm;j++)
  {
   a[i*a_colunm+j]=emp[i*a_colunm+j];
  }
}
*/
void Jacobi_Cyclic_Method(float *eigenvalues, float *eigenvectors,float *A, int n)
{
   int  i, j, k, m;
   float *pAk, *pAm, *p_r, *p_e;
   float threshold_norm;
   float threshold;
   float tan_phi, sin_phi, cos_phi, tan2_phi, sin2_phi, cos2_phi;
 volatile  float sin_2phi, cos_2phi, cot_2phi;
   float dum1;
   float dum2;
   float dum3;
   float max;

                  // Take care of trivial cases

   if ( n < 1) return;
   if ( n == 1) {
      eigenvalues[0] = *A;
      *eigenvectors = 1.0f;
      return;
   }

          // Initialize the eigenvalues to the identity matrix.

   for (p_e = eigenvectors, i = 0; i < n; i++)
      for (j = 0; j < n; p_e++, j++)
         if (i == j) *p_e = 1.0f; else *p_e = 0.0f;
  
            // Calculate the threshold and threshold_norm.
 
   for (threshold = 0.0f, pAk = A, i = 0; i < ( n - 1 ); pAk += n, i++) 
      for (j = i + 1; j < n; j++) threshold += *(pAk + j) * *(pAk + j);
   threshold = sqrt(threshold + threshold);
   threshold_norm = threshold * 0.001f;
   max = threshold + 1.0f;
   while (threshold > threshold_norm) {
      threshold /= 10.0f;
      if (max < threshold) continue;
      max = 0.0f;
      for (pAk = A, k = 0; k < (n-1); pAk += n, k++) {
         for (pAm = pAk + n, m = k + 1; m < n; pAm += n, m++) {
            if ( fabs(*(pAk + m)) < threshold ) continue;

                 // Calculate the sin and cos of the rotation angle which
                 // annihilates A[k][m].

            cot_2phi = 0.5f * ( *(pAk + k) - *(pAm + m) ) / *(pAk + m);
            dum1 = sqrt( cot_2phi * cot_2phi + 1.0f);
            if (cot_2phi < 0.0f) dum1 = -dum1;
            tan_phi = -cot_2phi + dum1;
            tan2_phi = tan_phi * tan_phi;
            sin2_phi = tan2_phi / (1.0f + tan2_phi);
            cos2_phi = 1.0f - sin2_phi;
            sin_phi = sqrt(sin2_phi);
            if (tan_phi < 0.0f) sin_phi = - sin_phi;
            cos_phi = sqrt(cos2_phi); 
            sin_2phi = 2.0f * sin_phi * cos_phi;
            cos_2phi = cos2_phi - sin2_phi;

                     // Rotate columns k and m for both the matrix A 
                     //     and the matrix of eigenvectors.

            p_r = A;
            dum1 = *(pAk + k);
            dum2 = *(pAm + m);
            dum3 = *(pAk + m);
            *(pAk + k) = dum1 * cos2_phi + dum2 * sin2_phi + dum3 * sin_2phi;
            *(pAm + m) = dum1 * sin2_phi + dum2 * cos2_phi - dum3 * sin_2phi;
            *(pAk + m) = 0.0f;
            *(pAm + k) = 0.0f;
            for (i = 0; i < n; p_r += n, i++) {
               if ( (i == k) || (i == m) ) continue;
               if ( i < k ) dum1 = *(p_r + k); else dum1 = *(pAk + i);
               if ( i < m ) dum2 = *(p_r + m); else dum2 = *(pAm + i);
               dum3 = dum1 * cos_phi + dum2 * sin_phi;
               if ( i < k ) *(p_r + k) = dum3; else *(pAk + i) = dum3;
               dum3 = - dum1 * sin_phi + dum2 * cos_phi;
               if ( i < m ) *(p_r + m) = dum3; else *(pAm + i) = dum3;
            }
            for (p_e = eigenvectors, i = 0; i < n; p_e += n, i++) {
               dum1 = *(p_e + k);
               dum2 = *(p_e + m);
               *(p_e + k) = dum1 * cos_phi + dum2 * sin_phi;
               *(p_e + m) = - dum1 * sin_phi + dum2 * cos_phi;
            }
         }
         for (i = 0; i < n; i++)
            if ( i == k ) continue;
            else if ( max < fabs(*(pAk + i))) max = fabs(*(pAk + i));
      }
   }
   for (pAk = A, k = 0; k < n; pAk += n, k++) eigenvalues[k] = *(pAk + k); 
}
