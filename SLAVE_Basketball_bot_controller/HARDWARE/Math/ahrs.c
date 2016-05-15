//#include <math.h>
//#include "Matrix.h"
//#include "ahrs.h"

//extern float q[4*1];
//float qk_1[4*1];
//float A[4*4];//define state transition matrix 4*4
//float A_T[4*4];
//float H[2*4];//define jacobian matrix
//float H_T[4*2];
//float P[4*4]={   0.0001,0.0,0.0,0.0,
//                 0.0,0.0001,0.0,0.0,
//								 0.0,0.0,0.0001,0.0,
//								 0.0,0.0,0.0,0.0001
//						 };//define covariance matrix
//float PKK_1[4*4];//P(K|K-1)
//float PH_T[4*2];
//float HPH_T[2*2];
//float Kg[4*2];//define Kalman gain matrix
//float KgH[4*4];
//float Q[4*4]={   1.0e-4,0.0,0.0,0.0,
//                 0.0,1.0e-4,0.0,0.0,
//								 0.0,0.0,1.0e-4,0.0,
//								 0.0,0.0,0.0,1.0e-4//0.00001
//             };//define the covariance of state process noise
//float R[2*2]={   0.42f,0.0,
//                 0.0,0.42f
//             };//define the covariance of observe noise   0.42f


////W：为陀螺仪的输出，q：是上一时刻滤波后的最优结果，Z：是使用加速度计和地磁传感器计算出的偏航角ψ、俯仰角θ和横滚角γ
////输出的结果最需要的是：   P[4*4]  q

//void Kalman_filter(float* W,float* Z,uint8_t flag)
//{
//unsigned char i;

//float DX,DY,DZ;
//float temp,atemp,btemp,temp_x,temp_y,temp_z;
//float model_q;
//float dcm02,dcm12,dcm22;
//float phi_err,theta_err;
//float AP[4*4];

//float dZ[2*1];
//float seq[4*1];

//qk_1[0]=q[0];
//qk_1[1]=q[1];
//qk_1[2]=q[2];
//qk_1[3]=q[3];

///*******firstly calculate A*******/

//DX=(W[0])*T*0.01745f;
//DY=(W[1])*T*0.01745f;
//DZ=(W[2])*T*0.01745f;

//temp=(DX*DX+DY*DY+DZ*DZ);

//atemp=0.5f-temp*0.0208f;
//btemp=1.0f-0.125f*temp;
//temp_x=atemp*DX;
//temp_y=atemp*DY;
//temp_z=atemp*DZ;

//A[0]=btemp;
//A[1]=-temp_x;
//A[2]=-temp_y;
//A[3]=-temp_z;

//A[4]=temp_x;
//A[5]=btemp;
//A[6]=temp_z;
//A[7]=-temp_y;

//A[8]=temp_y;
//A[9]=-temp_z;
//A[10]=btemp;
//A[11]=temp_x;

//A[12]=temp_z;
//A[13]=temp_y;
//A[14]=-temp_x;
//A[15]=btemp;
///******calculate A has completed******/

///******update the state of time k******/
///******Q(K|K-1)=A*Q(K-1|K-1)***********/
//q[0]=A[0]*qk_1[0]+A[1]*qk_1[1]+A[2]*qk_1[2]+A[3]*qk_1[3];
//q[1]=A[4]*qk_1[0]+A[5]*qk_1[1]+A[6]*qk_1[2]+A[7]*qk_1[3];
//q[2]=A[8]*qk_1[0]+A[9]*qk_1[1]+A[10]*qk_1[2]+A[11]*qk_1[3];
//q[3]=A[12]*qk_1[0]+A[13]*qk_1[1]+A[14]*qk_1[2]+A[15]*qk_1[3];

///*四元数归一*/
//model_q=sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);//四元数模
//q[0]=q[0]/model_q;
//q[1]=q[1]/model_q;
//q[2]=q[2]/model_q;
//q[3]=q[3]/model_q;


///******update the covariance of process******/
///******P(K|K-1)=A*P(K-1|K-1)*A_T******/

//MatrixTranspose(A,4,4,A_T);          //A_T= A(T)
//MatrixMultiply(A,4,4,P,4,4,AP);       //AP  = A*P
//MatrixMultiply(AP,4,4,A_T,4,4,PKK_1);    //P(K,K-1)      PKK_1 = AP*A_T
//PKK_1[0]  +=  Q[0];
//PKK_1[5]  +=  Q[5];
//PKK_1[10] +=  Q[10];
//PKK_1[15] +=  Q[15];			 

///******update Kalman gain******/
// dcm02 =     2*(q[1]*q[3] - q[0]*q[2]);  //T13
// dcm12 =     2*(q[2]*q[3] + q[0]*q[1]);  //T23
// dcm22 = 1.0f-2*(q[1]*q[1] + q[2]*q[2]);  //T33

// //计算偏导，即求出观测矩阵H
// theta_err = 2.0f / sqrt( 1 - dcm12*dcm12 );
// phi_err   = 2.0f / (dcm22*dcm22 + dcm02*dcm02);

// //HH中的q是更新前的还是更新后的？？？？？？
// /*H为经过变化后的观测矩阵,是观测方程分别对q0、q1、q2、q3求偏导所得*/

//H[0]= q[1] * theta_err;
//H[1]= q[0] * theta_err;
//H[2]= q[3] * theta_err;
//H[3]= q[2] * theta_err;

//H[4]= (q[2] * dcm22 )* phi_err;
//H[5]=-(q[3] * dcm22 + 2*q[1] * dcm02) * phi_err;
//H[6]= (q[0] * dcm22 - 2*q[2] * dcm02) * phi_err;
//H[7]=-(q[1] * dcm22 ) * phi_err;			 

////H中的q是更新前的还是更新后的？？？？？？
//MatrixTranspose(H,2,4,H_T);          //H_T= H(T)(4*2)
//MatrixMultiply(PKK_1,4,4,H_T,4,2,PH_T);    //PH_T(4*2)=PKK_1*H_T
//MatrixMultiply(H,2,4,PH_T,4,2,HPH_T);    //HPH_T(2*2)=H*P*H_T

////HPH'+R
//HPH_T[0]=HPH_T[0]+R[0];
//HPH_T[3]=HPH_T[3]+R[3];

///*(HPH'+R)^-1,PK[9]*/
//MatrixInverse(HPH_T,2,0);//(HPH_T+R)^-1
//MatrixMultiply(PH_T,4,2,HPH_T,2,2,Kg);    //Kg(4*3)=PH_T*(HPH_T+R)^-1

///******update the best estimate of time k******/
// 
///*dZ=Z-Z~*/
//dZ[0]=Z[1]-asin(dcm12);        //dθ
//dZ[1]=Z[2]+atan2(dcm02,dcm22);  //dγ
//MatrixMultiply(Kg,4,2,dZ,2,1,seq);//seq[4],滤波结果       seq=Kg*dZ

///******Q(K|K)=Q(K|K-1)+seq******/
//if(flag==0) 
//{
//          q[0]+=seq[0];
//          q[1]+=seq[1];
//          q[2]+=seq[2];
//          q[3]+=seq[3];
//}

//model_q=sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);//四元数模
//q[0]=q[0]/model_q;
//q[1]=q[1]/model_q;
//q[2]=q[2]/model_q;
//q[3]=q[3]/model_q;	
///******update the best estimate covariance of time k******/
//MatrixMultiply(Kg,4,2,H,2,4,KgH);                     //KgH=Kg*H

//for(i=0;i<16;i++)																															  
//{
//if((i==0)||(i==5)||(i==10)||(i==15))
//   KgH[i]=1-KgH[i];
//else
//  KgH[i]=-KgH[i];
//}//[I-Kg*H]

//MatrixMultiply(KgH,4,4,PKK_1,4,4,P);                   //P=KgH*PKK_1
//}

//extern float euler_angle[3];

//void observe(float acc_x,float acc_y,float acc_z)
//{
//  if(acc_y>1.0f) acc_y=1.0f;
//	if(acc_y<-1.0f) acc_y=-1.0f;
//   euler_angle[1]= asin(acc_y);//pitch
//   euler_angle[2]=-atan2(acc_x,acc_z); //roll
//}

