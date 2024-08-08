#ifndef _CONTROL_H
#define _CONTROL_H
#include "./SYSTEM/sys/sys.h"
#define control_gpio_front1      GPIO_PIN_3
#define control_gpio_f1					 GPIOD
#define control_gpio_back2       GPIO_PIN_10
#define control_gpio_b2					 GPIOB
#define control_gpio_back1			 GPIO_PIN_3
#define control_gpio_b1					 GPIOB
#define control_gpio_front2			 GPIO_PIN_5
#define control_gpio_f2					 GPIOD
#define stop_gpio1							GPIO_PIN_10
#define stop_gpio_1							GPIOG
#define stop_gpio2							GPIO_PIN_0
#define stop_gpio_2							GPIOE
#define stop_gpio3							GPIO_PIN_1
#define stop_gpio_3							GPIOC
#define stop_gpio4							GPIO_PIN_6
#define stop_gpio_4							GPIOE
#define input_gpio1							GPIO_PIN_12
#define input_gpio_1						GPIOE
#define input_gpio2							GPIO_PIN_11
#define input_gpio_2						GPIOE
#define input_gpio3							GPIO_PIN_13
#define input_gpio_3						GPIOD
#define input_gpio4							GPIO_PIN_11
#define input_gpio_4						GPIOD
#define input_gpio5							GPIO_PIN_6
#define input_gpio_5						GPIOF
#define input_gpio6							GPIO_PIN_8
#define input_gpio_6						GPIOF
#define input_gpio7							GPIO_PIN_2
#define input_gpio_7						GPIOB
#define input_gpio8							GPIO_PIN_0
#define input_gpio_8						GPIOB
void control_init(void);
void control_go_init(void);
void front(void);
void right(void);
void left(void);
void back();
void gpio_stop_init();
void control_speed_init();
int direction_sketch();
void un_stop(void);
void input_init(void);
void control_slow(void);
//int table_sketch();
#endif 
