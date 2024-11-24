#include "LLPD.hpp"

void LLPD::dac_init (bool useVoltageBuffer)
{
	// enable clock to dac
	RCC->APB1LENR |= RCC_APB1LENR_DAC12EN;

	// reset dac
	RCC->APB1LRSTR |= RCC_APB1LRSTR_DAC12RST;
	RCC->APB1LRSTR &= ~(RCC_APB1LRSTR_DAC12RST);

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

void LLPD::dac_init_use_dma (bool useVoltageBuffer)
{
	// enable clock to dac
	RCC->APB1LENR |= RCC_APB1LENR_DAC12EN;

	// reset dac
	RCC->APB1LRSTR |= RCC_APB1LRSTR_DAC12RST;
	RCC->APB1LRSTR &= ~(RCC_APB1LRSTR_DAC12RST);

	// set pins a4 and a5 to analog mode
	LLPD::gpio_analog_setup( GPIO_PORT::A, GPIO_PIN::PIN_4 );
	LLPD::gpio_analog_setup( GPIO_PORT::A, GPIO_PIN::PIN_5 );

	// disable both dac channels
	DAC1->CR &= ~( DAC_CR_EN1 | DAC_CR_EN2 );

	// enable syscfg clock for DMA
	RCC->APB4ENR |= RCC_APB4ENR_SYSCFGEN;

	// setup dma for use with both dac channels
	DAC1->CR |= ( DAC_CR_DMAEN1 | DAC_CR_DMAEN2 );

	// set trigger as tim6 for transfers and enable
	DAC1->CR |= ( DAC_CR_TEN1 | DAC_CR_TEN2 );
	DAC1->CR |= ( 5 << DAC_CR_TSEL1_Pos ) | ( 5 << DAC_CR_TSEL2_Pos );


		// enable dma1 clock
		RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

		// // ensure dma stream is disabled and control register is reset
		// DMA1_Stream0->CR = 0;
		// while ( DMA1_Stream0->CR & DMA_SxCR_EN ) {}

		// adc = ADC1;

		// // set peripheral address for stream
		// DMA1_Stream0->PAR = (uint64_t) &(adc->DR);

		// // set the memory address for where the adc data will be stored
		// DMA1_Stream0->M0AR = (uint64_t) adc12ChannelValues;

		// // clear memory for adc data
		// memset( adc12ChannelValues, 0, 16 * sizeof(*adc12ChannelValues) );

		// // configure the number of data to be transferred
		// DMA1_Stream0->NDTR = numChannels;

		// // configure stream priority to very high
		// DMA1_Stream0->CR |= DMA_SxCR_PL;

		// // set data transfer direction from peripheral to memory
		// DMA1_Stream0->CR &= ~(DMA_SxCR_DIR);

		// // enable memory incrementing
		// DMA1_Stream0->CR |= DMA_SxCR_MINC;

		// // set the peripheral and memory data sizes to 32 bits
		// DMA1_Stream0->CR &= ~(DMA_SxCR_PSIZE);
		// DMA1_Stream0->CR |= DMA_SxCR_PSIZE_1;
		// DMA1_Stream0->CR &= ~(DMA_SxCR_MSIZE);
		// DMA1_Stream0->CR |= DMA_SxCR_MSIZE_1;

		// // set direct mode
		// DMA1_Stream0->FCR &= ~(DMA_SxFCR_DMDIS);

		// // set up dma request input
		// DMAMUX1_Channel0->CCR = 9; // 9 is the dma request mux input for adc1 (as per reference manual)

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
