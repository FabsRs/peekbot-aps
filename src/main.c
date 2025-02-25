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

#include "core.h"

#define SERIAL 1024

int main(void)
{
    usart0.params.mode = USART0_MODE_ASYNC_NORMAL;
    usart0.params.baudrate = USART0_BR_57600;
    usart0.params.databits = USART0_DATABITS_8;
    usart0.params.parity = USART0_PARITY_NONE;
    usart0.params.stopbits = USART0_STOPBITS_1;
    usart0.params.rx = USART0_RX_ENABLE;
    usart0.params.tx = USART0_TX_ENABLE;
    usart0.params.timeout_ms = 250;

    uint8 rxBuffer[RX_BUFFER_SIZE];
    uint8 txBuffer[TX_BUFFER_SIZE];
    
    pinout_init();
    usart0_init(rxBuffer, RX_BUFFER_SIZE, txBuffer, TX_BUFFER_SIZE);

    varchar(STR64, serial);

    sei();

    while(1){
        if(usart0_serial_rx_count() >= 27)
        {
            memset(serial, 0, sizeof(serial));
            usart0_serial_rx(serial, sizeof(serial));
            usart0_serial_tx(serial, strlen(serial) - 1);
        }
    }

    return 0;
}