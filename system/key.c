#include "stm32f10x.h"
#include "key.h"

uint8_t key_is_pressed(int n)
{
	uint8_t pressed = 0;	

	if (n == KEY_LEFT)
	{
		pressed = (GPIOA->IDR & (0x1 << 3)) ? 0:1;
	}
	else if (n == KEY_CENTER)
	{
		pressed = (GPIOA->IDR & (0x1 << 2)) ? 0:1;
	}
	else if (n == KEY_RIGHT)
	{
		pressed = (GPIOA->IDR & (0x1 << 4)) ? 0:1;
	}	

	return pressed;
}

void key_init()
{
	/*
	 * enable clock for port-b, port-a, afio ip
	 */
	RCC->APB2ENR = RCC->APB2ENR | (0x1 << 2) | (0x1 << 0);
	
	/*
	 * enable irq6, irq7
	 */
	*(unsigned int *)0xE000E100 = *(unsigned int *)0xE000E100 | (0x1 << 7) | (0x1 << 6); 

	/*
	 * interrupt enable for user key (pb0, pb1)
	 */
	GPIOB->CRL = (GPIOB->CRL & ~ (0xFF << 0)) | (0x44 << 0);
	AFIO->EXTICR[0] = (AFIO->EXTICR[0]  & ~ (0xFF << 0)) | (0x11 << 0);
	EXTI->FTSR = EXTI->FTSR | (0x3 << 0);
	EXTI->IMR = EXTI->IMR | (0x3 << 0);

	/*
	 * input enable for user key (pa2, pa3, pa4)
	 */
	GPIOA->CRL = (GPIOA->CRL & ~ (0xFFF << 8)) | (0x444 << 8);
}

