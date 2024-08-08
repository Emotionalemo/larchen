#include "./BSP/BTIM/btim.h"
#include "./BSP/CONTROL/control.h"
#include "./BSP/WT/wt.h"
#include "./BSP/NEWIIC/newiic.h"
#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"

TIM_HandleTypeDef g_timx_handler;         /* 定时器参数句柄 */
int flag=0;
short rollss=0;
short pitchss=0;
short yawss;
uint8_t S[10];
extern ADC_HandleTypeDef g_adc_handle;   /* ADC句柄 */
extern uint16_t adcx;
/**
 * @brief       基本定时器TIMX定时中断初始化函数
 * @note
 *              基本定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              基本定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟 = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr : 自动重装值。
 * @param       psc : 时钟预分频数
 * @retval      无
 */
void btim_timx_int_init(uint16_t arr, uint16_t psc)
{
    g_timx_handler.Instance = BTIM_TIMX_INT;                      /* 定时器x */
    g_timx_handler.Init.Prescaler = psc;                          /* 分频 */
    g_timx_handler.Init.CounterMode = TIM_COUNTERMODE_UP;         /* 递增计数模式 */
    g_timx_handler.Init.Period = arr;                             /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx_handler);
    
    HAL_TIM_Base_Start_IT(&g_timx_handler);                       /* 使能定时器x和定时器更新中断 */
}

/**
 * @brief       定时器底层驱动，开启时钟，设置中断优先级
                此函数会被HAL_TIM_Base_Init()函数调用
 * @param       无
 * @retval      无
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == BTIM_TIMX_INT)
    {
        BTIM_TIMX_INT_CLK_ENABLE();                     /* 使能TIMx时钟 */
        HAL_NVIC_SetPriority(BTIM_TIMX_INT_IRQn, 1, 3); /* 抢占1，子优先级3 */
        HAL_NVIC_EnableIRQ(BTIM_TIMX_INT_IRQn);         /* 开启ITMx中断 */
    }
}

/**
 * @brief       基本定时器TIMX中断服务函数
 * @param       无
 * @retval      无
 */
void BTIM_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handler);  /* 定时器回调函数 */
}

/**
 * @brief       回调函数，定时器中断服务函数调用
 * @param       无
 * @retval      无
 */
__I2C_Status_TypeDef i2CStatus;
unsigned char RxData[10];          //IIC接口接收数据区
unsigned int  tof_1 = 1;         //读取到TOF250的具体数值
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
//       adcx = adc_get_result_average(ADC_CHANNEL_5, 1);    /* 获取通道5的转换值，10次取平均 */
//       temp5 = (float)adcx * (3.3 / 4096);                  /* 获取计算后的带小数的实际电压值，比如3.1111 */
//			 adcx = adc_get_result_average(ADC_CHANNEL_3, 1);    /* 获取通道5的转换值，10次取平均 */
//       temp3 = (float)adcx * (3.3 / 4096);                  /* 获取计算后的带小数的实际电压值，比如3.1111 */
//			 adcx = adc_get_result_average(ADC_CHANNEL_4, 1);    /* 获取通道5的转换值，10次取平均 */
//       temp4 = (float)adcx * (3.3 / 4096);                  /* 获取计算后的带小数的实际电压值，比如3.1111 */
//      
    }
	}