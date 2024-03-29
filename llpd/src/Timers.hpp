#include "LLPD.hpp"

#include <limits>

static volatile float*   tim6DelayVal = (float*) D3_SRAM_BASE; // value for delay functions
static volatile uint32_t tim6InterruptRate = 0; 	// interrupt rate used for delay functions
static volatile float*   tim6USecondMax = tim6DelayVal + 1; 	// when tim6USecondIncr reaches this value, the delay is over
static volatile float*   tim6USecondIncr = tim6USecondMax + 1;	// how much to increment per interrupt for microsecond delay

void LLPD::tim6_counter_setup (uint32_t prescalerDivisor, uint32_t cyclesPerInterrupt, uint32_t interruptRate)
{
	*tim6DelayVal = std::numeric_limits<float>::max();

	// store sample rate for delay functions
	tim6InterruptRate = interruptRate;
	*tim6USecondIncr = 1000000.0f / tim6InterruptRate;

	// make sure timer is disabled during setup
	TIM6->CR1 &= ~(TIM_CR1_CEN);

	// enable peripheral clock to TIM6
	RCC->APB1LENR |= RCC_APB1LENR_TIM6EN;

	// reset registers
	RCC->APB1LRSTR |= RCC_APB1LRSTR_TIM6RST;
	RCC->APB1LRSTR &= ~(RCC_APB1LRSTR_TIM6RST);

	// set timer prescaler and auto-reload values
	TIM6->PSC = prescalerDivisor;
	TIM6->ARR = cyclesPerInterrupt;

	// send an update event to apply the settings
	TIM6->EGR |= TIM_EGR_UG;
}

void LLPD::tim6_counter_enable_interrupts()
{
	// set priority and enable irq
	NVIC_SetPriority( TIM6_DAC_IRQn, 0x00 );
	NVIC_EnableIRQ( TIM6_DAC_IRQn );

	// enable interrupt register
	TIM6->DIER |= TIM_DIER_UIE;
}

void LLPD::tim6_counter_disable_interrupts()
{
	// disable irq
	NVIC_DisableIRQ( TIM6_DAC_IRQn );

	// disable interrupt register
	TIM6->DIER &= ~(TIM_DIER_UIE);
}

void LLPD::tim6_counter_start()
{
	TIM6->CR1 |= TIM_CR1_CEN;
}

void LLPD::tim6_counter_stop()
{
	TIM6->CR1 &= ~(TIM_CR1_CEN);
	TIM6->SR  &= ~(TIM_SR_UIF);
}

void LLPD::tim6_counter_clear_interrupt_flag()
{
	TIM6->SR &= ~(TIM_SR_UIF);
}

void LLPD::tim6_delay (uint32_t microseconds)
{
	*tim6DelayVal = 0;
	*tim6USecondMax = static_cast<float>( microseconds );

	// wait for delay to complete
	while ( *tim6DelayVal < *tim6USecondMax ) {}
}

bool LLPD::tim6_isr_handle_delay()
{
	if ( *tim6DelayVal < *tim6USecondMax )
	{
		*tim6DelayVal += *tim6USecondIncr;

		return true;
	}

	return false;
}
