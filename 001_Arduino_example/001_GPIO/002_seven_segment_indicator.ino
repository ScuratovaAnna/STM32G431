/*
#define 	SET_BIT(REG, BIT)   ((REG) |= (BIT))
#define 	CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
*/
typedef uint8_t u8;
u8 Count;
//             0     1    2     3     4     5     6     7    8     9
u8 CW[10] = { 0x3F, 0X6, 0x5D, 0x4F, 0x66, 0x6B, 0x7B, 0x7, 0x7F, 0x6F };
//*********************************************************************
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//*********************************************************************
//то что выполняется один раз
void setup() {
  HAL_Init();            // --> https://www.youtube.com/watch?v=bso8JyznhhQ
  SystemClock_Config();  //инициализируем систему тактирования
  MX_GPIO_Init();        //настраиваем наш выводы
}
//*********************************************************************
// вечный цикл в котором крутится код
void loop() {
  for (Count = 0; Count < 9; Count++) {

    GPIOA->BSRR = CW[Count];
    HAL_Delay(150);
    GPIOA->BSRR = CW[Count] << 16;
  }
  for (Count = 9; Count > 0; Count--) {
    GPIOA->BSRR = CW[Count];
    HAL_Delay(150);
    GPIOA->BSRR = CW[Count] << 16;
  }
}
//*************************** конец ***********************************

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
  //*******разрешаем тактирование порта A********************************
  //разрешаем тактирование порта A
  // __HAL_RCC_GPIOA_CLK_ENABLE();
  //SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN);
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  //RCC->AHB2ENR |= 1UL << 0;
  //RCC->AHB2ENR =0x1;
  //*********************************************************************
  GPIO_InitTypeDef GPIO_Init_portA = { 0 };
  GPIO_Init_portA.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;                      // определяем выводы
  GPIO_Init_portA.Mode = GPIO_MODE_OUTPUT_PP;                                                                                          // режим работы выводов
  GPIO_Init_portA.Speed = GPIO_SPEED_FREQ_LOW;                                                                                         //скорость работы я ставлю минимальную
  HAL_GPIO_Init(GPIOA, &GPIO_Init_portA);                                                                                              //собираем всё в кучу
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6, GPIO_PIN_RESET);  //выставляем низкий уровень
  HAL_Delay(500);                                                                                                                      //и чтобы это увидеть делаем задержку
}