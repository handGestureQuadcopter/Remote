/**
  ******************************************************************************
  * @file    Template/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
 
#include "main.h"
#include "MPU6050/userButton.h"
#include "MPU6050/uart.h"
#include "MPU6050/mpu6050.h"

#include "FreeRTOS.h"
#include "task.h"
//#include "semphr.h"

//#include "stm32f4xx_conf.h"

int main(void)
{
	uart1_peripheral_init();
	user_button_Interrupts_Configure();

	/* Initialize MPU6050 sensor 0, address = 0xD0, AD0 pin on sensor is low */
	while (MPU6050_Init(TM_MPU6050_Accelerometer_4G, TM_MPU6050_Gyroscope_250s)
			!= TM_MPU6050_Result_Ok) {
		/* Display message to user */
		USART1_puts("\r\nRemote is NOT READY! PLEASE Checkout.");
	}
	
	USART1_puts("\r\nRemote is ready to use!");

	if (!MPU6050_Task_Creat()) {
		USART1_puts("Initialize information task failed!\r\n");
	}

	vTaskStartScheduler();
}
