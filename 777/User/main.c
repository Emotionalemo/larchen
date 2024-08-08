#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/CONTROL/control.h"
#include "./BSP/LED/led.h"
#include "./BSP/IIC/myiic.h"
#include "./BSP/WT/wt.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/BTIM/btim.h"
#include "./BSP/NEWIIC/newiic.h"

#define key_number 1000
extern float temp3,temp4,temp5;
extern TIM_HandleTypeDef g_timx_pwm_chy_handle; /* ��ʱ��x��� */
extern TIM_HandleTypeDef g_timx_output_chy_handle;
extern int flag;
extern DAC_HandleTypeDef g_dac1_handle; 
extern unsigned int tof_1;//ǰ�ò��
//extern int tof_2;//ǰ���ò��
//extern int tof_3;//ǰ���ò��
extern short rollss;//������
extern short pitchss;//���ҹ���
extern short yawss;//ƫ����
extern uint16_t adcx;

int main(void)
{
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);         /* ����ʱ��,168Mhz */
    delay_init(168);                            /* ��ʱ��ʼ�� */
    usart_init(115200);
		HAL_Delay(500);
	  control_init();
	  control_go_init();
		gpio_stop_init();
	  iic_init();
		adc_init();
		User_I2C_Init();
			input_init();
		gtim_timx_pwm_chy_init(200-1,1680-1);
		__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 50);
   	btim_timx_int_init(1000-1,840-1);
	  while((HAL_GPIO_ReadPin(input_gpio_1,input_gpio1)==1)&&(HAL_GPIO_ReadPin(input_gpio_2,input_gpio2)==1));
		front();
		un_stop();
		while(1){
			
			adcx = adc_get_result_average(ADC_CHANNEL_5, 10);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
       temp5 = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
			 adcx = adc_get_result_average(ADC_CHANNEL_3, 10);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
       temp3 = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
			 adcx = adc_get_result_average(ADC_CHANNEL_4, 10);    /* ��ȡͨ��5��ת��ֵ��10��ȡƽ�� */
       temp4 = (float)adcx * (3.3 / 4096);                  /* ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111 */
      
	printf("%d\n",rollss);
//	printf("%f\n",temp5);
			delay_ms(100);
		
 	if(rollss>10){
	__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 250);
	delay_ms(1000);
	__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 50);
		}
		printf("%d\r\n",HAL_GPIO_ReadPin(input_gpio_1,input_gpio1));

		if(direction_sketch()==0){
			front();
			delay_ms(1);
		}
		else if(direction_sketch()==1){
			right();
			delay_ms(1);
		}
		else if(direction_sketch()==2){
			  back();
			delay_ms(100);
		}
		else
		{
			left();
			delay_ms(1);
		}
		delay_ms(1);
		}
			
		
//			if(tof_1 <30){
//				front();
//			}
//			else if(tof_2<30){
//					right();
//					delay_ms(50);
//			}
//			else if(tof_3<30){
//				left();
//				delay_ms(50);
//			}
//			else if(tof_4<30){
//				right();
//				delay_ms(100);
//			}
//			else if(HAL_GPIO_ReadPin(input_gpio_1,input_gpio1)){
//				
//			}
//			else if(HAL_GPIO_ReadPin(input_gpio_1,input_gpio1)){
//				
//			}
	//̨������ж�
	int down=0;
	if((temp3<key_number)&&(temp4<key_number)&&(temp5<key_number)){
		delay_ms(50);
		if((temp3<key_number)&&(temp4<key_number)&&(temp5<key_number))
		{
			down=1;
		}
	}
	if(down){
		if(HAL_GPIO_ReadPin(input_gpio_3,input_gpio3)==1&&HAL_GPIO_ReadPin(input_gpio_4,input_gpio4)==1)
		{
		__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 50);
		right();
		delay_ms(70);		
		back();
		delay_ms(200);
			}
		else if(HAL_GPIO_ReadPin(input_gpio_3,input_gpio3)==0&&HAL_GPIO_ReadPin(input_gpio_4,input_gpio4)==1){	
		__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 50);
		right();
		delay_ms(50);
		back();
		delay_ms(200);
		}
		else if(HAL_GPIO_ReadPin(input_gpio_3,input_gpio3)==1&&HAL_GPIO_ReadPin(input_gpio_4,input_gpio4)==0){
		__HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY, 50);
		left();	
		delay_ms(50);
		back();
		delay_ms(200);
		}
		else {
			back();
			delay_ms(200);
		}
	}
	//��ǳ����ж�
	
}	
	
	
	