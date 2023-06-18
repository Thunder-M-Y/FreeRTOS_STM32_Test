#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

void led_task(void *pvParameters)
{
	static uint8_t flag = 0;
    while(1)
    {
        if(0 == flag)
        {
            flag = 1;
            GPIO_ResetBits(GPIOB,GPIO_Pin_6);
        }
        else if(1== flag)
        {
            flag = 0;
            GPIO_SetBits(GPIOB,GPIO_Pin_6);
        }			
        vTaskDelay(1000);       
    }
}   
#define LED_TASK_PRIO		2
#define LED_STK_SIZE 		50  
TaskHandle_t LEDTask_Handler;
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOC,GPIO_Pin_13);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_12;				 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOB,GPIO_Pin_6);	
 GPIO_SetBits(GPIOB,GPIO_Pin_12);	

//	//创建LED0任务
//	xTaskCreate((TaskFunction_t )led_task,     	
//							(const char*    )"led_task",   	
//							(uint16_t       )LED_STK_SIZE, 
//							(void*          )NULL,				
//							(UBaseType_t    )LED_TASK_PRIO,	
//							(TaskHandle_t*  )&LEDTask_Handler);   
								
}
 
