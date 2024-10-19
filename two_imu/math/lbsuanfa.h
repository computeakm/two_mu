#ifndef __LBSUANFA_H
#define __LBSUANFA_H
typedef struct iir_parameter{
    float now_data;
	  float old_data;
	  float new_data;
	  float a;
	  float dt;
   
} iir_parameter;


void iir_init(iir_parameter* iir_data,float a,float dt,float* data);

float iir(iir_parameter* iir_data,float* data,float new_data);
float fir(float* data,float new_data);








#endif
