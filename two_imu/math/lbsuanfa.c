#include "math.h"
#include "main.h"
#include "lbsuanfa.h"
void iir_init(iir_parameter* iir_data,float a,float dt,float* data)
{
	iir_data->now_data=data[0];
	iir_data->new_data=data[1];
  iir_data->old_data=data[2];	
	iir_data->dt=dt;
	iir_data->a=a;
	
}


void add_data(float* data,float new_data)
{
	for(int i=2;i>0;i--)
	{
		data[i]=data[i-1];
	}
	data[0]=new_data;
	
}	

void iir_math(iir_parameter* iir_data,float* data)
{
  iir_data->now_data=data[0];
  iir_data->old_data=data[2];	
	iir_data->new_data=iir_data->now_data*iir_data->a+iir_data->old_data*(1-iir_data->a);
	data[1]=iir_data->new_data;
	
}


float iir(iir_parameter* iir_data,float* data,float new_data)
{
	add_data(data,new_data);
	iir_math(iir_data,data);
	return data[1];
}


float fir_math(float* data)
{
	uint8_t num=0;
	float fir_data;
fir_data=(data[num]+data[num+1]+data[num+2])/3;	
	return fir_data;
}

float fir(float* data,float new_data)
{
	add_data(data,new_data);
	return fir_math(data);
	
}




