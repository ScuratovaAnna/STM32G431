### Введение  
Данный дисплейный модуль(LCD 1621) предназначен для совместного использования с контроллерами Arduino, на основе LCD драйвера способного управлять 128 элементами (32x4).  

### Выводы LCD 1621 для подключения к STM32 
- Vcc и GND питание модуля (LCD 1621) от 2.4V до 5V.  
- LED+ питание подсветки  модуля (LCD 1621).  
- CS  Chip Select(вход выбора микросхемы с подтягивающим резистором).  
- WR тактирующий сигнал для записи данных , также есть подтягивающий резистор.  
- Data последовательный ввод данных.  
Контроллер HT1621 работает на SPI протоколе он имеет раздельное тактирование для записи WR и чтения RD.  

### Включение подсветки + задняя сторона модуля  
Включение подсветки модуля реализовано при помощи двух контактных площадок. При этом верхняя площадка запаяна, а нижняя распаяна. В этом положении подсветка модуля (LCD 1621) всегда включена. Для того чтобы начать управлять подсветкой необходимо распаять верхнюю площадку, а нижнюю запаять. Так же на тыльной стороне модуля (LCD 1621) расположен резистор R1, который управляет контрастом  модуля (LCD 1621). Для регулирования контраста  модуля (LCD 1621) необходимо заменить резистор на подстрочный резистор 10К.  

### Режимы LCD 1621  

Функция передачи по SPI протоколу с использованием програмной реализации SPI:

~~~C
void wrDATA(uint8_t data, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
	GPIOA->BSRR |= (1 << WR_clear);
	GPIOA->BSRR = (data & 0x80) ? GPIOA->BSRR | (1 << DATA) : GPIOA->BSRR | (1 << DATA_clear);
	GPIOA->BSRR |= (1 << WR);
    data = (data << 1);
  }
}
~~~  
Где, WR, CS, DATA, CS_clear, WR_clear, DATA_clear это именованные константы для работы с регистром  _BSRR_ и _BRR_.  

Напоминаю что мы используем выводы микроконтроллера PA.0 (CS) ; PA.1 (WR) ; PA.2 (DATA).  

~~~C
#define CS   0
#define WR   1
#define DATA 2

#define CS_clear   16
#define WR_clear   17
#define DATA_clear 18
~~~  

Далее пишем функцию для передачи команды:  
~~~C
void wrCMD(uint8_t CMD) {
  GPIOA->BSRR |= (1 << CS_clear);
  wrDATA(0x80, 4);
  wrDATA(CMD, 8);
  GPIOA->BSRR |= (1 << CS);
}
~~~

|Operation         |    Mode     | ID    |
|:----------------:|:-----------:|:-----:|
|Read              |Data         | 110   |
|Write             |Data         | 101   |
|Read-Modify-WRITE |Data         | 101   |
|Command           |Command      | 100   |  

 Функция инициализации дисплея:  
 ~~~C
void config(void) {
  wrCMD(BIAS);
  wrCMD(RC256);
  wrCMD(SYSDIS);
  wrCMD(WDTDIS1);
  wrCMD(SYSEN);
  wrCMD(LCDON);
}
 ~~~  
 Функциюя для передачи данных:  
 ~~~C
void wrone(uint8_t addr, uint8_t sdata) {
  addr <<= 2;
  GPIOA->BSRR |= (1 << CS_clear);
  wrDATA(0xa0, 3);
  wrDATA(addr, 6);
  wrDATA(sdata, 8);
  GPIOA->BSRR |= (1 << CS);
}
 ~~~  
 
![Battery_levels](https://github.com/ScuratovaAnna/STM32G431/blob/main/002_STM32CubeIDE/002_HT1621/GIF/001.gif)  
