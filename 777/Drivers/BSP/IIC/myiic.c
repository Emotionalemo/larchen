/**
 ****************************************************************************************************
 * @file        myiic.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       IIC ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200329
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/IIC/myiic.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/IIC/OLED_Font.h"
/**
 * @brief       ��ʼ��IIC
 * @param       ��
 * @retval      ��
 */
void iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    IIC_SCL_GPIO_CLK_ENABLE();  /* SCL����ʱ��ʹ�� */
    IIC_SDA_GPIO_CLK_ENABLE();  /* SDA����ʱ��ʹ�� */

    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* ���� */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);/* SCL */

    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;        /* ������� */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);/* SDA */
    /* SDA����ģʽ����,��©���,����, �����Ͳ���������IO������, ��©�����ʱ��(=1), Ҳ���Զ�ȡ�ⲿ�źŵĸߵ͵�ƽ */

    iic_stop();     /* ֹͣ�����������豸 */
	
	  delay_ms(200);
	
	  oled_write_byte(0xAE,COMMAND);
		oled_write_byte(0xD5,COMMAND);
		oled_write_byte(0x80,COMMAND);
		oled_write_byte(0xA8,COMMAND);
		oled_write_byte(0x3F,COMMAND);
		oled_write_byte(0xD3,COMMAND);
	  oled_write_byte(0x00,COMMAND);
		oled_write_byte(0x40,COMMAND);
		oled_write_byte(0xA1,COMMAND);
		oled_write_byte(0xC8,COMMAND);
		oled_write_byte(0xDA,COMMAND);
		oled_write_byte(0x12,COMMAND);
		oled_write_byte(0x81,COMMAND);
		oled_write_byte(0xCF,COMMAND);
		oled_write_byte(0xD9,COMMAND);
		oled_write_byte(0xF1,COMMAND);
		oled_write_byte(0xDB,COMMAND);
		oled_write_byte(0x30,COMMAND);
		oled_write_byte(0xA4,COMMAND);
		oled_write_byte(0xA6,COMMAND);
		oled_write_byte(0x8D,COMMAND);
		oled_write_byte(0x14,COMMAND);
		oled_write_byte(0xAF,COMMAND);

    oled_clear();
		
}

/**
 * @brief       IIC��ʱ����,���ڿ���IIC��д�ٶ�
 * @param       ��
 * @retval      ��
 */
static void iic_delay(void)
{
    delay_us(1);    /* 2us����ʱ, ��д�ٶ���250Khz���� *///363
}

/**
 * @brief       ����IIC��ʼ�ź�
 * @param       ��
 * @retval      ��
 */
void iic_start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(0);     /* START�ź�: ��SCLΪ��ʱ, SDA�Ӹ߱�ɵ�, ��ʾ��ʼ�ź� */
    iic_delay();
    IIC_SCL(0);     /* ǯסI2C���ߣ�׼�����ͻ�������� */
    iic_delay();
}

/**
 * @brief       ����IICֹͣ�ź�
 * @param       ��
 * @retval      ��
 */
void iic_stop(void)
{
    IIC_SDA(0);     /* STOP�ź�: ��SCLΪ��ʱ, SDA�ӵͱ�ɸ�, ��ʾֹͣ�ź� */
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(1);     /* ����I2C���߽����ź� */
    iic_delay();
}

/**
 * @brief       �ȴ�Ӧ���źŵ���
 * @param       ��
 * @retval      1������Ӧ��ʧ��
 *              0������Ӧ��ɹ�
 */
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    IIC_SDA(1);     /* �����ͷ�SDA��(��ʱ�ⲿ������������SDA��) */
    iic_delay();
    IIC_SCL(1);     /* SCL=1, ��ʱ�ӻ����Է���ACK */
    iic_delay();

    while (IIC_READ_SDA)    /* �ȴ�Ӧ�� */
    {
        waittime++;

        if (waittime > 250)
        {
            iic_stop();
            rack = 1;
            break;
        }
    }

    IIC_SCL(0);     /* SCL=0, ����ACK��� */
    iic_delay();
    return rack;
}

/**
 * @brief       ����ACKӦ��
 * @param       ��
 * @retval      ��
 */
void iic_ack(void)
{
    IIC_SDA(0);     /* SCL 0 -> 1  ʱ SDA = 0,��ʾӦ�� */
    iic_delay();
    IIC_SCL(1);     /* ����һ��ʱ�� */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
    IIC_SDA(1);     /* �����ͷ�SDA�� */
    iic_delay();
}

/**
 * @brief       ������ACKӦ��
 * @param       ��
 * @retval      ��
 */
void iic_nack(void)
{
    IIC_SDA(1);     /* SCL 0 -> 1  ʱ SDA = 1,��ʾ��Ӧ�� */
    iic_delay();
    IIC_SCL(1);     /* ����һ��ʱ�� */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       data: Ҫ���͵�����
 * @retval      ��
 */
void iic_send_byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((data & 0x80) >> 7);    /* ��λ�ȷ��� */
        iic_delay();
        IIC_SCL(1);
        iic_delay();
        IIC_SCL(0);
        data <<= 1;     /* ����1λ,������һ�η��� */
    }
    IIC_SDA(1);         /* �������, �����ͷ�SDA�� */
}

