#include <stm32f4xx.h>
#include "delay.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#define LED_PIN 5
#define LED_ON() GPIOA->ODR |= (1 << 5)
#define LED_OFF() GPIOA->ODR &= ~(1 << 5)

void SysTick_Handler(void) {
  TimeTick_Decrement();
}

int main() {
  SysTick_Init();

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  // Configure GPIOA pin 5 as output
  GPIOA->MODER |= GPIO_MODER_MODER5_0;
  // Configure GPIOA pin 5 in max speed
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;
 
  //int delay = 10;
  while (1) {

    LED_ON();
    //delay_nms(delay);
    delay_1ms();
    LED_OFF();

  }

  return 0;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */