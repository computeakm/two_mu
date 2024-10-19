
#include "imu_io.h"








uint8_t flag;

kalman1_state kal_a2;
struct bmp3_data bmp390_data,bmp390_data1;

icm42688RawData_t icm42688_a_data;
icm42688RawData_t icm42688_g_data;

Vector3 bmi088_a_data;
Vector3 bmi088_g_data;

float p_data[3],ax1_data[3],ay1_data[3],az1_data[3],gx1_data[3],gy1_data[3],gz1_data[3],
	              ax2_data[3],ay2_data[3],az2_data[3],gx2_data[3],gy2_data[3],gz2_data[3];

float pa,ax,ay,az,gx,gy,gz;
iir_parameter iir_a1,iir_g1,iir_a2,iir_g2,iir_p,iir_m,iir_roll;



float m[3],YAW[3],Pitch[3],RL[4];
TM_AHRSIMU_t AHRSIMU;
Attitude at;



uint8_t IMU_Init(void)
{
	
	int IMU_flag,flag;
	int try_num;
	
	
	
	
	IMU_flag=0;
	flag=BMP390_Init();
	switch (flag)
	{
		case 0:
			IMU_flag=IMU_flag;
			break;
		default :
			IMU_flag++;
	}
	
	
	
	flag=0;
	try_num=0;
	IMU_INIT(&bim088_spi);
	IMU_ENABLE_ALL();
	flag=IMU_READY();
	while(flag!=1&&try_num<10)
	{
		IMU_INIT(&bim088_spi);
		IMU_ENABLE_ALL();
		flag=IMU_READY();;
		try_num++;
	}
	switch (flag)
	{
		case 1:
			IMU_flag=IMU_flag;
			break;
		default :
			IMU_flag++;
	}
	
	
	flag=0;
	try_num=0;
	flag=bsp_Icm42688Init();
	while(flag!=0&&try_num<10)
	{
		flag=bsp_Icm42688Init();
		try_num++;
	}
	
		switch (flag)
		{
			case 0:
				IMU_flag=IMU_flag;
				break;
			default :
				IMU_flag++;
		}
		
		
	math_init();
 
return IMU_flag;	
	
}	



void math_init(void)
{
	 TM_AHRSIMU_Init(&AHRSIMU, 200, 0.2, 0);
	iir_init(&iir_p,0.09,12,&p_data[3]);
	iir_init(&iir_m,0.09,12,&m[3]);
	iir_init(&iir_a1,0.9,12,&ax1_data[3]);
	iir_init(&iir_g1,0.5,12,&gx1_data[3]);
	iir_init(&iir_a2,0.09,12,&ax2_data[3]);
	iir_init(&iir_g2,0.02,12,&gx2_data[3]);
	iir_init(&iir_roll,0.2,12,RL);
	
	
	
}





void Data_Updata(void)
{    
	
			bmp390_data1=bmp390_getdata();
			if(bmp390_data1.pressure!=-1)
			{
					bmp390_data=bmp390_data1;
					pa=iir(&iir_p,p_data,bmp390_data.pressure);
			}

			
			
			if(flag==1)
			{
						if(icm42688_READ_DATA_RDY_INT()==1)
					{
						bsp_IcmGetAccelerometer(&icm42688_a_data);
						bsp_IcmGetGyroscope(&icm42688_g_data);
						ax=iir(&iir_a2,ax1_data,icm42688_a_data.x);
						ay=iir(&iir_a2,ay1_data,icm42688_a_data.y);
						az=iir(&iir_a2,az1_data,icm42688_a_data.z);
						gx=iir(&iir_a2,gx1_data,icm42688_g_data.x);
						gy=iir(&iir_a2,gy1_data,icm42688_g_data.y);
						gz=iir(&iir_a2,gz1_data,icm42688_g_data.z);
						flag=1-flag;	
					}
			}
			
			if(flag==0)
			{
					bmi088_a_data=ACCEL_READ_ACCELERATION();	
					bmi088_g_data=GYRO_READ_RATES();

					ax=iir(&iir_a2,ax2_data,bmi088_a_data.x);
					ay=iir(&iir_a2,ay2_data,bmi088_a_data.y);
					az=iir(&iir_a2,az2_data,bmi088_a_data.z);
					gx=iir(&iir_g2,gx2_data,bmi088_g_data.x);
					gy=iir(&iir_g2,gy2_data,bmi088_g_data.y);
					gz=iir(&iir_g2,gz2_data,bmi088_g_data.z);
					flag=1-flag;	
			}
			
			
					
			
	
	
	
}


void Data_methob(Attitude* AT)
{
	
		TM_AHRSIMU_UpdateIMU(&AHRSIMU,gx,gy,gz,ax,ay,az);
		AT->meter=iir(&iir_m,m,convert_Pa_to_meter(pa));
		AT->Pitch=AHRSIMU.Pitch;
		AT->Yaw=AHRSIMU.Yaw;
		AT->Roll=AHRSIMU.Roll;

	
						//AT->meter=pa;
					//AT->Pitch=ay;
					//AT->Yaw=az-1;
					//AT->Roll=ax-6;
					//AT->Roll=ax-6;
					//AT->Roll=ay-2.5;
					//AT->Roll=az-1.2;
					//AT->Pitch=az;

	
//	
//	

	
}





void Data_See(See_data* se,uint8_t num)
{    
	
			bmp390_data1=bmp390_getdata();
			if(bmp390_data1.pressure!=-1)
			{
					bmp390_data=bmp390_data1;
					se->pa=iir(&iir_p,p_data,bmp390_data.pressure);
			}

			
			
			if(num==1)
			{
						if(icm42688_READ_DATA_RDY_INT()==1)
					{
						bsp_IcmGetAccelerometer(&icm42688_a_data);
						bsp_IcmGetGyroscope(&icm42688_g_data);
						se->ax=iir(&iir_a2,ax1_data,icm42688_a_data.x);
						se->ay=iir(&iir_a2,ay1_data,icm42688_a_data.y);
						se->az=iir(&iir_a2,az1_data,icm42688_a_data.z);
						se->gx=iir(&iir_a2,gx1_data,icm42688_g_data.x);
						se->gy=iir(&iir_a2,gy1_data,icm42688_g_data.y);
						se->gz=iir(&iir_a2,gz1_data,icm42688_g_data.z);
						
					}
			}
			
			if(num==0)
			{
					bmi088_a_data=ACCEL_READ_ACCELERATION();	
					bmi088_g_data=GYRO_READ_RATES();

					se->ax=iir(&iir_a2,ax2_data,bmi088_a_data.x);
					se->ay=iir(&iir_a2,ay2_data,bmi088_a_data.y);
					se->az=iir(&iir_a2,az2_data,bmi088_a_data.z);
					se->gx=iir(&iir_g2,gx2_data,bmi088_g_data.x);
					se->gy=iir(&iir_g2,gy2_data,bmi088_g_data.y);
					se->gz=iir(&iir_g2,gz2_data,bmi088_g_data.z);
					
			}
	
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	
	
	if(htim->Instance==used_tim.Instance)
	{
			
		Data_Updata();
		Data_methob(&at);
  
	}
}


void get_Attitude(Attitude* AT)
{
	AT->Pitch=at.Pitch;
	AT->Roll=at.Roll;
	AT->Yaw=at.Yaw;
	AT->meter=at.meter;
	AT->IMU_TEMP=at.IMU_TEMP;
	
	
}



