//##################################################################################################
//
//  Kuart - kernel abstraction for Uart.
//
//##################################################################################################

#ifndef KUART_H
#define KUART_H

#include "sys_types.h"
#include "wlibc_assert.h"

#define UART_WITH_VIDEO /* for x86 video output */
#include "uart.h"

class Uart
{
	static addr_t _addr;  // device base address

public:

	typedef void (*Print_t)(const char* format, ...);

	static inline void init(addr_t base_addr, unsigned baudrate, unsigned sys_clock_hz)
	{
		_addr = base_addr;
		uart_init(_addr, baudrate, sys_clock_hz);
	}

	static inline void putch(int c)
	{
		wassert(_addr != -1);
		int tx_irq_on = uart_tx_irq(_addr, 0);
		while (!uart_putc(_addr, c));
		while (!uart_is_tx_done(_addr));
		uart_tx_irq(_addr, tx_irq_on);
	}

	static inline int getch()
	{
		wassert(_addr != -1);
		return uart_getc(_addr);
	}

	static inline void puts(const char* str)
	{
		while (*str)
		{
			Uart::putch(*str);
			if (*str == '\n')
				Uart::putch('\r');
			str++;
		}
	}

	static inline void putsn(const char* str, size_t len)
	{
		while (len--)
		{
			Uart::putch(*str);
			if (*str == '\n')
				Uart::putch('\r');
			str++;
		}
	}
};

#endif // KUART_H
