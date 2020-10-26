#ifndef LLPD_H
#define LLPD_H

#include <stdint.h>

enum class RCC_CLOCK_SOURCE
{
	INTERNAL,
	EXTERNAL
};

enum class RCC_PLL_MULTIPLY
{
};

enum class GPIO_PORT
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H
};

enum class GPIO_PIN
{
	PIN_0  = 0,
	PIN_1  = 1,
	PIN_2  = 2,
	PIN_3  = 3,
	PIN_4  = 4,
	PIN_5  = 5,
	PIN_6  = 6,
	PIN_7  = 7,
	PIN_8  = 8,
	PIN_9  = 9,
	PIN_10 = 10,
	PIN_11 = 11,
	PIN_12 = 12,
	PIN_13 = 13,
	PIN_14 = 14,
	PIN_15 = 15,
};

enum class GPIO_OUTPUT_TYPE
{
	PUSH_PULL,
	OPEN_DRAIN
};

enum class GPIO_OUTPUT_SPEED
{
	LOW,
	MEDIUM,
	HIGH,
	VERY_HIGH
};

enum class GPIO_PUPD
{
	NONE,
	PULL_UP,
	PULL_DOWN
};

class LLPD
{
	public:
		// GPIO
		static void gpio_enable_clock (const GPIO_PORT& port);
		static void gpio_digital_input_setup (const GPIO_PORT& port, const GPIO_PIN& pin, const GPIO_PUPD& pupd,
						bool alternateFunc = false);
		static void gpio_analog_setup (const GPIO_PORT& port, const GPIO_PIN& pin);
		static void gpio_output_setup (const GPIO_PORT& port, const GPIO_PIN& pin, const GPIO_PUPD& pupd,
						const GPIO_OUTPUT_TYPE& type, const GPIO_OUTPUT_SPEED& speed,
						bool alternateFunc = false);
		static bool gpio_input_get (const GPIO_PORT& port, const GPIO_PIN& pin);
		static void gpio_output_set (const GPIO_PORT& port, const GPIO_PIN& pin, bool set);
		static void gpio_test();

		// TIM6
		static void tim6_counter_setup (uint32_t prescalerDivisor, uint32_t cyclesPerInterrupt, uint32_t interruptRate);
		static void tim6_counter_enable_interrupts();
		static void tim6_counter_disable_interrupts();
		static void tim6_counter_start();
		static void tim6_counter_stop();
		static void tim6_counter_clear_interrupt_flag();
		static void tim6_delay (uint32_t microseconds); // delay function may not be 100% accurate
		static bool tim6_isr_handle_delay(); 	// To use delay functions, this function needs to be in the tim6 isr.
							// It will return true if a delay is not finished, or false if it is.
};

#endif // LLPD_H
