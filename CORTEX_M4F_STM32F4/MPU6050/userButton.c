#include "userButton.h"
#include "mpu6050.h"
#include "uart.h"

uint8_t controller_mode = 0;

void user_button_Interrupts_Configure() {
	/* enable GPIOG for LED */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/*GPIO Configuration
	 * LED3 (Green): GPIO_Pin_13, LED4 (Red): GPIO_Pin_14
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Connect EXTI Line 0 to the button GPIO Pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, GPIO_PinSource0);

	/* Configure the EXTI line to generate an interrupt when the button is
	 * pressed. The button pin is high when pressed, so it needs to trigger
	 * when rising from low to high.
	 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Remote is in controller mode while pushing reset button */
	GPIO_SetBits(GPIOG, GPIO_Pin_14);
	GPIO_ResetBits(GPIOG, GPIO_Pin_13);
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{
		/* clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line0);

		controller_mode = (controller_mode + 1) % 2;
		change_mode();
	}
}

void change_mode() {
	if (controller_mode) {
		/* controller mode, lighting led 13 */
		GPIO_SetBits(GPIOG, GPIO_Pin_13);
		GPIO_ResetBits(GPIOG, GPIO_Pin_14);
		MPU6050_Task_Resume();
	} else {
		/* not in controller mode, lighting led 14 */
		GPIO_SetBits(GPIOG, GPIO_Pin_14);
		GPIO_ResetBits(GPIOG, GPIO_Pin_13);
		MPU6050_Task_Suspend();
	}
}
