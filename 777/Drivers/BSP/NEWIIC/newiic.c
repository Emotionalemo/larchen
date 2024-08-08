#include "./BSP/NEWIIC/newiic.h"
void SCL_init1();
void SDA_init1();
void SDA_IN();
void SDA_OUT();
void User_I2C_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//  __HAL_RCC_GPIOB_CLK_ENABLE();	
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//	GPIO_InitStructure.Pin = GPIO_PIN_6;
//	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP ;   //推挽输出
//	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//	/*对SDA进行开漏输出和上拉操作*/
//	GPIO_InitStructure.Pin=GPIO_PIN_7;
//	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_OD;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	SCL_init1();
	SDA_init1();
	HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT1,IIC_SCL_GPIO_PIN1,GPIO_PIN_SET); 	//PB6,PB7 输出高
	HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT1,IIC_SDA_GPIO_PIN1,GPIO_PIN_SET);
}
void SCL_init1(){
	GPIO_InitTypeDef GPIO_InitStructure;
	IIC_SCL_GPIO_CLK_ENABLE1();	
  GPIO_InitStructure.Pin = IIC_SCL_GPIO_PIN1;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP ;   //推挽输出
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(IIC_SCL_GPIO_PORT1, &GPIO_InitStructure);
}
void SDA_init1(){
	GPIO_InitTypeDef GPIO_InitStructure1;
  IIC_SDA_GPIO_CLK_ENABLE1();
	GPIO_InitStructure1.Pin=IIC_SDA_GPIO_PIN1;
	GPIO_InitStructure1.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure1.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure1.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(IIC_SDA_GPIO_PORT1, &GPIO_InitStructure1);
}
/*************************************************
Function: delay
Description: 
Input:  Delaycnt - delay cnt
*************************************************/
void delay(uint32_t Delaycnt)
{
	uint32_t i;
	
	for(i = 0; i<Delaycnt; i++)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	}
}

/*************************************************
Function: I2C_GenerateStart
Description: 
*************************************************/
static void I2C_GenerateStart(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA1(1);	  	  
	IIC_SCL1(1);
    delay(5);
   IIC_SDA1(0);//START:when CLK is high,DATA change form high to low 
    delay(5);
    IIC_SCL1(0);//钳住I2C总线，准备发送或接收数据 
    delay(5);
}
/*************************************************
Function: I2C_GenerateStop
Description: 
*************************************************/
static void I2C_GenerateStop(void)
{
	IIC_SCL1(0);
	IIC_SDA1(0);//STOP:when CLK is high DATA change form low to high
    delay(5);  
	IIC_SCL1(1); 
    delay(5); 
	IIC_SDA1(1);  
    delay(5); 
}
/*************************************************
Function: I2C_GenerateAck
Description: 
*************************************************/
static void I2C_GenerateAck(__I2C_ACK_TypeDef ack)
{
    if(ack == I2C_NACK) 
	{
        IIC_SDA1(1);
	}
    else
	{
        IIC_SDA1(0);;
	}
	
    delay(5);		
    IIC_SCL1(1);
    delay(5);
    IIC_SCL1(0);
		delay(5);
}
/*************************************************
Function: I2C_RecvAck
Description: 
*************************************************/
static __I2C_ACK_TypeDef I2C_RecvAck(uint32_t Timeout)
{
	uint32_t timeout = 0;
    __I2C_ACK_TypeDef ackStatus;
	
	IIC_SDA1(1);
	SDA_IN();
    delay(5);
	IIC_SCL1(1);
    delay(5);
	
	while(1)
	{
		ackStatus = (READ_SDA == 0) ? I2C_ACK : I2C_NACK;
		if(ackStatus == I2C_ACK)
		{
			break;
		}
		
		if((timeout++) > Timeout)
		{
			ackStatus = I2C_NACK;
			break;
		}
	}

    IIC_SCL1(0);
    delay(5);
    //SDA_OUT();
    return ackStatus;  
}
/*************************************************
Function: I2C_SendByte
Description: 
*************************************************/
static __I2C_ACK_TypeDef I2C_SendByte(uint8_t data, uint32_t Timeout)
{
    uint8_t i;
	
    IIC_SCL1(0);
	
    for (i = 0; i < 8; i++)      
    {
				SDA_OUT();
        if((data << i)&0x80)
        {
            IIC_SDA1(1);
        }
        else 
        {
            IIC_SDA1(0);;
        }
        delay(15); 
        IIC_SCL1(1);               
        delay(15);             
        IIC_SCL1(0);    
        delay(15);   
    }

    return I2C_RecvAck(Timeout);
}
/*************************************************
Function: I2C_RecvByte
Description: 
*************************************************/
static uint8_t I2C_RecvByte(void)
{
    uint8_t i;
    uint8_t data = 0, BitStatus = 0;
    
    IIC_SDA1(1);
		SDA_IN();
    delay(15);
    for (i = 0; i < 8; i++)
    {  
        data <<= 1;
        IIC_SCL1(1); 
        delay(15); 
		
        BitStatus = (READ_SDA == 1) ? 1 : 0;
        data |= BitStatus;     
		
        IIC_SCL1(0);
        delay(15);
    }
	
	SDA_OUT();
    return data;
}
/*************************************************
Function: I2C_DummyWrite
Description: I2C DummyWrite
*************************************************/
static __I2C_Status_TypeDef I2C_DummyWrite(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t OperLen, uint32_t Timeout)
{
	/* Generate Start Sign */
	I2C_GenerateStart();
	
	/* Send Slave Address */
	if(I2C_SendByte((SlaveAddr<<1) | I2C_Transmitter, Timeout) != I2C_ACK)
	{
		I2C_GenerateStop();
		return I2C_ERROR;
	}

//	/* Send RegAddr_H */
//	if(I2C_SendByte((RegAddr >> 8) & 0xFF, Timeout) != I2C_ACK)
//	{
//		I2C_GenerateStop();
//		return I2C_ERROR;
//	}
	
//	/* Send RegAddr_L */
//	if(I2C_SendByte(RegAddr & 0xFF, Timeout) != I2C_ACK)
//	{
//		I2C_GenerateStop();
//		return I2C_ERROR;
//	}
//	
		/* Send RegAddr */
	if(I2C_SendByte(RegAddr , Timeout) != I2C_ACK)
	{
		I2C_GenerateStop();
		return I2C_ERROR;
	}
	
//	/* Send Data length */
//	if(I2C_SendByte(OperLen, Timeout) != I2C_ACK)
//	{
//		I2C_GenerateStop();
//		return I2C_ERROR;
//	}

	return I2C_OK;
}


/*************************************************
Function: I2C_RecvBytes
Description: I2C SendBytes
*************************************************/
 uint8_t I2C_RecvBytes(uint8_t SlaveAddr, uint8_t RegAddr, uint8_t *Rxbuf, uint8_t OperLen, uint32_t Timeout)
{
	uint8_t i = 0;
	
	/* Dummy Write */
	if(I2C_DummyWrite(SlaveAddr, RegAddr, OperLen, Timeout) != I2C_OK)
	{
		I2C_GenerateStop();
		return I2C_ERROR;
	}
	delay(100);

	/* Generate Start Sign */
	I2C_GenerateStart();
	
	/* Send Slave Address */
	if(I2C_SendByte((SlaveAddr<<1) | I2C_Receiver, Timeout) != I2C_ACK)
	{
		I2C_GenerateStop();
		return I2C_ERROR;
	}
	
	/* Receive Data */
	for(i = 0; i < OperLen - 1; i++)
	{
		Rxbuf[i] = I2C_RecvByte();
		I2C_GenerateAck(I2C_ACK);
	}

	Rxbuf[OperLen - 1] = I2C_RecvByte();
	I2C_GenerateAck(I2C_NACK);
	
	/* Generate Stop Sign */
	I2C_GenerateStop();
	
	return I2C_OK;
}
void SDA_IN(){
	GPIO_InitTypeDef GPIO_InitStructure1;
  IIC_SDA_GPIO_CLK_ENABLE1();
	GPIO_InitStructure1.Pin=IIC_SDA_GPIO_PIN1;
	GPIO_InitStructure1.Mode=GPIO_MODE_INPUT;
	GPIO_InitStructure1.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure1.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(IIC_SDA_GPIO_PORT1, &GPIO_InitStructure1);
}
void SDA_OUT(){
	GPIO_InitTypeDef GPIO_InitStructure1;
  IIC_SDA_GPIO_CLK_ENABLE1();
	GPIO_InitStructure1.Pin=IIC_SDA_GPIO_PIN1;
	GPIO_InitStructure1.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure1.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure1.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(IIC_SDA_GPIO_PORT1, &GPIO_InitStructure1);
}