/**
 ****************************************************************************************************
 * @file        myiic.h
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
 
#ifndef __MYIIC_H
#define __MYIIC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ���� ���� */

#define IIC_SCL_GPIO_PORT               GPIOB
#define IIC_SCL_GPIO_PIN                GPIO_PIN_4
#define IIC_SCL_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */

#define IIC_SDA_GPIO_PORT               GPIOE
#define IIC_SDA_GPIO_PIN                GPIO_PIN_2
#define IIC_SDA_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */


#define DATA                        1
#define COMMAND                     0
/******************************************************************************************/

/* IO���� */
#define IIC_SCL(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SCL */

#define IIC_SDA(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SDA */

#define IIC_READ_SDA     HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN) /* ��ȡSDA */


/* IIC���в������� */
void iic_init(void);            /* ��ʼ��IIC��IO�� */
void iic_start(void);           /* ����IIC��ʼ�ź� */
void iic_stop(void);            /* ����IICֹͣ�ź� */
void iic_ack(void);             /* IIC����ACK�ź� */
void iic_nack(void);            /* IIC������ACK�ź� */
uint8_t iic_wait_ack(void);     /* IIC�ȴ�ACK�ź� */
void iic_send_byte(uint8_t txd);/* IIC����һ���ֽ� */
uint8_t iic_read_byte(unsigned char ack);/* IIC��ȡһ���ֽ� */

													
													
													
													
/*oled����*/
void iic_write_command(uint8_t iic_command);    //��дָ��
void iic_write_data(uint8_t iic_data);          //��д����
void oled_write_byte(uint8_t data,uint8_t mode);//������ݺ�ָ��Ȼ���͵���λ��
void oled_pos_set(uint8_t y,uint8_t x);         //���ù��
void oled_open_display(void);                   //oled����չʾ
void oled_off_display(void);                    //oledֹͣչʾ
void oled_clear(void);                          //����
void oled_fill(void);                           //���


void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowOneChinese(uint8_t Line, uint8_t Column,int Char);
void OLED_ShowChinese(uint8_t Line, uint8_t Column, int *String,int n);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, long Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void oled_pot_display(uint32_t x,uint32_t y);//��纯��������ط�Ĭ���º���Ϊx�ᣬ��y��Ϊy��
//^y
//1
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////������x
#endif

