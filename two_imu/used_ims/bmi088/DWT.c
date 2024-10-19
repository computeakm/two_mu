#include "DWT.h"

/*******************************************************************************
* ��    �ƣ� ��ʱ
* ��    �ܣ� ��ʱ
* ��ڲ����� _ulDelayTime����ʱ��ʱ��
* ���ڲ����� ��
* �������ߣ� BAXIANGE.
* �������ڣ� 2021-05-21
* ��    �ģ�
* �޸����ڣ�
* ��    ע��
*******************************************************************************/
uint8_t times_t;

void bsp_InitDWT(void)
{
	DEM_CR         |= (unsigned int)DEM_CR_TRCENA;   /* Enable Cortex-M4's DWT CYCCNT reg.  */
	DWT_CYCCNT      = (unsigned int)0u;
	DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}


void bsp_DelayUS(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = DWT_CYCCNT;                                     /* �ս���ʱ�ļ�����ֵ */
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */	
	}
}

void bsp_DelayMS(uint32_t _ulDelayTime)
{
	bsp_DelayUS(1000*_ulDelayTime);
}

/* Sleep implementation */
void inv_imu_sleep_us(uint32_t us)
{
	bsp_DelayUS(us);
}

uint32_t inv_imu_get_time_us(void)
{

	
	return OS_TS_GET()/73 + times_t*58835168;
}

uint32_t OS_TS_GET(void)
 
{
	uint32_t _get_ts;
	
	uint32_t _ts;
	
	static uint32_t _ts_bak;    /* ʱ������� */
 
//	_get_ts = DWT_CYCCNT / 73;
	_get_ts = DWT_CYCCNT;
	
	if(_get_ts < _ts_bak)
		{
			/* ��������� */
			_ts = 0XFFFFFFFF - _ts_bak + _get_ts;
//			_ts = 58835168 - _ts_bak + _get_ts;
			/* �����ϴ����� �����������ʱ���*/
			_ts = _ts+_ts_bak;
		}
	else
		{
			/* ������� */
			_ts = _get_ts;
		}
		
		_ts_bak = _ts;
		
		return _ts;
}

