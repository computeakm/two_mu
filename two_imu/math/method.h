#ifndef __METHOD_H
#define __METHOD_H

typedef struct Attitude{
    float Roll;             /*!< Roll angle value. This parameter is in units of degrees */
    float Pitch;            /*!< Pitch angle value. This parameter is in units of degrees */
    float Yaw;              /*!< Yaw angle value. This parameter is in units of degrees */
    float meter;
	  float IMU_TEMP;
   
} Attitude;

typedef struct See_data{
		float ax;
	  float ay;
	  float az;
	  float gx;
		float gy;
	  float gz;
    float pa;
   
   
} See_data;



uint8_t IMU_Init(void);
void math_init(void);
void Data_Updata(void);
void Data_methob(Attitude* AT);
void Data_See(See_data* se,uint8_t num);
void get_Attitude(Attitude* AT);




#endif
