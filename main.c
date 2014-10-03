#include <stm32f4xx.h>
#include "delay.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define LED_PIN 5
#define LED_ON() GPIOA->ODR |= (1 << 5)
#define LED_OFF() GPIOA->ODR &= ~(1 << 5)

#define _DELAY_STOP 3
#define _DELAY_CHANGE_TIMEOUT 4000

uint32_t delays[] = { 50, 250, 500};
uint32_t current_delay_val = 0;
uint32_t delay_timeout_counter = 0;

void SysTick_Handler(void) {
  TimeTick_Decrement();

  delay_timeout_counter++;

  if (delay_timeout_counter > _DELAY_CHANGE_TIMEOUT)
  {
    delay_timeout_counter = 0;
    if (current_delay_val < _DELAY_STOP - 1)
    {
      current_delay_val++;
    }
    else
      current_delay_val = 0; 
  }
}

int main() {
  SysTick_Config(16000); // 160MHz*0.001 = 16000 - эквивалентно 1мс между IRQ SysClock

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  // Configure GPIOA pin 5 as output
  GPIOA->MODER |= GPIO_MODER_MODER5_0;
  // Configure GPIOA pin 5 in max speed
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;
 
  while (1) {
    LED_OFF();
    delay_nms(delays[current_delay_val]);
    LED_ON();
    delay_nms(delays[current_delay_val]);
  }

  return 0;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */