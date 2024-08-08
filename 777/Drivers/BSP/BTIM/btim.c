#include "./BSP/BTIM/btim.h"
#include "./BSP/CONTROL/control.h"
#include "./BSP/WT/wt.h"
#include "./BSP/NEWIIC/newiic.h"
#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"

TIM_HandleTypeDef g_timx_handler;         /* ��ʱ��������� */
int flag=0;
short rollss=0;
short pitchss=0;
short yawss;
uint8_t S[10];
extern ADC_HandleTypeDef g_adc_handle;   /* ADC��� */
extern uint16_t adcx;
/**
 * @brief       ������ʱ��TIMX��ʱ�жϳ�ʼ������
 * @note
 *              ������ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ������ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ42M, ���Զ�ʱ��ʱ�� = 84Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr : �Զ���װֵ��
 * @param       psc : ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void btim_timx_int_init(uint16_t arr, uint16_t psc)
{
    g_timx_handler.Instance = BTIM_TIMX_INT;                      /* ��ʱ��x */
    g_timx_handler.Init.Prescaler = psc;                          /* ��Ƶ */
    g_timx_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* ��������ģʽ */
    g_timx_handler.Init.Period = arr;                             /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_timx_handler);
    
    HAL_TIM_Base_Start_IT(&g_timx_handler);                       /* ʹ�ܶ�ʱ��x�Ͷ�ʱ�������ж� */
}

/**
 * @brief       ��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
                �˺����ᱻHAL_TIM_Base_Init()��������
 * @param       ��
 * @retval      ��
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIMX_INT_CLK_ENABLE();                     /* ʹ��TIMxʱ�� */
        HAL_NVIC_SetPriority(BTIM_TIMX_INT_IRQn, 1, 3); /* ��ռ1�������ȼ�3 */
        HAL_NVIC_EnableIRQ(BTIM_TIMX_INT_IRQn);         /* ����ITMx�ж� */
    }
}

/**
 * @brief       ������ʱ��TIMX�жϷ�����
 * @param       ��
 * @retval      ��
 */
void BTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handler);  /* ��ʱ���ص����� */
}

/**
 * @brief       �ص���������ʱ���жϷ���������
 * @param       ��
 * @retval      ��
 */
__I2C_Status_TypeDef i2CStatus;
unsigned char RxData[10];          //IIC�ӿڽ���������
unsigned int  tof_1 = 1;         //��ȡ��TOF250�ľ�����ֵ
float temp3,temp4,temp5=0.00;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
       WT_read(WT931_address, 0x3d ,S, 6);
			 rollss=((short)(S[1]<<8|S[0]))/32768.0*180.0;
       pitchss=((short)(S[3]<<8|S[2]))/32768.0*180.0;
       yawss=((short)(S[5]<<8|S[4]))/32768.0*180.0;
//			 i2CStatus = I2C_RecvBytes(TOF250_ID,TOF250_DIST_H, &RxData[0], 2,100);
//			 tof_1 = RxData[1] + RxData[0]*256;	 
//       adcx = adc_get_result_average(ADC_CHANNEL_5, 1);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
//       temp5 = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
//			 adcx = adc_get_result_average(ADC_CHANNEL_3, 1);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
//       temp3 = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
//			 adcx = adc_get_result_average(ADC_CHANNEL_4, 1);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
//       temp4 = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
//      
    }
	}