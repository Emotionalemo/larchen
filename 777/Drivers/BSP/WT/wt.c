#include "./BSP/WT/wt.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/IIC/myiic.h"


uint8_t WT_write(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length)
{
    uint16_t count=0;
    iic_start();
    iic_send_byte(dev);/* IIC发送一个字节 */
    iic_wait_ack();
    iic_send_byte(reg);/* IIC发送一个字节 */
    iic_wait_ack();
    for(count=0; count<length; count++)
    {
        iic_send_byte(data[count]);
        iic_wait_ack();
    }
    iic_stop();
    
    return 1;
}


uint8_t WT_read(uint8_t dev, uint8_t reg, uint8_t *data, uint32_t length)
{
    uint16_t count=0;
    iic_start();
    iic_send_byte(dev<<1);/* IIC发送一个字节 */
    iic_wait_ack();
    iic_send_byte(reg);/* IIC发送一个字节 */
    iic_wait_ack();
    iic_start();
    iic_send_byte(dev<<1|1);/* IIC发送一个字节 */
    iic_wait_ack();
    for(count=0; count<length; count++)
    {
        if(count!=length-1)
        {
            data[count]=iic_read_byte(1);
        }
        else  
        {
            data[count]=iic_read_byte(0);
        }
    }
    iic_stop();
    
    return 1;
}

