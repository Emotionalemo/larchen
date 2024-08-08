/**
 ****************************************************************************************************
 * @file        adc.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-18
 * @brief       ADC ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211018
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/LED/led.h"



ADC_HandleTypeDef g_adc_handle;   /* ADC��� */
  uint16_t adcx=0;
    float temp=0;

/**
 * @brief       ADC��ʼ������
 *   @note      ������֧��ADC1/ADC2����ͨ��, ���ǲ�֧��ADC3
 *              ����ʹ��12λ����, ADC����ʱ��=21M, ת��ʱ��Ϊ: �������� + 12��ADC����
 *              ��������������: 480, ��ת��ʱ�� = 492 ��ADC���� = 23.42us
 * @param       ��
 * @retval      ��
 */
void adc_init(void)
{
    g_adc_handle.Instance = ADC_ADCX;
    g_adc_handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;        /* 4��Ƶ��ADCCLK = PCLK2/4 = 84/4 = 21Mhz */
    g_adc_handle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 12λģʽ */
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* �Ҷ��� */
    g_adc_handle.Init.ScanConvMode = DISABLE;                               /* ��ɨ��ģʽ */
    g_adc_handle.Init.ContinuousConvMode = DISABLE;                         /* �ر�����ת�� */
    g_adc_handle.Init.NbrOfConversion = 1;                                  /* 1��ת���ڹ��������� Ҳ����ֻת����������1 */
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;                      /* ��ֹ����������ģʽ */
    g_adc_handle.Init.NbrOfDiscConversion = 0;                              /* ����������ͨ����Ϊ0 */
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* ������� */
    g_adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* ʹ��������� */
    g_adc_handle.Init.DMAContinuousRequests = DISABLE;                      /* �ر�DMA���� */
    HAL_ADC_Init(&g_adc_handle);                                            /* ��ʼ�� */
}

/**
 * @brief       ADC�ײ��������������ã�ʱ��ʹ��
                �˺����ᱻHAL_ADC_Init()����
 * @param       hadc:ADC���
 * @retval      ��
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC_ADCX)
    {
        GPIO_InitTypeDef gpio_init_struct;
        ADC_ADCX_CHY_CLK_ENABLE();      /* ʹ��ADCxʱ�� */
        ADC_ADCX_CHY_GPIO_CLK_ENABLE(); /* ����GPIOʱ�� */

        /* AD�ɼ�����ģʽ����,ģ������ */
        gpio_init_struct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        gpio_init_struct.Pull = GPIO_NOPULL;
			  gpio_init_struct.Alternate = GPIO_AF1_TIM2;

        HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT, &gpio_init_struct);
    }
}

/**
 * @brief       ����ADCͨ������ʱ��
 * @param       adcx : adc���ָ��,ADC_HandleTypeDef
 * @param       ch   : ͨ����, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       stime: ����ʱ��  0~7, ��Ӧ��ϵΪ:
 *   @arg       ADC_SAMPLETIME_3CYCLES,  3��ADCʱ������        ADC_SAMPLETIME_15CYCLES, 15��ADCʱ������
 *   @arg       ADC_SAMPLETIME_28CYCLES, 28��ADCʱ������       ADC_SAMPLETIME_56CYCLES, 56��ADCʱ������
 *   @arg       ADC_SAMPLETIME_84CYCLES, 84��ADCʱ������       ADC_SAMPLETIME_112CYCLES,112��ADCʱ������
 *   @arg       ADC_SAMPLETIME_144CYCLES,144��ADCʱ������      ADC_SAMPLETIME_480CYCLES,480��ADCʱ������
 * @param       rank: ��ͨ���ɼ�ʱ��Ҫ���õĲɼ����,
                �����㶨��channel1��rank=1��channel2��rank=2��
                ��ô��Ӧ����DMA����ռ�ı�������AdcDMA[0] ��i��channel1��ת�������AdcDMA[1]����ͨ��2��ת������� 
                ��ͨ��DMA����Ϊ ADC_REGULAR_RANK_1
 *   @arg       ���1~16��ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      ��
 */
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    /* ���ö�ӦADCͨ�� */
    ADC_ChannelConfTypeDef adc_channel;
    adc_channel.Channel = ch;               /* ����ADCX��ͨ��ch */
    adc_channel.Rank = rank;                /* ���ò������� */
    adc_channel.SamplingTime = stime;       /* ���ò���ʱ�� */
    HAL_ADC_ConfigChannel( adc_handle, &adc_channel);   
}

