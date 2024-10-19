#include "IO_SPI.h"
#include "DWT.h"
#include "spi.h"
/*******************************************************************************
* ��    �ƣ� icm42688_read_reg
* ��    �ܣ� ��ȡ�����Ĵ�����ֵ
* ��ڲ����� reg: �Ĵ�����ַ
* ���ڲ����� ��ǰ�Ĵ�����ַ��ֵ
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ʹ��SPI��ȡ�Ĵ���ʱҪע��:���λΪ��дλ�����datasheet page51.
*******************************************************************************/
 
uint8_t hal_Spi2_ReadWriteByte(uint8_t txdata)
{
    uint8_t rxdata = 0;
    HAL_SPI_TransmitReceive(&hspi2, &txdata, &rxdata, 1, 5); //��ʱ5ms
    return rxdata;
}
 
/*******************************************************************************
* ��    �ƣ� Icm_Spi_ReadWriteNbytes
* ��    �ܣ� ʹ��SPI��дn���ֽ�
* ��ڲ����� pBuffer: д�������  len:д������ĳ���
* ���ڲ����� ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע��
*******************************************************************************/
void Icm_Spi_ReadWriteNbytes(uint8_t* pBuffer, uint8_t len)
{
   
 
#if defined(ICM_USE_HARD_SPI)
	  uint8_t i = 0;
    for(i = 0; i < len; i ++)
    {
//        *pBuffer = hal_Spi2_ReadWriteByte(*pBuffer);
			  *pBuffer = hal_Spi2_ReadWriteByte(*pBuffer);
        pBuffer++;
    }
#endif
 
}
 
 
/*******************************************************************************
* ��    �ƣ� icm42688_read_reg
* ��    �ܣ� ��ȡ�����Ĵ�����ֵ
* ��ڲ����� reg: �Ĵ�����ַ
* ���ڲ����� ��ǰ�Ĵ�����ַ��ֵ
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ʹ��SPI��ȡ�Ĵ���ʱҪע��:���λΪ��дλ�����datasheet page51.
*******************************************************************************/
uint8_t icm42688_read_reg(uint8_t reg)
{
    uint8_t regval = 0;
 
#if defined(ICM_USE_HARD_SPI)
    SPI_SCL3300_CS_LOW();
//		printf("ICM_SPI_CS_LOW\n");
    reg |= 0x80;
    /* д��Ҫ���ļĴ�����ַ */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* ��ȡ�Ĵ������� */
    Icm_Spi_ReadWriteNbytes(&regval, 1);
    SPI_SCL3300_CS_HIGH();
//		printf("ICM_SPI_CS_HIGH\n");
#elif defined(ICM_USE_HARD_I2C)
 
#endif
 
    return regval;
}
 
/*******************************************************************************
* ��    �ƣ� icm42688_read_regs
* ��    �ܣ� ������ȡ����Ĵ�����ֵ
* ��ڲ����� reg: ��ʼ�Ĵ�����ַ *buf����ָ��,uint16_t len����
* ���ڲ����� ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ʹ��SPI��ȡ�Ĵ���ʱҪע��:���λΪ��дλ�����datasheet page50.
*******************************************************************************/
void icm42688_read_regs(uint8_t reg, uint8_t* buf, uint16_t len)
{
#if defined(ICM_USE_HARD_SPI)
    reg |= 0x80;
    SPI_SCL3300_CS_LOW();
    /* д��Ҫ���ļĴ�����ַ */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* ��ȡ�Ĵ������� */
    Icm_Spi_ReadWriteNbytes(buf, len);
    SPI_SCL3300_CS_HIGH();
#elif defined(ICM_USE_HARD_I2C)
#endif
}
 
 
/*******************************************************************************
* ��    �ƣ� icm42688_write_reg
* ��    �ܣ� �򵥸��Ĵ���д����
* ��ڲ����� reg: �Ĵ�����ַ value:����
* ���ڲ����� 0
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� ʹ��SPI��ȡ�Ĵ���ʱҪע��:���λΪ��дλ�����datasheet page50.
*******************************************************************************/
uint8_t icm42688_write_reg(uint8_t reg, uint8_t value)
{
#if defined(ICM_USE_HARD_SPI)
    SPI_SCL3300_CS_LOW();
    /* д��Ҫ���ļĴ�����ַ */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* ��ȡ�Ĵ������� */
    Icm_Spi_ReadWriteNbytes(&value, 1);
    SPI_SCL3300_CS_HIGH();
#elif defined(ICM_USE_HARD_I2C)
#endif
    return 0;
}
