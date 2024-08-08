#include "./BSP/CONTROL/control.h"
#include "./BSP/DAC/dac.h"
#include "./SYSTEM/delay/delay.h"

void control_init(void){
	  GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOE_CLK_ENABLE();                                 
    gpio_init_struct.Pin =  input_gpio1| input_gpio2;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(input_gpio_1, &gpio_init_struct);       /* 初始化LED0引脚 */
		GPIO_InitTypeDef gpio_init_struct1;
    __HAL_RCC_GPIOD_CLK_ENABLE();                                 
    gpio_init_struct1.Pin =  input_gpio3| input_gpio4;                   /* LED0引脚 */
    gpio_init_struct1.Mode = GPIO_MODE_INPUT;            /* 推挽输出 */
    gpio_init_struct1.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct1.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(input_gpio_3, &gpio_init_struct1);       /* 初始化LED0引脚 */
}
void input_init(void){
	  GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOF_CLK_ENABLE();                                 
    gpio_init_struct.Pin =  input_gpio5| input_gpio6;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(input_gpio_5, &gpio_init_struct);       /* 初始化LED0引脚 */
	  GPIO_InitTypeDef gpio_init_struct1;
    __HAL_RCC_GPIOB_CLK_ENABLE();                                 
    gpio_init_struct1.Pin =  input_gpio7| input_gpio8;                   /* LED0引脚 */
    gpio_init_struct1.Mode = GPIO_MODE_INPUT;            /* 推挽输出 */
    gpio_init_struct1.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct1.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(input_gpio_7, &gpio_init_struct1);       /* 初始化LED0引脚 */
}
void control_go_init(void){
	  GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOD_CLK_ENABLE();                               
    gpio_init_struct.Pin =  control_gpio_front1|control_gpio_front2;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(control_gpio_f1, &gpio_init_struct);       /* 初始化LED0引脚 */
		GPIO_InitTypeDef gpio_init_struct1;
	  __HAL_RCC_GPIOB_CLK_ENABLE();
		gpio_init_struct1.Pin =   control_gpio_back2|control_gpio_back1;                   /* LED0引脚 */
    gpio_init_struct1.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct1.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct1.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(control_gpio_b1, &gpio_init_struct1);       /* 初始化LED0引脚 */
	  HAL_GPIO_WritePin(control_gpio_f1,control_gpio_front1,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(control_gpio_f2,control_gpio_front2,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(control_gpio_b1,control_gpio_back1,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(control_gpio_b2,control_gpio_back2,GPIO_PIN_RESET);	
}
void back(void){
		HAL_GPIO_WritePin(control_gpio_f1,control_gpio_front1,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(control_gpio_f2,control_gpio_front2,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(control_gpio_b1,control_gpio_back1,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(control_gpio_b2,control_gpio_back2,GPIO_PIN_SET);	
}
void front(){
		HAL_GPIO_WritePin(control_gpio_f1,control_gpio_front1,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(control_gpio_f2,control_gpio_front2,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(control_gpio_b1,control_gpio_back1,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(control_gpio_b2,control_gpio_back2,GPIO_PIN_RESET);	
}
void right(void){
	  HAL_GPIO_WritePin(control_gpio_f1,control_gpio_front1,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(control_gpio_f2,control_gpio_front2,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(control_gpio_b1,control_gpio_back1,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(control_gpio_b2,control_gpio_back2,GPIO_PIN_RESET);	
}
void left(void){
		HAL_GPIO_WritePin(control_gpio_f1,control_gpio_front1,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(control_gpio_f2,control_gpio_front2,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(control_gpio_b1,control_gpio_back1,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(control_gpio_b2,control_gpio_back2,GPIO_PIN_SET);	
}
void gpio_stop_init(){
	  GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOG_CLK_ENABLE();                               
    gpio_init_struct.Pin =  stop_gpio1;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(stop_gpio_1, &gpio_init_struct);       /* 初始化LED0引脚 */
	  GPIO_InitTypeDef gpio_init_struct1;
    __HAL_RCC_GPIOE_CLK_ENABLE();                               
    gpio_init_struct1.Pin =  stop_gpio2|stop_gpio4;                   /* LED0引脚 */
    gpio_init_struct1.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct1.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct1.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(stop_gpio_2, &gpio_init_struct1);       /* 初始化LED0引脚 */
	  GPIO_InitTypeDef gpio_init_struct2;
    __HAL_RCC_GPIOC_CLK_ENABLE();                               
    gpio_init_struct2.Pin =  stop_gpio3;                   /* LED0引脚 */
    gpio_init_struct2.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct2.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct2.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(stop_gpio_3, &gpio_init_struct2);       /* 初始化LED0引脚 */
	  HAL_GPIO_WritePin(stop_gpio_1,stop_gpio1,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(stop_gpio_2,stop_gpio2,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(stop_gpio_3,stop_gpio3,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(stop_gpio_4,stop_gpio4,GPIO_PIN_RESET);	
}
void un_stop(void){
	  HAL_GPIO_WritePin(stop_gpio_1,stop_gpio1,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(stop_gpio_2,stop_gpio2,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(stop_gpio_3,stop_gpio3,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(stop_gpio_4,stop_gpio4,GPIO_PIN_SET);	
}
int direction_sketch(){
	if((HAL_GPIO_ReadPin(input_gpio_3, input_gpio3)==1)&&(HAL_GPIO_ReadPin(input_gpio_4, input_gpio4)==1)){
		return 0;//没有遇到边缘
	}
	else if((HAL_GPIO_ReadPin(input_gpio_3, input_gpio3)==0)&&(HAL_GPIO_ReadPin(input_gpio_4, input_gpio4)==1)){
		return 1;//左侧遇到边缘
	}
else if((HAL_GPIO_ReadPin(input_gpio_3, input_gpio3)==0)&&(HAL_GPIO_ReadPin(input_gpio_4, input_gpio4)==0)){
	return 2;//两侧都遇到边缘
}
	else{
		return -1;//右侧遇到边缘
	}
}
//void control_open_init(void){
//	GPIO_InitTypeDef gpio_init_struct;
//    __HAL_RCC_GPIOD_CLK_ENABLE();                               
//    gpio_init_struct.Pin =  control_gpio_front1|control_gpio_front2;                   /* LED0引脚 */
//    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* 推挽输出 */
//    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
//    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
//    HAL_GPIO_Init(control_gpio_f1, &gpio_init_struct);       /* 初始化LED0引脚 */
//		GPIO_InitTypeDef gpio_init_struct1;
//	  __HAL_RCC_GPIOB_CLK_ENABLE();
//		gpio_init_struct1.Pin =   control_gpio_back2|control_gpio_back1;                   /* LED0引脚 */
//    gpio_init_struct1.Mode = GPIO_MODE_OUTPUT_OD;            /* 推挽输出 */
//    gpio_init_struct1.Pull = GPIO_PULLUP;                    /* 上拉 */
//    gpio_init_struct1.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
//    HAL_GPIO_Init(control_gpio_b1, &gpio_init_struct1);       /* 初始化LED0引脚 */
//		HAL_GPIO_WritePin(control_gpio_f1,control_gpio_front1,GPIO_PIN_RESET);
//	  HAL_GPIO_WritePin(control_gpio_f2,control_gpio_front2,GPIO_PIN_RESET);
//	  HAL_GPIO_WritePin(control_gpio_b1,control_gpio_back1,GPIO_PIN_RESET);
//	  HAL_GPIO_WritePin(control_gpio_b2,control_gpio_back2,GPIO_PIN_RESET);	
//}
//void control_slow(void){
//	control_open_init();
//	dac_set_voltage(1,4095);
//	dac_set_voltage(2,4095);
//}