/**
 * @brief       ���ADCת����Ľ��
 * @param       ch: ͨ��ֵ 0~17��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_17
 * @retval      ��
 */
uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&g_adc_handle, ch, 1, ADC_SAMPLETIME_480CYCLES);   /* ����ͨ�������кͲ���ʱ�� */
    HAL_ADC_Start(&g_adc_handle);                                       /* ����ADC */
    HAL_ADC_PollForConversion(&g_adc_handle, 10);                       /* ��ѯת�� */

    return (uint16_t)HAL_ADC_GetValue(&g_adc_handle);                   /* �������һ��ADC1�������ת����� */
}

/**
 * @brief       ��ȡͨ��ch��ת��ֵ��ȡtimes��, Ȼ��ƽ��
 * @param       ch      : ͨ����, 0~17
 * @param       times   : ��ȡ����
 * @retval      ͨ��ch��times��ת�����ƽ��ֵ
 */
uint32_t adc_get_result_average(uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)     /* ��ȡtimes������ */
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }

    return temp_val / times;        /* ����ƽ��ֵ */
}

///**
// * @brief       KEY0 �ⲿ�жϷ������
// * @param       ��
// * @retval      ��
// */
//void KEY0_INT_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY0�����ж��� ���жϱ�־λ */
//    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
//}

///**
// * @brief       KEY1 �ⲿ�жϷ������
// * @param       ��
// * @retval      ��
// */
//void KEY1_INT_IRQHandler(void)
//{ 
//    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);         /* �����жϴ����ú��� ���KEY1�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
//    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);         /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
//}

///**
// * @brief       KEY2 �ⲿ�жϷ������
// * @param       ��
// * @retval      ��
// */
//void KEY2_INT_IRQHandler(void)
//{ 
//    HAL_GPIO_EXTI_IRQHandler(KEY2_INT_GPIO_PIN);        /* �����жϴ����ú��� ���KEY2�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
//    __HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);        /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
//}

///**
// * @brief       WK_UP �ⲿ�жϷ������
// * @param       ��
// * @retval      ��
// */
//void WKUP_INT_IRQHandler(void)
//{ 
//    HAL_GPIO_EXTI_IRQHandler(WKUP_INT_GPIO_PIN);        /* �����жϴ����ú��� ���KEY_UP�����ж��� ���жϱ�־λ���ж��°벿��HAL_GPIO_EXTI_Callbackִ�� */
//    __HAL_GPIO_EXTI_CLEAR_IT(WKUP_INT_GPIO_PIN);        /* HAL��Ĭ�������ж��ٴ���ص����˳�ʱ����һ���жϣ����ⰴ�������󴥷� */
//}


//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    delay_ms(20);      /* ���� */
//    switch(GPIO_Pin)
//    {
//        case KEY0_INT_GPIO_PIN:
//            if (KEY0 == 0)
//            {
//				adcx = adc_get_result_average(ADC_CHANNEL_3, 10);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
//        temp = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
//        adcx = temp;                                        /* ��ֵ�������ָ�adcx��������ΪadcxΪu16���� */
//        lcd_show_xnum(134, 170, adcx, 1, 16, 0, BLUE);      /* ��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3 */
//        temp -= adcx;                                       /* ���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111 */
//        temp *= 1000;                                       /* С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС���� */
//        lcd_show_xnum(150, 170, temp, 3, 16, 0X80, BLUE);   /* ��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111. */LED0_TOGGLE();  /* LED0 ״̬ȡ�� */ 
//							
//            }
//            break;

