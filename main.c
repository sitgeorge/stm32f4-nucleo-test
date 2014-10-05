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

   uint8_t blink_on = 1;

   void EXTI15_10_IRQHandler()
   {
       uint32_t pr = EXTI->PR & EXTI_PR_PR13;

       if (pr == EXTI_PR_PR13)  
       {
	   if (blink_on == 1)
	       blink_on = 0;
	   else
	       blink_on = 1;
	   EXTI->PR |= (EXTI_PR_PR13);
       }
   }

   int main() {
       SysTick_Config(16000); // 160MHz*0.001 = 16000 - эквивалентно 1мс между IRQ SysClock

       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
       // Configure GPIOA pin 5 as output
       GPIOA->MODER |= GPIO_MODER_MODER5_0;
       // Configure GPIOA pin 5 in max speed
       GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;

       // Setting EXTx interrupt for GPIOC
       RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable clock for PC13 (GPIOC)
       GPIOC->MODER &= ~(GPIO_MODER_MODER13); // input mode for PC13
       //GPIOC->PUPDR |= GPIO_PUPDR_PUPDR13_0; // pull up
       GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR13);

       RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
       //SYSCFG->EXTICR[4] &= ~(SYSCFG_EXTICR4_EXTI13_PC);
       SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
       
       //EXTI->IMR &= ~EXTI_IMR_MR13;
       //EXTI->EMR &= ~EXTI_EMR_MR13;
       EXTI->RTSR |= EXTI_RTSR_TR13;

       //EXTI->FTSR |= EXTI_FTSR_TR13;
       EXTI->IMR |= EXTI_IMR_MR13;

       NVIC_SetPriority(EXTI15_10_IRQn, 1);
       NVIC_EnableIRQ(EXTI15_10_IRQn);

       //__enable_irq();

       //while(1) if (blink_on) LED_ON(); else LED_OFF();


       while (1) {
	   if (blink_on){
	       LED_OFF();
	       delay_nms(delays[current_delay_val]);
	       LED_ON();
	       delay_nms(delays[current_delay_val]);
	   }
       }

       return 0;
   }

#ifdef __cplusplus
}
#endif /* __cplusplus */
