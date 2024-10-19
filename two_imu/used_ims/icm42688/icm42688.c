#include "icm42688.h"
#if defined(ICM_USE_HARD_SPI)
#include "spi.h"
#include "IO_SPI.h"
#elif defined(ICM_USE_HARD_I2C)
#include "bsp_cpu_i2c2.h"
#endif
#include "DWT.h"
#include "stdio.h"
#include "imu_io.h"
static float accSensitivity   = 0.244f;   //���ٶȵ���С�ֱ��� mg/LSB
static float gyroSensitivity  = 32.8f;    //�����ǵ���С�ֱ���


/*ICM42688ʹ�õ�ms����ʱ�����������û��ṩ��*/
#define ICM42688DelayMs(_nms)  bsp_DelayMS(_nms)

#if defined(ICM_USE_HARD_SPI)
#define ICM_RCC_SPIX_CS()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define ICM_SPI_CS_LOW()     HAL_GPIO_WritePin(ICM_PORT_SPIX_CS, ICM_PIN_SPIX_CS, GPIO_PIN_RESET)
#define ICM_SPI_CS_HIGH()    HAL_GPIO_WritePin(ICM_PORT_SPIX_CS, ICM_PIN_SPIX_CS, GPIO_PIN_SET)


/*******************************************************************************
* ��    �ƣ� bsp_IcmSpixCsInit
* ��    �ܣ� Icm SPI��CS�������ų�ʼ��
* ��ڲ����� ��
* ���ڲ����� ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע��
*******************************************************************************/
//void bsp_IcmSpixCsInit(void)
//{
//    GPIO_InitTypeDef GPIO_InitStruct = {0};

//    /* GPIO Ports Clock Enable */
//    ICM_RCC_SPIX_CS();

