#ifndef __NEWIIC_H
#define __NEWIIC_H

#include "./SYSTEM/sys/sys.h"

#define IIC_SCL_GPIO_PORT               GPIOD
#define IIC_SCL_GPIO_PIN                GPIO_PIN_2
#define IIC_SCL_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define IIC_SDA_GPIO_PORT               GPIOD
#define IIC_SDA_GPIO_PIN                GPIO_PIN_4
#define IIC_SDA_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */


//#define USER_I2C_SCL_PIN  GPIO_Pin_6
//#define USER_I2C_SDA_PIN  GPIO_Pin_7
//#define USER_I2C_GPIO     GPIOB

//IO方向设置
 
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

#define IIC_SCL(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SCL */
#define IIC_SDA(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SDA */
#define READ_SDA			HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN) /* 读取SDA */							
#define TOF250_DIST_H 0x00             //TOF250距离值高8位
#define TOF250_DIST_L 0x01             //TOF250距离值低8位
#define TOF250_READ_L    2             //TOF250读取的长度
#define TOF250_ID     0x52             //TOF250的IIC地址													
													
													
													
//IO操作函数	 
//#define IIC_SCL    PBout(6) //SCL
//#define IIC_SDA    PBout(7) //SDA	 
//#define READ_SDA   PBin(7)  //输入SDA 

typedef enum
{
	I2C_OK = 0U,
	I2C_BUSY = 1U,
	I2C_TIMEOUT = 3U,
	I2C_ERROR = 4U,
}__I2C_Status_TypeDef;

typedef enum
{
	I2C_ACK = 0U,
	I2C_NACK = 1U,
}__I2C_ACK_TypeDef;

typedef enum
{
	I2C_Transmitter = 0U,
	I2C_Receiver = 1U,
}__I2C_Oper_TypeDef;

typedef enum
{
	I2C_SDA_IN = 0U,
	I2C_SDA_OUT = 1U,
}__I2C_SDAMode_TypeDef;
/*************************************************
Function: User_I2C_Init
Description: config I2C 
*************************************************/
void User_I2C_Init(void);

/*************************************************
Function: delay
Description: 
Input:  Delaycnt - delay cnt
*************************************************/
void delay(uint32_t Delaycnt);

/*************************************************
Function: I2C_SendBytes
Description: I2C SendBytes
*************************************************/
__I2C_Status_TypeDef I2C_SendBytes(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *TxBuf, uint8_t OperLen, uint32_t Timeout);

/*************************************************
Function: I2C_RecvBytes
Description: I2C SendBytes
*************************************************/
__I2C_Status_TypeDef I2C_RecvBytes(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *Rxbuf, uint8_t OperLen, uint32_t Timeout);
unsigned char tof250_i2c_read(unsigned char tof_ID,unsigned char word_adress, unsigned char *rdata, unsigned char num);
#endif

                                   
																	 
																	 
														
