#include "stdio.h"
#include "stm32f10x.h"
#include "uart.h"
#include "led.h"
#include "key.h"
#include "fnd.h"
#include "cds.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef struct Message
{
	uint32_t tick;
	uint32_t value;
} message_t;

extern int gTickCount;
SemaphoreHandle_t xSemaphore;
QueueHandle_t xQueue;
TaskHandle_t hTask_B;
unsigned int START;
unsigned int tick;

void _delay(unsigned int n)
{
	for(int k = 0; k<n; k++);
}

void vApplicationIdleHook()
{
	
}

void queue_init()
{
	xQueue = xQueueCreate(4, sizeof(int));
}

void Task_A(void * param)
{	
	message_t message;
	
	while(1)
	{
		
		if(START)
		{
			tick++;
			xQueueSend(xQueue, (void *)&tick, portMAX_DELAY);
		}
		
		vTaskDelay(10);
		
	}
}

void Task_B(void * param)
{	
	
	unsigned int tick_r;
	
	while(1)
	{

		unsigned int a0, a1, a2, a3;

		xQueueReceive(xQueue, (void *)&tick_r, 0); 
				
		a3 = (tick_r / 10) % 10;
		a2 = (tick_r / 100) % 10;
		a1 = (tick_r / 1000) % 10;
		a0 = (tick_r / 10000) % 10;
		
		fnd_display_number(0, a0);
		vTaskDelay(5);
		
		fnd_display_number(1, a1);
		vTaskDelay(5);
		
		fnd_display_number(2, a2);
		vTaskDelay(5);

		fnd_display_number(3, a3);
		vTaskDelay(5);
				
	}
}

void Task_C(void * param)
{
	while(1)
	{
	led_toggle(LED_0);
	led_toggle(LED_1);
	led_toggle(LED_2);
	led_toggle(LED_3);
	led_toggle(LED_4);
	led_toggle(LED_5);
	led_toggle(LED_6);
	led_toggle(LED_7);
	vTaskDelay(1000);
	}
}



void freertos_testing()
{
	xTaskCreate(	Task_A,       //
							"Task_A",          
							configMINIMAL_STACK_SIZE,     
							NULL,   
							tskIDLE_PRIORITY,
							NULL );     
	xTaskCreate(	Task_B,       
								"Task_B",          
								configMINIMAL_STACK_SIZE,     
								NULL,   
								tskIDLE_PRIORITY,
								&hTask_B );   
	
	xTaskCreate(	Task_C,       
								"Task_C",          
								configMINIMAL_STACK_SIZE,     
								NULL,   
								tskIDLE_PRIORITY,
								NULL );   
	
	xSemaphore = xSemaphoreCreateBinary();
	
	queue_init();
	vTaskStartScheduler();
	
	//never get here.
}