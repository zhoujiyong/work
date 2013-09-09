/*******************************************************************
*
*    DESCRIPTION:
*
*    AUTHOR:
*
*    HISTORY:
*
*    DATE:2011-4-14
*
*******************************************************************/

/** include files **/
#include "Leds.h"
/** local definitions **/

/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/
static unsigned short g_pusBlinkRate[LED_NUM] =
{
    0, 0, 0, 0
};
static unsigned short g_pusBlinkPeriod[LED_NUM];
static unsigned long g_ulBlinkCount = 0;
static GPIO_TypeDef *g_pulLEDBase[LED_NUM] =
{
    LED1_PORT,
    LED2_PORT,
	LED3_PORT,
	LED4_PORT,
};
static const uint16_t g_pucLEDPin[LED_NUM] =
{
    LED1_PIN,
    LED2_PIN,
	LED3_PIN,
	LED4_PIN,
};
/** private data **/

/** public functions **/

/** private functions **/

void Leds_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* clock enable  */
    RCC_APB2PeriphClockCmd(LED1_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(LED2_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(LED3_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(LED4_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/*  推挽输出  */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*  50M时钟速度  */
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/*  推挽输出  */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*  50M时钟速度  */
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/*  推挽输出  */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*  50M时钟速度  */
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/*  推挽输出  */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*  50M时钟速度  */
	GPIO_Init(LED4_PORT, &GPIO_InitStructure);
}

//中断调用
void LedsTick(void)
{
	unsigned long ulIdx, ulCount;
	//
    // Increment the blink counter.
    //
    g_ulBlinkCount++;

    //
    // Loop through the four LEDs.
    //
    for(ulIdx = 0; ulIdx < LED_NUM; ulIdx++)
    {
        //
        // See if this LED is enabled for blinking.
        //
        if(g_pusBlinkRate[ulIdx] != 0)
        {
            //
            // Get the count in terms of the clock for this LED.
            //
            ulCount = g_ulBlinkCount % g_pusBlinkRate[ulIdx];

            //
            // The LED should be turned on when the count is zero.
            //
            if(ulCount == 0)
            {
                GPIO_SetBits(g_pulLEDBase[ulIdx], g_pucLEDPin[ulIdx]);
            }

            //
            // The LED should be turned off when the count equals the period.
            //
            if(ulCount == g_pusBlinkPeriod[ulIdx])
            {
                GPIO_ResetBits(g_pulLEDBase[ulIdx], g_pucLEDPin[ulIdx]);
            }
        }
    }
}

//static 
void UILEDBlink(unsigned long ulIdx, unsigned short usRate, unsigned short usPeriod)
{
    //
    // Clear the blink rate for this LED.
    //
    g_pusBlinkRate[ulIdx] = 0;

    //
    // A blink period of zero means that the LED should be turned off.
    //
    if(usPeriod == 0)
    {
        //
        // Turn off the LED.
        //
        GPIO_ResetBits(g_pulLEDBase[ulIdx], g_pucLEDPin[ulIdx]);
    }

    //
    // A blink rate equal to the blink period means that the LED should be
    // turned on.
    //
    else if(usRate == usPeriod)
    {
        //
        // Turn on the LED.
        //
        GPIO_SetBits(g_pulLEDBase[ulIdx], g_pucLEDPin[ulIdx]);
    }

    //
    // Otherwise, the LED should be blinked at the given rate.
    //
    else
    {
        //
        // Save the blink rate and period for this LED.
        //
        g_pusBlinkRate[ulIdx] = usRate;
        g_pusBlinkPeriod[ulIdx] = usPeriod;
    }
}

void LedsTask(void *p_arg)
{
	UILED1Blink(1000, 30);
	UILED2Blink(1000, 30);
    for (;;) 
    {
//        LED0_ON();
//		LED1_ON();
//        OSTimeDlyHMSM(0,0,0,50); /* delay 300ms  */
//        LED0_OFF();
//		LED1_OFF();
        OSTimeDlyHMSM(0,0,0,50); /* delay 300ms  */
    }
}
