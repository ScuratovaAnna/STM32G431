/**-----------------------------------------------------------------------
 *\date  03.07.2024
 *\brief
 *
 * ** Код написан в STM32CubeIDE **
 *
 *
 *      1        1->PA.0
 *     ---       2->PA.1
 *  6 | 7 |3     3->PA.2
 *     ---       4->PA.3
 *  5 | 4 |2     5->PA.4
 *     ---       6->PA.5
 *               7->PA.6
 *
 *  Button interrupts   PB.9  up  PB.1  down
 *  в качестве кнопки использован --> модуль сенсорной кнопки TTP223
 *
 *\authors        
 *\сode debugging 
 *
 **************************************************************************
 *\brief
 */
 
//#include "stdbool.h"
#include "main.h"
typedef uint8_t u8;
/*                      0     1    2     3     4     5     6     7    8     9   */
volatile u8 CW[10] = { 0x3F, 0X6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x67 };
volatile u8 Count = 1;

void SystemClock_Config(void);
static void Seven_Segment_GPIO_Init(void);
static void Button_GPIO_Init(void);

int main(void) {

	HAL_Init();
	SystemClock_Config();
	Seven_Segment_GPIO_Init();
	Button_GPIO_Init();
	while (1) {
		switch (Count) {
		case 1:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[0];
			break;
		case 2:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[1];
			break;
		case 3:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[2];
			break;
		case 4:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[3];
			break;
		case 5:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[4];
			break;
		case 6:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[5];
			break;
		case 7:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[6];
			break;
		case 8:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[7];
			break;
		case 9:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[8];
			break;
		case 10:
			GPIOA->BSRR = 0xFFFF << 16;
			GPIOA->BSRR = CW[9];
			break;
		}
	}
}

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}
/************************************************************************/
static void Seven_Segment_GPIO_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIO_InitTypeDef Seven_Segment_GPIO_Init_portA = { 0 };

	Seven_Segment_GPIO_Init_portA.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2
			| GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	Seven_Segment_GPIO_Init_portA.Mode = GPIO_MODE_OUTPUT_PP;
	Seven_Segment_GPIO_Init_portA.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &Seven_Segment_GPIO_Init_portA);
	HAL_GPIO_WritePin(GPIOA,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
					| GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);
}
/************************************************************************/
static void Button_GPIO_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIO_InitTypeDef Button_GPIO_Init = { 0 };

	Button_GPIO_Init.Pin = GPIO_PIN_1 | GPIO_PIN_9;
	Button_GPIO_Init.Mode = GPIO_MODE_IT_RISING;
	Button_GPIO_Init.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &Button_GPIO_Init);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI1_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_9) {
		Count++;
		if (Count > 10) {
			Count = 10;
		}
	}
	if (GPIO_Pin == GPIO_PIN_1) {
		Count--;
		if (Count <= 0)
			Count = 1;
	}
}

void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
