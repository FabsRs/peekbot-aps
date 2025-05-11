// MIT License
// 
// Copyright (c) 2024 Fabrizio Eduardo Rojas Iacocca
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the SWoftware is
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

#ifndef USART0_H
#define USART0_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/string.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "avr.h"
#include "ccommons.h"

#define USART0_MODE_ASYNC_NORMAL	0x01
#define USART0_MODE_ASYNC_DOUBLE	0x02

#define USART0_BR_2400 		2400
#define USART0_BR_4800		4800
#define USART0_BR_9600		9600
#define USART0_BR_14400		14400
#define USART0_BR_19200		19200
#define USART0_BR_28800		28800
#define USART0_BR_38400		38400
#define USART0_BR_57600		57600
#define USART0_BR_76800 	76800
#define USART0_BR_115200 	115200
#define USART0_BR_230400	230400
#define USART0_BR_250000 	250000
#define USART0_BR_500000 	500000
#define USART0_BR_1000000 	1000000

#define USART0_DATABITS_5	0x01
#define USART0_DATABITS_6	0x02
#define USART0_DATABITS_7	0x03
#define USART0_DATABITS_8	0x04

#define USART0_PARITY_NONE	0x01
#define USART0_PARITY_EVEN	0x02
#define USART0_PARITY_ODD	0x03

#define USART0_STOPBITS_1	0x01
#define USART0_STOPBITS_2	0x02

#define USART0_RX_ENABLE	0x01
#define USART0_RX_DISABLE	0x02

#define USART0_TX_ENABLE	0x01
#define USART0_TX_DISABLE	0x02

// Structure to represent USART0 parameters
// mode: USART mode (USART0_MODE_ASYNC_NORMAL or USART0_MODE_ASYNC_DOUBLE)
// baudrate: Baud rate for communication
// databits: Number of data bits (USART0_DATABITS_5, USART0_DATABITS_6, USART0_DATABITS_7, USART0_DATABITS_8)
// parity: Parity mode (USART0_PARITY_NONE, USART0_PARITY_EVEN, USART0_PARITY_ODD)
// stopbits: Number of stop bits (USART0_STOPBITS_1, USART0_STOPBITS_2)
// rx: Receiver enable/disable (USART0_RX_ENABLE, USART0_RX_DISABLE)
// tx: Transmitter enable/disable (USART0_TX_ENABLE, USART0_TX_DISABLE)
// timeout_ms: Timeout in milliseconds
typedef struct _USART0_PARAMS
{
	uint8 mode;
	uint32 baudrate;
	uint8 databits;
	uint8 parity;
	uint8 stopbits;
	uint8 rx;
	uint8 tx;
	uint16 timeout_ms;
}*PUSART0_PARAMS,USART0_PARAMS;

// Structure to represent USART0 buffer
// buffer: Pointer to the buffer
// size: Size of the buffer
// head: Head index for the buffer
// tail: Tail index for the buffer
typedef struct _USART0_BUFFER
{
	puint8 buffer;
	uint16 size;
	uint16 head;
	uint16 tail;
}*PUSRAT0_BUFFER,USART0_BUFFER;

// Structure to represent USART0 status
// tx_busy: Flag indicating if transmission is busy
// rx_full: Flag indicating if the receiver buffer is full
// rx_void: Flag indicating if the receiver buffer is empty
typedef struct _USART0_STATUS
{
	uint8 tx_busy;
	uint8 rx_full;
	uint8 rx_void;
}*PUSART0_STATUS,USART0_STATUS;

// Structure to represent USART0
// params: USART0 parameters
// rx: Receiver buffer
// tx: Transmitter buffer
// status: USART0 status
typedef struct _USART0
{
	USART0_PARAMS params;
	USART0_BUFFER rx;
	USART0_BUFFER tx;
	USART0_STATUS status;
}*PUSART0,USART0;

// Initializes USART0 with specified buffers
int8 usart0_init(puint8 rxBuffer, uint16 rxBufferSize, puint8 txBuffer, uint16 txBufferSize);

// Sets the baud rate for USART0
int8 usart0_set_baud(uint8 mode, uint32 baudrate);

// Sets default parameters for USART0
int8 usart0_set_default();

// Sets the frame format for USART0
int8 usart0_set_frame(uint8 databits, uint8 parity, uint8 stopbits);

// Enables or disables the receiver for USART0
int8 usart0_set_rx(uint8 rx);

// Enables or disables the transmitter for USART0
int8 usart0_set_tx(uint8 tx);

// Transmits a string via USART0
int8 usart0_serial_tx(char* string, uint16 size);

// Transmits a single byte via USART0
int8 usart0_serial_tx_byte(char C);

// USART0 data register empty interrupt service routine
int8 usart0_isr_udr(void);

// Receives a string via USART0
int8 usart0_serial_rx(char* string, uint16 size);

// Returns the number of bytes available in the receiver buffer
uint16 usart0_serial_rx_count();

// Flushes the receiver buffer
int8 usart0_serial_rx_flush();

// Receives a single byte via USART0
int8 usart0_serial_rx_byte(char* c);

// USART0 receive complete interrupt service routine
int8 usart0_isr_rx(void);

// Delays execution for a specified
int8 usart0_delay_ms(uint16 time);

volatile USART0 usart0;

#endif//USART0_H