//    /*Configure GPIO pins */
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    GPIO_InitStruct.Pin = ICM_PIN_SPIX_CS;
//    HAL_GPIO_Init(ICM_PORT_SPIX_CS, &GPIO_InitStruct);
//    HAL_GPIO_WritePin(ICM_PORT_SPIX_CS, ICM_PIN_SPIX_CS, GPIO_PIN_SET);
//}

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
static void Icm_Spi_ReadWriteNbytes(uint8_t* pBuffer, uint8_t len)
{
    uint8_t i = 0;

#if defined(ICM_USE_HARD_SPI)
    for(i = 0; i < len; i ++)
    {
        *pBuffer = hal_Spi2_ReadWriteByte(*pBuffer);
			  //*pBuffer = HAL_SPI_WriteReadByte(*pBuffer);

        pBuffer++;
    }
#endif

}
#endif

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
static uint8_t icm42688_read_reg(uint8_t reg)
{
    uint8_t regval = 0;

#if defined(ICM_USE_HARD_SPI)
    ICM_SPI_CS_LOW();
//		printf("ICM_SPI_CS_LOW\n");
    reg |= 0x80;
    /* д��Ҫ���ļĴ�����ַ */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* ��ȡ�Ĵ������� */
    Icm_Spi_ReadWriteNbytes(&regval, 1);
    ICM_SPI_CS_HIGH();
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
static void icm42688_read_regs(uint8_t reg, uint8_t* buf, uint16_t len)
{
#if defined(ICM_USE_HARD_SPI)
    reg |= 0x80;
    ICM_SPI_CS_LOW();
    /* д��Ҫ���ļĴ�����ַ */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* ��ȡ�Ĵ������� */
    Icm_Spi_ReadWriteNbytes(buf, len);
    ICM_SPI_CS_HIGH();
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
static uint8_t icm42688_write_reg(uint8_t reg, uint8_t value)
{
#if defined(ICM_USE_HARD_SPI)
    ICM_SPI_CS_LOW();
    /* д��Ҫ���ļĴ�����ַ */
    Icm_Spi_ReadWriteNbytes(&reg, 1);
    /* ��ȡ�Ĵ������� */
    Icm_Spi_ReadWriteNbytes(&value, 1);
    ICM_SPI_CS_HIGH();
#elif defined(ICM_USE_HARD_I2C)
#endif
    return 0;
}



float bsp_Icm42688GetAres(uint8_t Ascale)
{
    switch(Ascale)
    {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (11), 4 Gs (10), 8 Gs (01), and 16 Gs  (00).
    case AFS_2G:
        accSensitivity = 2000 / 32768.0f;
        break;
    case AFS_4G:
        accSensitivity = 4000 / 32768.0f;
        break;
    case AFS_8G:
        accSensitivity = 8000 / 32768.0f;
        break;
    case AFS_16G:
        accSensitivity = 16000 / 32768.0f;
        break;
    }

    return accSensitivity;
}

float bsp_Icm42688GetGres(uint8_t Gscale)
{
    switch(Gscale)
    {
    case GFS_15_125DPS:
        gyroSensitivity = 15.125f / 32768.0f;
        break;
    case GFS_31_25DPS:
        gyroSensitivity = 31.25f / 32768.0f;
        break;
    case GFS_62_5DPS:
        gyroSensitivity = 62.5f / 32768.0f;
        break;
    case GFS_125DPS:
        gyroSensitivity = 125.0f / 32768.0f;
        break;
    case GFS_250DPS:
        gyroSensitivity = 250.0f / 32768.0f;
        break;
    case GFS_500DPS:
        gyroSensitivity = 500.0f / 32768.0f;
        break;
    case GFS_1000DPS:
        gyroSensitivity = 1000.0f / 32768.0f;
        break;
    case GFS_2000DPS:
        gyroSensitivity = 2000.0f / 32768.0f;
        break;
    }
    return gyroSensitivity;
}

/*******************************************************************************
* ��    �ƣ� bsp_Icm42688RegCfg
* ��    �ܣ� Icm42688 �Ĵ�������
* ��ڲ����� ��
* ���ڲ����� ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע��
*******************************************************************************/
int8_t bsp_Icm42688RegCfg(void)
{
    uint8_t reg_val = 0;
    /* ��ȡ who am i �Ĵ��� */
    reg_val = icm42688_read_reg(ICM42688_WHO_AM_I);
		//printf("reg_val:%d\n",reg_val);
    icm42688_write_reg(ICM42688_REG_BANK_SEL, 0); //����bank 0����Ĵ���
    icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x01); //��λ������
    //ICM42688DelayMs(1);
    HAL_Delay(100);

    if(reg_val == ICM42688_ID)
    {
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 1); //����bank 1����Ĵ���
        icm42688_write_reg(ICM42688_INTF_CONFIG4, 0x02); //����Ϊ4��SPIͨ��

        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0); //����bank 0����Ĵ���
        icm42688_write_reg(ICM42688_FIFO_CONFIG, 0x40); //Stream-to-FIFO Mode(page63)


        reg_val = icm42688_read_reg(ICM42688_INT_SOURCE0);
        icm42688_write_reg(ICM42688_INT_SOURCE0, 0x00);
        icm42688_write_reg(ICM42688_FIFO_CONFIG2, 0x00); // watermark
        icm42688_write_reg(ICM42688_FIFO_CONFIG3, 0x02); // watermark
        icm42688_write_reg(ICM42688_INT_SOURCE0, reg_val);
        icm42688_write_reg(ICM42688_FIFO_CONFIG1, 0x63); // Enable the accel and gyro to the FIFO

        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        icm42688_write_reg(ICM42688_INT_CONFIG, 0x36);

        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_INT_SOURCE0);
        reg_val |= (1 << 2); //FIFO_THS_INT1_ENABLE
        icm42688_write_reg(ICM42688_INT_SOURCE0, reg_val);

        bsp_Icm42688GetAres(AFS_8G);
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_ACCEL_CONFIG0);//page74
        reg_val |= (AFS_8G << 5);   //���� ��8g
        reg_val |= (AODR_1000Hz);     //������� 50HZ
        icm42688_write_reg(ICM42688_ACCEL_CONFIG0, reg_val);

        bsp_Icm42688GetGres(GFS_1000DPS);
        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_GYRO_CONFIG0);//page73
        reg_val |= (GFS_1000DPS << 5);   //���� ��1000dps
        reg_val |= (GODR_1000Hz);     //������� 50HZ
        icm42688_write_reg(ICM42688_GYRO_CONFIG0, reg_val);

        icm42688_write_reg(ICM42688_REG_BANK_SEL, 0x00);
        reg_val = icm42688_read_reg(ICM42688_PWR_MGMT0); //��ȡPWR��MGMT0��ǰ�Ĵ�����ֵ(page72)
        reg_val &= ~(1 << 5);//ʹ���¶Ȳ���
        reg_val |= ((3) << 2);//����GYRO_MODE  0:�ر� 1:���� 2:Ԥ�� 3:������
        reg_val |= (3);//����ACCEL_MODE 0:�ر� 1:�ر� 2:�͹��� 3:������
        icm42688_write_reg(ICM42688_PWR_MGMT0, reg_val);
        HAL_Delay(100); //������PWR��MGMT0�Ĵ����� 200us�ڲ������κζ�д�Ĵ����Ĳ���

        return 0;
    }
    return -1;
}
/*******************************************************************************
* ��    �ƣ� bsp_Icm42688Init
* ��    �ܣ� Icm42688 ��������ʼ��
* ��ڲ����� ��
* ���ڲ����� 0: ��ʼ���ɹ�  ����ֵ: ��ʼ��ʧ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע��
*******************************************************************************/
int8_t bsp_Icm42688Init(void)
{
//    bsp_IcmSpixCsInit();
		 //SPI_Init();

    return(bsp_Icm42688RegCfg());

}

