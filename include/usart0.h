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

//	UDREn	(USART Data Register Empty) [Can generate interrupts] < set when transmit buffer is empty> <cleared by writing UDRn. >
//	UDRIEn  (USART Data Register Interrupt Empty Enable) [Allows the generation of interrupts by UDREn]
//
//	TXEN 	(Transmit Enable)
//	RXENn 	(Recieve Enable)
//
//	TXCn	(Transmit Complete) [Can generate interrupts] < set when transmit buffer and shift reg are empty > < cleared when complete >
//			NOTE - Disabling the transmitter will take effect afer the buffer is empty
//	TXCIEn	(Transmit Complete Interrupt Enable) [Allows the generation of interrupts by TXCn]
//
//	RXCn	(Recieve Complete) 	< set to 1 when data not read is in the recieve buffer >
//			NOTE - Disabling the reciever will take effect immediate
//	RXENn	(Recieve Complete Interrupt Enable) < When set to 0, the recieve buffer is flushed >
//
//	FEn 	(Frame Error)  [Indicates the state of the next readable frame stored in the recieve buffer] < set to 0 when stop bit is 1 >
//	DORn	(Data Overrun) [Indicates data loss due to recieve buffer full condition] < set to 1 when another start bit is detected, when full >
//	UPEn	(Parity Error) [Indicates that the next framer recieved had an error] < 1 when error is detected >
//
//	UCSRnA – USART Control and Status Register n A 							(000??000) -> 0x18
//
//			7.	RXCn: USART Receive Complete								< Set USART_RX_vect interrupt >
//			6.	TXCn: USART Transmit Complete								< Set USART_TX_vect interrupt >
//			5.	UDREn: USART Data Register Empty
//			4.	FEn: Frame Error
//			3.	DORn: Data OverRun
//			2.	UPEn: USART Parity Error
//			1.	U2Xn: Double the USART Transmission Speed
//			0.	MPCMn: Multi-processor Communication Mode
//
//	UCSRnB – USART Control and Status Register n B 							(110??000)
//
//	 		7.	RXCIEn: RX Complete Interrupt Enable n
//	 		6.	TXCIEn: TX Complete Interrupt Enable n
//	 		5.	UDRIEn: USART Data Register Empty Interrupt Enable n
//	 		4.	RXENn: Receiver Enable n
//	 		3.	TXENn: Transmitter Enable n
//	 		2.	UCSZn2: Character Size n
//	 		1.	RXB8n: Receive Data Bit 8 n
//	 		0.	TXB8n: Transmit Data Bit 8 n
//
//	UCSRnC – USART Control and Status Register n C 							()
//
//			7 : 6.	UMSELn1:0 USART Mode Select
//			5 : 4.	UPMn1:0: Parity Mode
//			3.		USBSn: Stop Bit Select
//			2 : 1.	UCSZn1:0: Character Size
//			0.		UCPOLn: Clock Polarity
//
//	UBRRnL and UBRRnH – USART Baud Rate Registers
//
//			15 : 12	Reserved
//			11 : 0  UBRR[11:0]: USART Baud Rate Register
//
//	NOTE - Receive Buffer can store up to 2 characters.

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/avr/iom328p.h>
#include <avr/avr/interrupt.h>
#include "ccommons.h"

#define USART0_MODE_ASYNC_NORMAL	0x1
#define USART0_MODE_ASYNC_DOUBLE	0x2

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

#define USART0_DATABITS_5	0x1
#define USART0_DATABITS_6	0x2
#define USART0_DATABITS_7	0x3
#define USART0_DATABITS_8	0x4

#define USART0_PARITY_NONE	0x1
#define USART0_PARITY_EVEN	0x2
#define USART0_PARITY_ODD	0x3

#define USART0_STOPBITS_1	0x1
#define USART0_STOPBITS_2	0x2

#define USART0_RX_ENABLE	0x1
#define USART0_RX_DISABLE	0x2

#define USART0_TX_ENABLE	0x1
#define USART0_TX_DISABLE	0x2

typedef struct _USART0
{
	uint8 mode;
	uint32 baudrate;
	uint8 databits;
	uint8 parity;
	uint8 stopbits;
	uint8 rx;
	uint8 tx;
}*PUSART0,USART0;

int8 usart0_init(PUSART0 usart0);
int8 usart0_set_baud(uint8 mode, uint32 baudrate);
int8 usart0_set_default(void);
int8 usart0_set_frame(uint8 databits, uint8 parity, uint8 stopbits);
int8 usart0_set_rx(uint8 rx);
int8 usart0_set_tx(uint8 tx);

#endif//USART0_H