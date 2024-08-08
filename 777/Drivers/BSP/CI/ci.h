#ifndef _CI_H
#define _CI_H
#include "./SYSTEM/sys/sys.h"
#ifdef __cplusplus
extern "C" {
#endif
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */
void IIC_Write(unsigned char dev_addr, unsigned char mem_addr ,unsigned char data);
void IIC_Read(unsigned char dev_addr, unsigned char mem_addr ,unsigned char *buf ,unsigned char len);
void Single_Write_HMC5883(unsigned char REG_Address, unsigned char REG_data);
unsigned char Single_Read_HMC5883 (unsigned char REG_Address);

void Init_HMC5883(void);
void Multiple_Read_HMC5883(void);
#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */