#ifndef LLPD_H
#define LLPD_H

#include <stdint.h>

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

enum class SPI_NUM
{
	SPI_1,
	SPI_2,
	SPI_3,
	SPI_4,
	SPI_5,
	SPI_6
};

enum class SPI_BAUD_RATE
{
	SYSCLK_DIV_BY_2,
	SYSCLK_DIV_BY_4,
	SYSCLK_DIV_BY_8,
	SYSCLK_DIV_BY_16,
	SYSCLK_DIV_BY_32,
	SYSCLK_DIV_BY_64,
	SYSCLK_DIV_BY_128,
	SYSCLK_DIV_BY_256
};

enum class SPI_CLK_POL
{
	LOW_IDLE,
	HIGH_IDLE
};

enum class SPI_CLK_PHASE
{
	FIRST,
	SECOND
};

enum class SPI_DUPLEX
{
	FULL,
	HALF
};

enum class SPI_FRAME_FORMAT
{
	MSB_FIRST,
	LSB_FIRST
};

enum class SPI_DATA_SIZE
{
	BITS_4,
	BITS_5,
	BITS_6,
	BITS_7,
	BITS_8,
	BITS_9,
	BITS_10,
	BITS_11,
	BITS_12,
	BITS_13,
	BITS_14,
	BITS_15,
	BITS_16
};

// TODO finish the i2c implementation
enum class I2C_NUM
{
	I2C_1
};

enum class I2C_ADDR_MODE
{
	BITS_7
};

class LLPD
{
	public:
		// RCC
		static void rcc_clock_start_max_cpu1(); // starts M7 core at 480 MHz using PLL and LDO (needs to be used with below function)
		static void rcc_clock_start_max_cpu2(); // starts M4 core at 240 MHx using PLL and LDO (needs to be used with above function)

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

		// SPI spi1( sck =  a5, miso =  a6, mosi =  a7 )
		//     spi2( sck = b13, miso = b14, mosi = b15 )
		//     spi3( sck = c10, miso = c11, mosi = c12 )
		//     spi4( sck = e12, miso = e13, mosi = e14 )
		//     spi5( sck =  f7, miso =  f8, mosi =  f9 )
		//     spi6( sck = g13, miso = g12, mosi = g14 )
		static void spi_master_init (const SPI_NUM& spiNum, const SPI_BAUD_RATE& baudRate, const SPI_CLK_POL& pol,
						const SPI_CLK_PHASE& phase, const SPI_DUPLEX& duplex,
						const SPI_FRAME_FORMAT& frameFormat, const SPI_DATA_SIZE& dataSize);
		static uint16_t spi_master_send_and_recieve (const SPI_NUM& spiNum, uint8_t data);

		// I2C TODO just stubs for now so this stuff compiles
		static void i2c_master_setup (const I2C_NUM& i2cNum, uint32_t timingRegVal);
		static void i2c_master_set_slave_address (const I2C_NUM& i2cNum, const I2C_ADDR_MODE& addrMode, uint16_t address) {}
		static void i2c_master_write (const I2C_NUM& i2cNum, bool setStopCondition, uint8_t numBytes, uint8_t bytes...) {}
		static void i2c_master_read (const I2C_NUM& i2cNum, bool setStopCondition, uint8_t numBytes, uint8_t* bytes...) {}
		static void i2c_master_read_into_array (const I2C_NUM& i2cNum, bool setStopCondition, uint8_t numBytes, uint8_t* arr) {}
};

#endif // LLPD_H
