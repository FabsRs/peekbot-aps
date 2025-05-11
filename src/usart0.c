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

#include "usart0.h"

int8 usart0_init(puint8 rxBuffer, uint16 rxBufferSize, puint8 txBuffer, uint16 txBufferSize)
{
    __pi(rxBuffer);
    __pi(txBuffer);
    __fi(usart0_set_default, /* NULL */);
    __fi(usart0_set_baud,usart0.params.mode, usart0.params.baudrate);
    __fi(usart0_set_frame,usart0.params.databits, usart0.params.parity, usart0.params.stopbits);
    __fi(usart0_set_rx,usart0.params.rx);
    __fi(usart0_set_tx,usart0.params.tx);
    
    if(rxBufferSize <= 0)
    {
        return -1;
    }
    if(txBufferSize <= 0)
    {
        return -1;
    }

    usart0.rx.size = rxBufferSize;
    usart0.rx.head = 0;
    usart0.rx.tail = 0;
    usart0.rx.buffer = rxBuffer;
    usart0.tx.size = txBufferSize;
    usart0.tx.head = 0;
    usart0.tx.tail = 0;
    usart0.tx.buffer = txBuffer;
    return 0;
}

int8 usart0_set_baud(uint8 mode, uint32 baudrate)
{
    if(baudrate <= 0)
    {
        return -1;
    }
    if(mode <= 0)
    {
        return -1;
    }
    unsigned int ubrr = 0;

    switch(mode)
    {
        case USART0_MODE_ASYNC_NORMAL:
            ubrr = (F_CPU/(16*baudrate)) - 1;
            UCSR0A &= ~(1<<U2X0);
        break;
        case USART0_MODE_ASYNC_DOUBLE:
            ubrr = (F_CPU/(8*baudrate)) - 1;
            UCSR0A |= (1<<U2X0);
        break;
        default:
            return -1;
    }
	UBRR0H = (ubrr & 0xFF00) >> 8;
	UBRR0L = (ubrr & 0x00FF);

    return 0;
}

int8 usart0_set_default(void)
{
    UCSR0A &= ~(1<<MPCM0);
    UCSR0B &= ~(1<<TXCIE0);
    UCSR0B &= ~(1<<RXB80);
    UCSR0B &= ~(1<<TXB80);
    UCSR0A &= ~(1<<UDRIE0);
    UCSR0C &= ~(1<<UMSEL00);
    UCSR0C &= ~(1<<UMSEL01);
    UCSR0C &= ~(1<<UCPOL0);

    return 0;
}

int8 usart0_set_frame(uint8 databits, uint8 parity, uint8 stopbits)
{
    if(databits <= 0 || parity <= 0 || stopbits <= 0)
    {
        return -1;
    }

    switch (databits)
    {
    case USART0_DATABITS_5:
        UCSR0B &= ~(1<<UCSZ02);
        UCSR0C &= ~(1<<UCSZ01);
        UCSR0C &= ~(1<<UCSZ00);
        break;
    case USART0_DATABITS_6:
        UCSR0B &= ~(1<<UCSZ02);
        UCSR0C &= ~(1<<UCSZ01);
        UCSR0C |= (1<<UCSZ00);
        break;
    case USART0_DATABITS_7:
        UCSR0B &= ~(1<<UCSZ02);
        UCSR0C |= (1<<UCSZ01);
        UCSR0C &= ~(1<<UCSZ00);
        break;
    case USART0_DATABITS_8:
        UCSR0B &= ~(1<<UCSZ02);
        UCSR0C |= (1<<UCSZ01);
        UCSR0C |= (1<<UCSZ00);
        break;
    default:
        return -1;
    }

    switch (parity)
    {
    case USART0_PARITY_NONE:
        UCSR0C &= ~(1<<UPM00);
        UCSR0C &= ~(1<<UPM01);
        break;
    case USART0_PARITY_EVEN:
        UCSR0C |= (1<<UPM00);
        UCSR0C &= ~(1<<UPM01);
        break;
    case USART0_PARITY_ODD:
        UCSR0C &= ~(1<<UPM00);
        UCSR0C &= ~(1<<UPM01);
        break;
    default:
        return -1;
    }

    switch (stopbits)
    {
    case USART0_STOPBITS_1:
        UCSR0C &= ~(1<<USBS0);
        break;
    case USART0_STOPBITS_2:
        UCSR0C |= (1<<USBS0);
        break;
    default:
        return -1;
    }

    return 0;
}