/**
 * @brief       IIC��ȡһ���ֽ�
 * @param       ack:  ack=1ʱ������ack; ack=0ʱ������nack
 * @retval      ���յ�������
 */
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* ����1���ֽ����� */
    {
        receive <<= 1;  /* ��λ�����,�������յ�������λҪ���� */
        IIC_SCL(1);
        iic_delay();

        if (IIC_READ_SDA)
        {
            receive++;
        }
        
        IIC_SCL(0);
        iic_delay();
    }

    if (!ack)
    {
        iic_nack();     /* ����nACK */
    }
    else
    {
        iic_ack();      /* ����ACK */
    }

    return receive;
}


void iic_write_command(uint8_t iic_command)
{
	iic_start();
	iic_send_byte(0x78);
	iic_wait_ack();
	iic_send_byte(0x00);
	iic_wait_ack();
  iic_send_byte(iic_command);
  iic_wait_ack();
  iic_stop();	
}

void iic_write_data(uint8_t iic_data)
{
	iic_start();
	iic_send_byte(0x78);
	iic_wait_ack();
	iic_send_byte(0x40);
	iic_wait_ack();
	iic_send_byte(iic_data);
	iic_wait_ack();
	iic_stop();
}

void oled_write_byte(uint8_t data,uint8_t mode)//mode Ϊ���������ݣ���������������
{
	if(mode)
	{
		iic_write_data(data);
	}
	else
	{
		iic_write_command(data);
	}
}

/**
  * @brief  OLED���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  * @retval ��
  */
void oled_pos_set(uint8_t y,uint8_t x)//OLED_SetCursor(uint8_t Y, uint8_t X)
{
	oled_write_byte(0xB0 | y,COMMAND);
	oled_write_byte(0x10 | ((x & 0xF0) >> 4),COMMAND);
	oled_write_byte(0x00 | (x & 0x0F),COMMAND);
}


void oled_open_display(void)
{
	oled_write_byte(0x8d,COMMAND);
	oled_write_byte(0x14,COMMAND);
	oled_write_byte(0xaf,COMMAND);
}

void oled_off_display(void)
{
	oled_write_byte(0x8d,COMMAND);
	oled_write_byte(0x10,COMMAND);
	oled_write_byte(0xae,COMMAND);
}

void oled_clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		oled_write_byte(0xb0+i,COMMAND);
		oled_write_byte(0xb0+i,COMMAND);
		oled_write_byte(0xb0+i,COMMAND);
		for(n=0;n<128;n++)
			oled_write_byte(0,DATA);
	}
}

void oled_fill(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{
		oled_write_byte(0xb0+i,COMMAND);
		oled_write_byte(0x00,COMMAND);
		oled_write_byte(0x10,COMMAND);
		for(n=0;n<128;n++)
		oled_write_byte(0xff,DATA);
	}
}


/**
  * @brief  OLED��ʾһ���ַ�
  * @param  Line ��λ�ã���Χ��1~4
  * @param  Column ��λ�ã���Χ��1~16
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	oled_pos_set((Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 8; i++)
	{
		oled_write_byte(OLED_F8x16[Char - ' '][i],DATA);			//��ʾ�ϰ벿������
	}
	oled_pos_set((Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
	for (i = 0; i < 8; i++)
	{
		oled_write_byte(OLED_F8x16[Char - ' '][i + 8],DATA);		//��ʾ�°벿������
	}
}
/**
  * @brief  OLED��ʾһ�������ַ�
  * @param  Line ��λ�ã���Χ��1~4
  * @param  Column ��λ�ã���Χ��1~8
  * @param  Char Ҫ��ʾ��һ�������ַ�˳�򣬷�Χ���μ�OLED_Font.h
  * @retval ��
  */
void OLED_ShowOneChinese(uint8_t Line, uint8_t Column,int Char)
{      	
	uint8_t i;
	oled_pos_set((Line - 1) * 2, (Column - 1) * 16);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 16; i++)
	{
		oled_write_byte(OLED_F16x16[Char][i],DATA);			//��ʾ�ϰ벿������
	}
	oled_pos_set((Line - 1) * 2 + 1, (Column - 1) * 16);	//���ù��λ�����°벿��
	for (i = 0; i < 16; i++)
	{
		oled_write_byte(OLED_F16x16[Char][i + 16],DATA);		//��ʾ�°벿������
	}
}
/**
  * @brief  OLED��ʾ�����ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~8
  * @param  String Ҫ��ʾ���ַ�������Χ���μ�OLED_Font.h�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChinese(uint8_t Line, uint8_t Column, int *String,int n)
{
	uint8_t i;
	for (i = 0; String[i] !=-1; i++)
	{delay_ms((n-1)*200);
		OLED_ShowOneChinese(Line, Column + i, String[i]);
	}
}
/**
  * @brief  OLED��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, long Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

void oled_pot_display(uint32_t x,uint32_t y)
{
	uint32_t page,data;
	page=7-y/8;
	data=y-(y/8)*8;
	page=(uint8_t)page;
	x=(uint8_t)x;
	oled_pos_set(x,page); 
	switch(data)
	
	{
		case 1:oled_write_byte(0x80,DATA);break;
		case 2:oled_write_byte(0x40,DATA);break;
		case 3:oled_write_byte(0x20,DATA);break;
		case 4:oled_write_byte(0x10,DATA);break;
		case 5:oled_write_byte(0x08,DATA);break;
		case 6:oled_write_byte(0x04,DATA);break;
		case 7:oled_write_byte(0x02,DATA);break;
		case 8:oled_write_byte(0x01,DATA);break;
	}
	
	
}







