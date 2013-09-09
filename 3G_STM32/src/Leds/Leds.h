#ifndef __LEDS_H__
#define __LEDS_H__

#include "..\\include.h"

#define LED1_CLOCK  RCC_APB2Periph_GPIOB 
#define LED2_CLOCK  RCC_APB2Periph_GPIOB
#define LED3_CLOCK  RCC_APB2Periph_GPIOB
#define LED4_CLOCK  RCC_APB2Periph_GPIOB
#define LED1_PORT  GPIOB
#define LED1_PIN   GPIO_Pin_9
#define LED2_PORT  GPIOB
#define LED2_PIN   GPIO_Pin_9
#define LED3_PORT  GPIOB
#define LED3_PIN   GPIO_Pin_9
#define LED4_PORT  GPIOB
#define LED4_PIN   GPIO_Pin_9
              
#define LED1_OFF()	(LED1_PORT->BRR = LED1_PIN)
#define LED1_ON()	(LED1_PORT->BSRR = LED1_PIN)
#define LED1_FLASH()  (LED1_PORT->ODR ^= LED1_PIN)

#define LED2_OFF()	(LED2_PORT->BRR = LED2_PIN)
#define LED2_ON()	(LED2_PORT->BSRR = LED2_PIN)
#define LED2_FLASH()  (LED2_PORT->ODR ^= LED2_PIN)

#define LED3_OFF()	(LED3_PORT->BRR = LED3_PIN)
#define LED3_ON()	(LED3_PORT->BSRR = LED3_PIN)
#define LED3_FLASH()  (LED3_PORT->ODR ^= LED3_PIN)

#define LED4_OFF()	(LED4_PORT->BRR = LED4_PIN)
#define LED4_ON()	(LED4_PORT->BSRR = LED4_PIN)
#define LED4_FLASH()  (LED4_PORT->ODR ^= LED4_PIN)

#define LED_NUM 4
#define UILED1Blink(usRate, usPeriod)  UILEDBlink(0, usRate, usPeriod)    
#define UILED2Blink(usRate, usPeriod)  UILEDBlink(1, usRate, usPeriod)
#define UILED3Blink(usRate, usPeriod)  UILEDBlink(2, usRate, usPeriod)
#define UILED4Blink(usRate, usPeriod)  UILEDBlink(3, usRate, usPeriod)

void Leds_Init(void);
void LedsTask(void *p_arg);

void LedsTick(void);
void UILEDBlink(unsigned long ulIdx, unsigned short usRate, unsigned short usPeriod);
#endif     
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