//        case KEY1_INT_GPIO_PIN:
//            if (KEY1 == 0)
//            {
//							printf("1");
//              adcx = adc_get_result_average(ADC_CHANNEL_4, 10);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
//              temp = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
//              adcx = temp;                                        /* ��ֵ�������ָ�adcx��������ΪadcxΪu16���� */
//              lcd_show_xnum(134, 190, adcx, 1, 16, 0, BLUE);      /* ��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3 */
//              temp -= adcx;                                       /* ���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111 */
//              temp *= 1000;                                       /* С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС���� */
//              lcd_show_xnum(150, 190, temp, 3, 16, 0X80, BLUE);   /* ��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111. */
//              

//            }
//            break;

//        case KEY2_INT_GPIO_PIN:
//            if (KEY2 == 0)
//            {
//														printf("2");

//        adcx = adc_get_result_average(ADC_CHANNEL_5, 10);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
//        temp = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
//        adcx = temp;                                        /* ��ֵ�������ָ�adcx��������ΪadcxΪu16���� */
//        lcd_show_xnum(134, 130, adcx, 1, 16, 0, BLUE);      /* ��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3 */
//        temp -= adcx;                                       /* ���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111 */
//        temp *= 1000;                                       /* С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС���� */
//        lcd_show_xnum(150, 130, temp, 3, 16, 0X80, BLUE);   /* ��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111. */
//				delay_ms(5000);

//            }
//            break;

//        case WKUP_INT_GPIO_PIN:
//            if (WK_UP == 1)
//            {
//        adcx = adc_get_result_average(ADC_CHANNEL_6, 10);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
//        temp = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
//        adcx = temp;                                        /* ��ֵ�������ָ�adcx��������ΪadcxΪu16���� */
//        lcd_show_xnum(134, 150, adcx, 1, 16, 0, BLUE);      /* ��ʾ��ѹֵ���������֣�3.1111�Ļ������������ʾ3 */
//        temp -= adcx;                                       /* ���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111 */
//        temp *= 1000;                                       /* С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС���� */
//        lcd_show_xnum(150, 150, temp, 3, 16, 0X80, BLUE);   /* ��ʾС�����֣�ǰ��ת��Ϊ��������ʾ����������ʾ�ľ���111. */
//				delay_ms(5000);
//   
//            }
//            break;

//        default : break;
//    }
//}

///**
// * @brief       �ⲿ�жϳ�ʼ������
// * @param       ��
// * @retval      ��
// */
//void extix_init(void)
//{
//    GPIO_InitTypeDef gpio_init_struct;
//    
//    key_init();
//    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
//    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �½��ش��� */
//    gpio_init_struct.Pull = GPIO_PULLUP;
//    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0����Ϊ�½��ش����ж� */

//    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
//    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �½��ش��� */
//    gpio_init_struct.Pull = GPIO_PULLUP;
//    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1����Ϊ�½��ش����ж� */
//    
//    gpio_init_struct.Pin = KEY2_INT_GPIO_PIN;
//    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* �½��ش��� */
//    gpio_init_struct.Pull = GPIO_PULLUP;
//    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &gpio_init_struct);    /* KEY2����Ϊ�½��ش����ж� */
//    
//    gpio_init_struct.Pin = WKUP_INT_GPIO_PIN;
//    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* �����ش��� */
//    gpio_init_struct.Pull = GPIO_PULLDOWN;
//    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);        /* WKUP����Ϊ�����ش����ж� */

//    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2);               /* ��ռ0�������ȼ�2 */
//    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* ʹ���ж���4 */

//    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2);               /* ��ռ1�������ȼ�2 */
//    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* ʹ���ж���3 */
//    
//    HAL_NVIC_SetPriority(KEY2_INT_IRQn, 2, 2);               /* ��ռ2�������ȼ�2 */
//    HAL_NVIC_EnableIRQ(KEY2_INT_IRQn);                       /* ʹ���ж���2 */

//    HAL_NVIC_SetPriority(WKUP_INT_IRQn, 3, 2);               /* ��ռ3�������ȼ�2 */
//    HAL_NVIC_EnableIRQ(WKUP_INT_IRQn);                       /* ʹ���ж���0 */

//}







