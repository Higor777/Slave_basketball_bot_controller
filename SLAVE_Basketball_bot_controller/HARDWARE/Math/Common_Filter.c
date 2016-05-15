

/*
   限幅滤波算法 
*/
typedef float ALF;  // 滤波的数据类型

ALF AmplitudeLimiterFilter(ALF new_value , ALF A)   // 输入当前采集值 和限幅幅度   返回有效值
{   
	
  static ALF value;    
	if ( ( new_value - value > A ) || ( value - new_value > A ))  
	return value;              // 返回上一次有效值
	else
	{
     value=new_value;       //更新上次有效值并返回	
		 return value;  
  }
      
}   

/*	
	 中位值滤波法 
	 连续采样N次，按大小排序，取中间值为有效值
	 采用冒泡法
*/

typedef float MIDF;  // 滤波的数据类型
#define MidN  11   
MIDF MiddlevalueFilter(MIDF new_value[MidN]) //输入最近采样的n个值，返回中值
{   
		MIDF value_buf[MidN];   
		char count,i,j,temp;   
		for ( count=0;count<MidN;count++)   
		{   
				value_buf[count] = new_value[count];   	
		}   
		for (j=0;j<MidN-1;j++)   
		{   
				for (i=0;i<MidN-j;i++)   
				{   
						if ( value_buf[i]>value_buf[i+1] )   
						{   
								temp = value_buf[i];   
								value_buf[i] = value_buf[i+1];    
								value_buf[i+1] = temp;   
						}   
				}   
		}   
		return value_buf[(MidN-1)/2];   
}        

/*
  算术平均滤波算法
*/	

typedef float AAF;  // 滤波的数据类型
#define AAN 12   
AAF Arithmetic_Average_F(AAF new_value[AAN])   
{   
		AAF  sum = 0;   
	  char count;
		for ( count=0;count<AAN;count++)   
		{  
			
				sum += new_value[count];   
			  
		}   
		return sum/AAN;   
}   

		
/*	

   滑动平均滤波 
   对之前N个值取平均
   
*/
typedef float SAF;  // 滤波的数据类型
#define SAN 12   

char Slide_Average_filter(SAF new_value)   
{   
		static SAF value_buf[SAN],sum=0;   
		static char i=0,count=0;     
		value_buf[i++] =  new_value;   
		if ( i == SAN )   i = 0;   
		for ( count=0;count<SAN;count++)   
				sum += value_buf[count];   
		return sum/SAN;   
}   

/*
		
    #define N 12   
    char filter()   
    {   
        char count,i,j;   
        char value_buf[N];   
        int  sum=0;   
        for  (count=0;count<N;count++)   
        {   
            value_buf[count] = get_ad();   
            delay();   
        }   
        for (j=0;j<N-1;j++)   
        {   
            for (i=0;i<N-j;i++)   
            {   
                if ( value_buf[i]>value_buf[i+1] )   
                {   
                    temp = value_buf[i];   
                    value_buf[i] = value_buf[i+1];    
                    value_buf[i+1] = temp;   
                }   
            }   
        }   
        for(count=1;count<N-1;count++)   
            sum += value[count];   
        return (char)(sum/(N-2));   
    }   

/*****

		
    #define a 50   
    char value;   
    char filter()   
    {   
        char  new_value;   
        new_value = get_ad();   
        return (100-a)*value + a*new_value;    
    }   

		
		
		
    #define N 12   
    char code coe[N] = {1,2,3,4,5,6,7,8,9,10,11,12};   
    char code sum_coe = 1+2+3+4+5+6+7+8+9+10+11+12;   
    char filter()   
    {   
        char count;   
        char value_buf[N];   
        int  sum=0;   
        for (count=0,count<N;count++)   
        {   
            value_buf[count] = get_ad();   
            delay();   
        }   
        for (count=0,count<N;count++)   
            sum += value_buf[count]*coe[count];   
        return (char)(sum/sum_coe);   
    }   

		
		
    #define N 12   
    char filter()   
    {   
        char count=0;   
        char new_value;   
        new_value = get_ad();   
        while (value !=new_value);   
        {   
            count++;   
            if (count>=N)   return new_value;   
            delay();   
            new_value = get_ad();   
        }   
        return value;       
    }   
		****/
