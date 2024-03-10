#include "stdio.h"
#include "stm32f10x.h"
#include "uart.h"
#include "led.h"
#include "key.h"
#include "fnd.h"
#include "cds.h"

#include "FreeRTOS.h"
#include "semphr.h"

uint32_t gTickCount = 0;
extern SemaphoreHandle_t xSemaphore;
extern unsigned int START;
extern unsigned int tick;
extern QueueHandle_t xQueue;

void EXTI0_IRQHandler(void)
{
	EXTI->PR = (0x1 << 0);
	
	START = START ^ 1;
	
	printf("EXTI-0 interrupt. \r\n");
}

void EXTI1_IRQHandler(void)
{
	EXTI->PR = (0x1 << 1);
	
	tick = 0;
	xQueueSendToBackFromISR(xQueue, &tick, NULL);
	
	printf("EXTI-1 interrupt. \r\n");
}

void SysTick_Handler(void)
{
	gTickCount = gTickCount + 1;
}

void delay(uint32_t msec)
{
	uint32_t next_tick = gTickCount + msec;
	while (gTickCount < next_tick)
	{
	}
}

void systick_init()
{
	*(unsigned int *)0xE000E010 = 0x7;
	*(unsigned int *)0xE000E014 = 72000 - 1;
}

void main()
{
 	systick_init();
	uart_init();
	led_init();
	key_init();
	fnd_init();
	cds_init();

	led_off_all();	

	// In to the FreeRTOS
	freertos_testing();
	
	uint32_t k = 0;
  while (1)
  {
		/*
	 	 *
		 */
		if (key_is_pressed(KEY_LEFT))
		{
			printf("LEFT KEY PESSED \r\n");
		}
		else if (key_is_pressed(KEY_CENTER))
		{
			printf("CENTER KEY PESSED \r\n");
		}
		else if (key_is_pressed(KEY_RIGHT))
		{
			printf("RIGHT KEY PESSED \r\n");
		}

		/*
		 *
		 */
		if ((k%5)==0)
		{
			static uint32_t n = 0;
			led_off_all();
			led_on(LED_0 + n);
			n = (n + 1) % 8;
		}		
		
		/*
		 *
		 */
		uint32_t tick;
		uint32_t a0, a1, a2, a3;
		
		tick = gTickCount;
		a3 = (tick / 10) % 10;
		a2 = (tick / 100) % 10;
		a1 = (tick / 1000) % 10;
		a0 = (tick / 10000) % 10;
		
		fnd_display_number(0, a0);
		delay(5);
		
		fnd_display_number(1, a1);
		delay(5);
		
		fnd_display_number(2, a2);
		delay(5);

		fnd_display_number(3, a3);
		delay(5);
		
		
		/*
		 *
		 */
		if ((k%10) == 0)
		{
			printf("CDS Value : %d \r\n", cds_get_value());	
		}
		
		k = k + 1;		
  }

}
