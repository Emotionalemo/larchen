/**
 ****************************************************************************************************
 * @file        myiic.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       IIC 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32H750开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200329
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/IIC/myiic.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/IIC/OLED_Font.h"
/**
 * @brief       初始化IIC
 * @param       无
 * @retval      无
 */
void iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    IIC_SCL_GPIO_CLK_ENABLE();  /* SCL引脚时钟使能 */
    IIC_SDA_GPIO_CLK_ENABLE();  /* SDA引脚时钟使能 */

    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;        /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* 快速 */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);/* SCL */

    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;        /* 推挽输出 */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);/* SDA */
    /* SDA引脚模式设置,开漏输出,上拉, 这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平 */

    iic_stop();     /* 停止总线上所有设备 */
	
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
 * @brief       IIC延时函数,用于控制IIC读写速度
 * @param       无
 * @retval      无
 */
static void iic_delay(void)
{
    delay_us(1);    /* 2us的延时, 读写速度在250Khz以内 *///363
}

/**
 * @brief       产生IIC起始信号
 * @param       无
 * @retval      无
 */
void iic_start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(0);     /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
    iic_delay();
    IIC_SCL(0);     /* 钳住I2C总线，准备发送或接收数据 */
    iic_delay();
}

/**
 * @brief       产生IIC停止信号
 * @param       无
 * @retval      无
 */
void iic_stop(void)
{
    IIC_SDA(0);     /* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(1);     /* 发送I2C总线结束信号 */
    iic_delay();
}

/**
 * @brief       等待应答信号到来
 * @param       无
 * @retval      1，接收应答失败
 *              0，接收应答成功
 */
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    IIC_SDA(1);     /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    iic_delay();
    IIC_SCL(1);     /* SCL=1, 此时从机可以返回ACK */
    iic_delay();

    while (IIC_READ_SDA)    /* 等待应答 */
    {
        waittime++;

        if (waittime > 250)
        {
            iic_stop();
            rack = 1;
            break;
        }
    }

    IIC_SCL(0);     /* SCL=0, 结束ACK检查 */
    iic_delay();
    return rack;
}

/**
 * @brief       产生ACK应答
 * @param       无
 * @retval      无
 */
void iic_ack(void)
{
    IIC_SDA(0);     /* SCL 0 -> 1  时 SDA = 0,表示应答 */
    iic_delay();
    IIC_SCL(1);     /* 产生一个时钟 */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
    IIC_SDA(1);     /* 主机释放SDA线 */
    iic_delay();
}

/**
 * @brief       不产生ACK应答
 * @param       无
 * @retval      无
 */
void iic_nack(void)
{
    IIC_SDA(1);     /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
    iic_delay();
    IIC_SCL(1);     /* 产生一个时钟 */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}

/**
 * @brief       IIC发送一个字节
 * @param       data: 要发送的数据
 * @retval      无
 */
void iic_send_byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        IIC_SDA((data & 0x80) >> 7);    /* 高位先发送 */
        iic_delay();
        IIC_SCL(1);
        iic_delay();
        IIC_SCL(0);
        data <<= 1;     /* 左移1位,用于下一次发送 */
    }
    IIC_SDA(1);         /* 发送完成, 主机释放SDA线 */
}

/**
 * @brief       IIC读取一个字节
 * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* 接收1个字节数据 */
    {
        receive <<= 1;  /* 高位先输出,所以先收到的数据位要左移 */
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
        iic_nack();     /* 发送nACK */
    }
    else
    {
        iic_ack();      /* 发送ACK */
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

void oled_write_byte(uint8_t data,uint8_t mode)//mode 为真则发送数据，否则发送命令数据
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
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
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
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	oled_pos_set((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		oled_write_byte(OLED_F8x16[Char - ' '][i],DATA);			//显示上半部分内容
	}
	oled_pos_set((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		oled_write_byte(OLED_F8x16[Char - ' '][i + 8],DATA);		//显示下半部分内容
	}
}
/**
  * @brief  OLED显示一个中文字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~8
  * @param  Char 要显示的一个中文字符顺序，范围：参见OLED_Font.h
  * @retval 无
  */
void OLED_ShowOneChinese(uint8_t Line, uint8_t Column,int Char)
{      	
	uint8_t i;
	oled_pos_set((Line - 1) * 2, (Column - 1) * 16);		//设置光标位置在上半部分
	for (i = 0; i < 16; i++)
	{
		oled_write_byte(OLED_F16x16[Char][i],DATA);			//显示上半部分内容
	}
	oled_pos_set((Line - 1) * 2 + 1, (Column - 1) * 16);	//设置光标位置在下半部分
	for (i = 0; i < 16; i++)
	{
		oled_write_byte(OLED_F16x16[Char][i + 16],DATA);		//显示下半部分内容
	}
}
/**
  * @brief  OLED显示中文字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~8
  * @param  String 要显示的字符串，范围：参见OLED_Font.h可见字符
  * @retval 无
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
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
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
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
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
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
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
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
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
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
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
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
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







