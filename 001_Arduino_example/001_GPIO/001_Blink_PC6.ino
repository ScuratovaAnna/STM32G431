/**-----------------------------------------------------------------------
 *\date  23.06.2024
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
 *  |            |
 *
 *
 *\authors        
 *\сode debugging 
 *
 **************************************************************************
 *\brief
 */

/* Прототипы функций --> https://www.c-cpp.ru/books/ispolzovanie-prototipov-funkcii  */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//***********************************************************************/
//то что выполняется один раз
void setup() {
  HAL_Init();            // --> https://www.youtube.com/watch?v=bso8JyznhhQ
  SystemClock_Config();  //инициализируем систему тактирования
  MX_GPIO_Init();        //настраиваем наш вывод GPIO --> PC.6
}
/************************************************************************/
// вечный цикл в котором крутится код
void loop() {
  //зажигаем светодиод выставляем высокий уровень на выводе PC.6
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
  HAL_Delay(20);  //ждём (функция задержки)
                  //гасим светодиод выставляем низкий уровень на выводе PC.6
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_Delay(20);  //ждём (функция задержки)
}
/*************************** конец **************************************/

// Настройка системы тактирования --> настройка тактирования выполнена от внешнего кварца без PLL
// На данном этапе можно это пропустить
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
//настройка GPIO --> более подробно http://mypractic.ru/hal-gpio-generic-driver-funkcii-upravleniya-portami-vvoda-vyvoda
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
   __HAL_RCC_GPIOC_CLK_ENABLE();                         //разрешаем тактирование порта C
  GPIO_InitStruct.Pin = GPIO_PIN_6;                      //определяем вывод
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;            //режим работы вывода
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;           //скорость работы я ставлю минимальную
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);                //собираем всё в кучу
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);  //выставляем низкий уровень на выводе PC.6
  HAL_Delay(500);                                        //и чтобы это увидеть делаем задержку
}
/****************************** End of file *****************************/