int8 usart0_set_rx(uint8 rx)
{
    if(rx <= 0)
    {
        return -1;
    }

    switch (rx)
    {
    case USART0_RX_ENABLE:
        UCSR0B |= (1<<RXCIE0);
        UCSR0B |= (1<<RXEN0);
        break;
    case USART0_RX_DISABLE:
        UCSR0B &= ~(1<<RXCIE0);
        UCSR0B &= ~(1<<RXEN0);
        break;
    default:
        return -1;
    }

    return 0;
}
int8 usart0_set_tx(uint8 tx)
{
    if(tx <= 0)
    {
        return -1;
    }
    
    switch (tx)
    {
    case USART0_TX_ENABLE:
        UCSR0B |= (1<<TXEN0);
    break;
    case USART0_TX_DISABLE:
        UCSR0B &= ~(1<<TXEN0);
        break;
    default:
        return -1;
    }

    return 0;
}

int8 usart0_serial_tx(char* string, uint16 size)
{
    if(!string)
        return -1;
    for(uint16 i = 0 ; i < size; i++)
    {
        usart0_serial_tx_byte(string[i]);
    }
    return 0;
}

int8 usart0_serial_tx_byte(char c)
{
    if(usart0.tx.head == usart0.tx.tail && bit_is_set(UCSR0A, UDRE0))
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            UDR0 = c;
            UCSR0A |= (1<<TXC0);
        }
        return 0;
    }
    uint16 i = (usart0.tx.head + 1) % usart0.tx.size;
    while(i == usart0.tx.tail) 
    {
        if(bit_is_clear(SREG, SREG_I))
        {
            if(bit_is_set(UCSR0A, UDRE0))
            {
                usart0_isr_udr();
            }
        }
    }
    usart0.tx.buffer[usart0.tx.head] = c;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        usart0.tx.head = i;
        UCSR0B |= (1<<UDRIE0);
    }
    return 0;
}

int8 usart0_isr_udr(void)
{
    uint8 c = usart0.tx.buffer[usart0.tx.tail];
    usart0.tx.tail = (usart0.tx.tail + 1) % usart0.tx.size;
    UDR0 = c;
    UCSR0A |= (1<<TXC0);
    if(usart0.tx.head == usart0.tx.tail)
    {
        UCSR0B &= ~(1<<UDRIE0);
    }
    return 0;
}

ISR(USART_UDRE_vect, ISR_BLOCK)
{
    usart0_isr_udr();
}

int8 usart0_serial_rx(char* string, uint16 size)
{
    uint16 count = 0;
    if(!string)
        return -1;
    usart0_delay_ms(usart0.params.timeout_ms);
    count = usart0_serial_rx_count();
    memset(string, 0, size);
    for(int i = 0 ; i < size || i < count; i++)
    {
        usart0_serial_rx_byte(string + i);
    }
    return 0;
}

uint16 usart0_serial_rx_count()
{
    return (usart0.rx.size + usart0.rx.head - usart0.rx.tail) % usart0.rx.size;
}

int8 usart0_serial_rx_flush()
{
    memset(usart0.rx.buffer, 0, usart0.rx.size);
    usart0.rx.head = 0;
    usart0.rx.tail = 0;
    return 0;
}

int8 usart0_serial_rx_byte(char* c)
{
    if(usart0.rx.head == usart0.rx.tail)
    {
        return -1;
    }
    else
    {
        *c = usart0.rx.buffer[usart0.rx.tail];
        usart0.rx.tail = (usart0.rx.tail + 1) % usart0.rx.size;
        return 0;
    }
}

int8 usart0_isr_rx(void)
{
    if(bit_is_clear(UCSR0A, UPE0))
    {
        char c = UDR0;
        uint16 i = (usart0.rx.head + 1) % usart0.rx.size;
        if(i != usart0.rx.tail)
        {
            usart0.rx.buffer[usart0.rx.head] = c;
            usart0.rx.head = i;
        }
    }
    else
    {
        UDR0;
    }
    return 0;
}

ISR(USART_RX_vect, ISR_BLOCK)
{
    usart0_isr_rx();
}

int8 usart0_delay_ms(uint16 time)
{
    do{
        _delay_ms(1);
        time--;
    }while(!time);
    return 0;
}