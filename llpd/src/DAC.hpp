#include "LLPD.hpp"

void LLPD::dac_init (bool useVoltageBuffer)
{
	// enable clock to dac
	RCC->APB1LENR |= RCC_APB1LENR_DAC12EN;

	// set pins a4 and a5 to analog mode
	LLPD::gpio_analog_setup( GPIO_PORT::A, GPIO_PIN::PIN_4 );
	LLPD::gpio_analog_setup( GPIO_PORT::A, GPIO_PIN::PIN_5 );

	// disable both dac channels
	DAC1->CR &= ~( DAC_CR_EN1 | DAC_CR_EN2 );

	// enable voltage buffer
	DAC1->MCR &= ~( DAC_MCR_MODE1 | DAC_MCR_MODE2 );

	// disable voltage buffer for both channels if requested
	if ( ! useVoltageBuffer )
	{
		DAC1->MCR |= DAC_MCR_MODE1_1 | DAC_MCR_MODE2_1;
	}

	// enable both dac channels
	DAC1->CR |= DAC_CR_EN1 | DAC_CR_EN2;
}

void LLPD::dac_send (uint16_t ch1Data, uint16_t ch2Data)
{
	// put data in data registers and ensure only 12 bits are used
	DAC1->DHR12RD = ( (ch1Data & 0b0000111111111111) | ((ch2Data & 0b0000111111111111) << 16) );
}
