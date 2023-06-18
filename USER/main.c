#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

#define     BREATH_COUNT  800 
#define numLEDs 1   //WS2812个数
unsigned char buf_R[numLEDs] = {0};//WS2812颜色缓存
unsigned char buf_G[numLEDs] = {0};//WS2812颜色缓存
unsigned char buf_B[numLEDs] = {0};//WS2812颜色缓存


#define     WS2812          GPIO_Pin_9
#define     WS2812_HIGH    (GPIO_SetBits(GPIOB,GPIO_Pin_12))
#define     WS2812_LOW     (GPIO_ResetBits(GPIOB,GPIO_Pin_12))
void WS2812_Write0(void)
{
    WS2812_HIGH;
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	  __nop();
    WS2812_LOW;
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    __nop();__nop();
}
void WS2812_Write1(void)
{
    WS2812_HIGH;
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
    __nop();__nop();
    WS2812_LOW;
    __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	  __nop();
}
void WS2812_Reset(void)
{
    WS2812_LOW;
    delay_us(80);
}
void WS2812_Write_Byte(uint8_t byte)
{
	uint8_t i;

	for(i=0;i<8;i++)
	{
		if(byte&0x80)
		{
				WS2812_Write1();
		}
		else
		{
				WS2812_Write0();
		}
		byte <<= 1;
	}
}
void WS2812_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue)
{
	WS2812_Write_Byte(green);
	WS2812_Write_Byte(red);
	WS2812_Write_Byte(blue);
}

void RGB_LED_Red(void)
{
	uint8_t i;
	//4个LED全彩灯
	for(i=0;i<4;i++)
	{
		WS2812_Write_24Bits(0, 0xff, 0);
	}
}

void RGB_LED_Close(void)
{
	uint8_t i;
	//4个LED全彩灯
	for(i=0;i<4;i++)
	{
		WS2812_Write_24Bits(0, 0, 0);
	}
}

void RGB_LED_Green(void)
{
	uint8_t i;

	for(i=0;i<4;i++)
	{
		WS2812_Write_24Bits(0xff, 0, 0);
	}
}

void RGB_LED_Blue(void)
{
	uint8_t i;

	for(i=0;i<4;i++)
	{
		WS2812_Write_24Bits(0, 0, 0xff);
	}
}




void Send_2811_24bits(unsigned char G8,unsigned char R8,unsigned char B8)
{
          
	unsigned int n = 0;
	for(n=0;n<8;n++)
	{
		G8<<=n;
		if((G8&0x80) == 0x80)
		{
						WS2812_Write1();
		}
		else  
		{
			WS2812_Write0();
		}
	}

	for(n=0;n<8;n++)
	{
		R8<<=n;
		if((R8&0x80) == 0x80)
		{
						WS2812_Write1();
		}
		else  
		{
						WS2812_Write0();
		}				 
	}

	for(n=0;n<8;n++)
	{
		B8<<=n;
		if((B8&0x80) == 0x80)
		{
						WS2812_Write1();
		}
		else  
		{
			WS2812_Write0();
		}
	}
}

void RGB_Rst()
{
	WS2812_Write0();;
	delay_us(50);
}

void Set_Colour(unsigned char r,unsigned char g,unsigned char b)
{
	unsigned char i;
	for(i=0;i<numLEDs;i++)
	{
		buf_R[i] = r; //??
		buf_G[i] = g;
		buf_B[i] = b;
	}
	for(i=0;i<numLEDs;i++)
	{
		Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);//????
	}
}

void SetPointColour(unsigned int num,unsigned char r,unsigned char g,unsigned char b)
{
	unsigned char i;
	for(i=0;i<numLEDs;i++)
	{
		buf_R[num] = r;//??
		buf_G[num] = g;
		buf_B[num] = b;
	}
	for(i=0;i<numLEDs;i++)
	{
		Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);//????
	}
}


void SetPixelColor(unsigned char num,unsigned long c)
{
	unsigned char i;
	for(i=0;i<numLEDs;i++)
	{
		buf_R[num] = (unsigned char)(c>>16);
		buf_G[num] = (unsigned char)(c>>8);
		buf_B[num] = (unsigned char)(c);
	}
	for(i=0;i<numLEDs;i++)
	{
		Send_2811_24bits(buf_G[i],buf_R[i],buf_B[i]);
	}
}


void PixelUpdate()
{
	RGB_Rst();
}

unsigned long Color(unsigned char r, unsigned char g, unsigned char b)
{
  return ((unsigned long)r << 16) | ((unsigned long)g <<  8) | b;
}


unsigned long Wheel(unsigned char WheelPos)
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85)
	{
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void rainbow(unsigned int wait)
{
  unsigned int i, j;

	for(j=0; j<256; j++)
	{
		for(i=0; i<numLEDs; i++)
	{
		SetPixelColor(i, Wheel((i+j) & 255));
	}
		PixelUpdate();
		delay_ms(wait);
	}
}


void rainbowCycle(unsigned int wait)
{
  unsigned int i, j;

  for(j=0;j<256*5;j++)
  { 
    for(i=0;i<numLEDs;i++)
    {
			SetPixelColor(i, Wheel(((i * 256 / numLEDs) + j) & 255));
    }
    PixelUpdate();
    delay_us (wait);
  }
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	  
	uart_init(115200);				//初始化串口

	printf("----------DevBoard_STM32F103--------\r\n");   
	printf("-----------Operating System---------\r\n"); 
//user	
	LED_Init(); 
	WS2812_Reset();
	printf("Button Pressed!\n");
	while(1)
	{           		
		for(int i = 0;i < BREATH_COUNT;i++)
		{
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);
				delay_us(i); 
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
				delay_us(BREATH_COUNT - i); 
		}
		for(int i = BREATH_COUNT;i > 0;i--)
		{
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);
				delay_us(i); 
				GPIO_SetBits(GPIOC,GPIO_Pin_13);
				delay_us(BREATH_COUNT - i); 
		}
		delay_ms(400);    
		
		rainbowCycle(3000);

	}
}






