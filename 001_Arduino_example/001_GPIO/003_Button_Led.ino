/**-----------------------------------------------------------------------
 *\date  24.06.2024
 *\brief
 *
 * ** Код написан в Arduino 2.3.2. STM32 core support for Arduino**
 *    https://github.com/stm32duino/Arduino_Core_STM32
 *    Этот репозиторий добавляет поддержку микроконтроллера STM32 в Arduino IDE.
 *
 *    https://github.com/stm32duino/Arduino_Core_STM32/wiki/Getting-Started
 *    Добавление и  поддержка плат STM32 в Arduino. Пошаговая инструкция.
 *
 *   STM32G431CBU6          
 *   ------------        
 *  |            |
 *  |            |
 *  |        PC.6| ----> LED 
 *  |            |
 *  |        PA.4| <---- Button (TTP223)
 *  |            |
 *
 *
 *    \authors        
 *    \сode debugging 
 *    STM32 HAL GPIO Read Pin (GPIO Input Example).
 *    https://deepbluembedded.com/stm32-gpio-pin-read-lab-digital-input/
 **************************************************************************
 *\brief
 */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//***********************************************************************/
void setup() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
}
/************************************************************************/
void loop() {
  //input pin  PA.4  4 bit 
  ((GPIOA->IDR & 0x10) == 0x0) ? (HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)) : (HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET));
  // 0 bit -> 0x1; 1 bit -> 0x2; 2 bit -> 0x4; 3 bit -> 0x8; 4 bit -> 0x10
}
/*************************** конец **************************************/

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
static void MX_GPIO_Init(void) {
  /*  разрешаем тактирование порта A  */
  // __HAL_RCC_GPIOA_CLK_ENABLE();
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

  /*  разрешаем тактирование порта C  */
  // __HAL_RCC_GPIOC_CLK_ENABLE();
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
  /* Led PC.6 */
  GPIO_InitTypeDef GPIO_Output = { 0 };
  GPIO_Output.Pin = GPIO_PIN_6;
  GPIO_Output.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Output.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_Output);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
  /* Button PA.4 */
  GPIO_InitTypeDef GPIO_Input = { 0 };
  GPIO_Input.Pin = GPIO_PIN_4;
  GPIO_Input.Mode = GPIO_MODE_INPUT;
  GPIO_Input.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_Input);
}
/****************************** End of file *****************************/