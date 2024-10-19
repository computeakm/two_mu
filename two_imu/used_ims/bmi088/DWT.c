#include "DWT.h"

/*******************************************************************************
* 名    称： 延时
* 功    能： 延时
* 入口参数： _ulDelayTime：延时的时间
* 出口参数： 无
* 作　　者： BAXIANGE.
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注：
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
		
	tStart = DWT_CYCCNT;                                     /* 刚进入时的计数器值 */
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */	
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
	
	static uint32_t _ts_bak;    /* 时间戳备份 */
 
//	_get_ts = DWT_CYCCNT / 73;
	_get_ts = DWT_CYCCNT;
	
	if(_get_ts < _ts_bak)
		{
			/* 做溢出修正 */
			_ts = 0XFFFFFFFF - _ts_bak + _get_ts;
//			_ts = 58835168 - _ts_bak + _get_ts;
			/* 加上上次数据 即可求出本次时间差*/
			_ts = _ts+_ts_bak;
		}
	else
		{
			/* 正常情况 */
			_ts = _get_ts;
		}
		
		_ts_bak = _ts;
		
		return _ts;
}

