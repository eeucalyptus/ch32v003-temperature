

#include "ch32v003fun.h"
#include <stdio.h>

#define ITERATIONS 128
#define SERIALPLOT

#define T0 19682
#define T22 19580

int32_t raw_to_temperature(int raw) {
	// value at 0 degrees and 22 degrees is known
	// Interpolate between these values and return temperature
	// in 1/10 degrees
	return (raw - T0) * 220 / (T22 - T0);
}

int main()
{
	SystemInit();

	printf("Temperature sensor proof of concept\r\n");

	// enable power interface module clock
	RCC->APB1PCENR |= RCC_APB1Periph_PWR;

	// enable low speed oscillator (LSI)
	RCC->RSTSCKR |= RCC_LSION;
	while ((RCC->RSTSCKR & RCC_LSIRDY) == 0) {}

	// enable AutoWakeUp event
	EXTI->EVENR |= EXTI_Line9;
	EXTI->FTENR |= EXTI_Line9;

	// configure AWU prescaler
	PWR->AWUPSC |= PWR_AWU_Prescaler_1;

	// configure AWU window comparison value
	PWR->AWUWR &= ~0x3f;
	PWR->AWUWR |= 63;

	// enable AWU
	PWR->AWUCSR |= (1 << 1);

    // TIMER
    RCC->APB2PCENR |= RCC_APB2Periph_TIM1;
	//TIM1->SMCFGR |= TIM_SlaveMode_External1 | TIM_TS_TI1F_ED;
    TIM1->PSC = 10-1;
	TIM1->CTLR1 |= TIM_CEN;

	while(1)
	{
		uint32_t count = 0;

		// Iterate a few times to get a good average
		for(int i = 0; i < ITERATIONS; i++) {
			// reset timer
			TIM1->SWEVGR |= TIM_UG;
			// sleep 8 times
			__WFE();
			__WFE();
			__WFE();
			__WFE();
			__WFE();
			__WFE();
			__WFE();
			__WFE();
			// read timer
			count += TIM1->CNT;
		}
		count /= ITERATIONS;
		#ifdef SERIALPLOT
		printf("%d,", raw_to_temperature(count));
		#else
		printf("Counted %u impulses, T=%d\r\n", count, raw_to_temperature(count));
		#endif
	}

	while(1)
	{}
}
