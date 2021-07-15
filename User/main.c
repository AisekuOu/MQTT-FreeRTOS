#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "w5500evb_sysc.h"
#include "w5500evb_delay.h"
#include "w5500evb_usart.h"
#include "w5500evb_w5500.h"
#include "w5500evb_tasks.h"

int main(void)
{
	HAL_Init();

	SystemClock_Config_HSE();

	Delay_Init(72);

	USART_Init();

	W5500_Init();

	Start_Task_Creat();

	vTaskStartScheduler();
}