/*******************************************************************************
* ��    �ƣ� bsp_IcmGetTemperature
* ��    �ܣ� ��ȡIcm42688 �ڲ��������¶�
* ��ڲ����� ��
* ���ڲ����� ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� datasheet page62
*******************************************************************************/
int8_t bsp_IcmGetTemperature(int16_t* pTemp)
{
    uint8_t buffer[2] = {0};

    icm42688_read_regs(ICM42688_TEMP_DATA1, buffer, 2);

    *pTemp = (int16_t)(((int16_t)((buffer[0] << 8) | buffer[1])) / 132.48 + 25);
    return 0;
}

/*******************************************************************************
* ��    �ƣ� bsp_IcmGetAccelerometer
* ��    �ܣ� ��ȡIcm42688 ���ٶȵ�ֵ
* ��ڲ����� ������ٶȵ�ֵ
* ���ڲ����� ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� datasheet page62
*******************************************************************************/
int8_t bsp_IcmGetAccelerometer(icm42688RawData_t* accData)
{
    uint8_t buffer[6] = {0};

    icm42688_read_regs(ICM42688_ACCEL_DATA_X1, buffer, 6);

    accData->hx = ((uint16_t)buffer[0] << 8) | buffer[1];
    accData->hy = ((uint16_t)buffer[2] << 8) | buffer[3];
    accData->hz = ((uint16_t)buffer[4] << 8) | buffer[5];

    accData->x = (float)accData->hx * accSensitivity;
    accData->y = (float)accData->hy * accSensitivity;
    accData->z = (float)accData->hz * accSensitivity;

    return 0;
}

/*******************************************************************************
* ��    �ƣ� bsp_IcmGetGyroscope
* ��    �ܣ� ��ȡIcm42688 �����ǵ�ֵ
* ��ڲ����� ���������ǵ�ֵ
* ���ڲ����� ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� datasheet page63
*******************************************************************************/
int8_t bsp_IcmGetGyroscope(icm42688RawData_t* GyroData)
{
    uint8_t buffer[6] = {0};

    icm42688_read_regs(ICM42688_GYRO_DATA_X1, buffer, 6);

    GyroData->hx = ((uint16_t)buffer[0] << 8) | buffer[1];
    GyroData->hy = ((uint16_t)buffer[2] << 8) | buffer[3];
    GyroData->hz = ((uint16_t)buffer[4] << 8) | buffer[5];

    GyroData->x = (float)GyroData->hx * gyroSensitivity;
    GyroData->y = (float)GyroData->hy * gyroSensitivity;
    GyroData->z = (float)GyroData->hz * gyroSensitivity;
    return 0;
}

/*******************************************************************************
* ��    �ƣ� bsp_IcmGetRawData
* ��    �ܣ� ��ȡIcm42688���ٶ�����������
* ��ڲ����� ����
* ���ڲ����� ��
* �������ߣ� Baxiange
* �������ڣ� 2022-07-25
* ��    �ģ�
* �޸����ڣ�
* ��    ע�� datasheet page62,63
*******************************************************************************/
int8_t bsp_IcmGetRawData(icm42688RawData_t* accData, icm42688RawData_t* GyroData)
{
    uint8_t buffer[12] = {0};

    icm42688_read_regs(ICM42688_ACCEL_DATA_X1, buffer, 12);

    accData->x  = ((uint16_t)buffer[0] << 8)  | buffer[1];
    accData->y  = ((uint16_t)buffer[2] << 8)  | buffer[3];
    accData->z  = ((uint16_t)buffer[4] << 8)  | buffer[5];
    GyroData->x = ((uint16_t)buffer[6] << 8)  | buffer[7];
    GyroData->y = ((uint16_t)buffer[8] << 8)  | buffer[9];
    GyroData->z = ((uint16_t)buffer[10] << 8) | buffer[11];


    accData->x = (int16_t)(accData->x * accSensitivity);
    accData->y = (int16_t)(accData->y * accSensitivity);
    accData->z = (int16_t)(accData->z * accSensitivity);

    GyroData->x = (int16_t)(GyroData->x * gyroSensitivity);
    GyroData->y = (int16_t)(GyroData->y * gyroSensitivity);
    GyroData->z = (int16_t)(GyroData->z * gyroSensitivity);

    return 0;
}

uint8_t icm42688_READ_DATA_RDY_INT(void)
{
	uint8_t flag;
 flag=icm42688_read_reg(ICM42688_INT_STATUS);
	if((flag&0x08)==0x08)
	{	
		return 1;
	}

	 return 0;
}
