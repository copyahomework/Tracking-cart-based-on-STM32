#include "motor.h"
#include "tim.h"
#include "LED.h"

void motor_Init(void)	
{
  HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start(&htim4);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4); 
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);	
	
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
}




/**
函    数：设置各引脚的PWM占空比（范围：0~80）

直流电机pwm控制程序
资源：TIM2(PA0、PA1、PA2、PA3)
      TIM3(PA6、PA7、PB0、PB1)
			

PA0、PA1-----	左			PA2、PA3----- 右   前轮
PB8、PB9----- 左      PB0、PB1-----	右   后轮
*/

void setPWM(int num,int speed)
{
	if(speed<0)	speed=0;					
	if(speed>80) speed=80;	
	switch(num)
	{
		case 1:__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,speed);break;
		case 2:__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,speed);break;
		case 3:__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,speed);break;
		case 4:__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,speed);break;
		case 5:__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,speed);break;
		case 6:__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,speed);break;
		case 7:__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,speed);break;
		case 8:__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,speed);break;
		default:;break;
	}
}

float K_left1=5,K_left2=5,K_right1=5,K_right2=5;//K为调整系数，用于调整速度

void moveXiaoche(int left_1,int right_1,int left_2,int right_2) //正数正转，负数反转
{
    // 左前轮（PA0、PA1 - TIM2_CH1、CH2）由 left_1 控制
    if(left_1>=0)    
    {
        setPWM(5,left_1*K_left1);  // TIM2_CH1（PA0）
        setPWM(6,0);
    }
    else
    {
        setPWM(6,-left_1*K_left1); // TIM2_CH2（PA1）
        setPWM(5,0);
    }
    
    // 右前轮（PA2、PA3 - TIM2_CH3、CH4）由 right_1 控制
    if(right_1>=0)    
    {
        setPWM(7,right_1*K_right1);  // TIM2_CH3（PA2）
        setPWM(8,0);
    }
    else
    {
        setPWM(8,-right_1*K_right1); // TIM2_CH4（PA3）
        setPWM(7,0);        
    }
    
    // 左后轮（PB8、PB9 - TIM4_CH3、CH4）由 left_2 控制
    if(left_2>=0)    
    {
        setPWM(1,left_2*K_left2); // TIM4_CH3（PB8）
        setPWM(2,0);
    }
    else
    {
        setPWM(2,-left_2*K_left2); // TIM4_CH4（PB9）
        setPWM(1,0);        
    }
    
    // 右后轮（PB0、PB1 - TIM3_CH3、CH4）由 right_2 控制
    if(right_2>=0)    
    {
        setPWM(3,right_2*K_right2); // TIM3_CH3（PB0）
        setPWM(4,0);
    }
    else
    {
        setPWM(4,-right_2*K_right2); // TIM3_CH4（PB1）
        setPWM(3,0);        
    }    
}

void qianjin(void){
	moveXiaoche(80,80,80,80);
	offLED(LED7_GPIO,LED7_PIN);
}

void houtui(void){
	moveXiaoche(-80,-80,-80,-80);
	offLED(LED7_GPIO,LED7_PIN);
}

void tingche(void){
	moveXiaoche(0,0,0,0);
	onLED(LED7_GPIO,LED7_PIN);
}

void zuozhuan(void){
	moveXiaoche(-80,80,-80,80);
	offLED(LED7_GPIO,LED7_PIN);
}

void youzhuan(void){
	moveXiaoche(80,-80,80,-80);
	offLED(LED7_GPIO,LED7_PIN);	
}

void zuoqian(void){
	moveXiaoche(0,80,0,80);
	offLED(LED7_GPIO,LED7_PIN);
}

void youqian(void){
	moveXiaoche(80,0,80,0);
	offLED(LED7_GPIO,LED7_PIN);	
}
