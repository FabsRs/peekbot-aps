// MIT License
// 
// Copyright (c) 2024 Fabrizio Eduardo Rojas Iacocca
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef UART_H
#define UART_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/avr/io.h>
#include <avr/avr/interrupt.h>

/* Set the correction byte to 0 */

#define UART_NO_CORRECTION 0

/* Control flag bits */

#define UART_PARITY			0x01	// DISABLED: No parity bit 		ENABLED: Parity bit
#define UART_ONE_SB			0x02	// DISABLED: Two stop bits 		ENABLED: One stop bit
#define UART_NORMAL_SPEED	0x04	// DISABLED: Double speed  		ENABLED: Normal speed
#define UART_TIMEOUT_US	 	0x08	// DISABLED: Timeout in ms 		ENABLED: Timeout in us
#define UART_PARITY_EVEN	0x10	// DISABLED: Parity is odd		ENABLED: Parity is even

/* Data direction flag bits */

#define UART_READ			0x90	// DISABLED: Cannot read 		ENABLED: Can read
#define UART_WRITE			0x48 	// DISABLED: Cannort write 		ENABLED: Can read

/* Pre defined baud rates for Baud flag*/

#define UART_B2400 		2400
#define UART_B4800		4800
#define UART_B9600		9600
#define UART_B14400		14400
#define UART_B19200		19200
#define UART_B28800		28800
#define UART_B38400		38400
#define UART_B57600		57600
#define UART_B76800 	76800
#define UART_B115200 	115200
#define UART_B230400	230400
#define UART_B250000 	250000
#define UART_B500000 	500000
#define UART_B1000000 	1000000

typedef struct _UART
{
	unsigned char 	_cflag;				// Control flag
	unsigned char 	_dflag;				// Data direction flag
	unsigned long 	_bflag;				// Baud flag
	unsigned char* 	_rx_buffer_ptr;	 	// Pointer to user deffined storage array
	unsigned char  	_rx_buffer_size;	// Size of said buffer array						//Memory size: 0x08FF (2304 bytes). For safety reasons, a maximum of 256 bytes can be allocated
	unsigned int  	_timeout;			// int holding the timeout counter
	char			_correction;		// char to correct the output values, shouldn't be too large... initialized to 0
}*PUART,UART;

#endif//UART_H