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

void main(void)
{
    USART0 usart0 =
    {
        .mode = USART0_MODE_ASYNC_NORMAL,
        .baudrate = USART0_BR_9600,
        .databits = USART0_DATABITS_8,
        .parity = USART0_PARITY_NONE,
        .stopbits = USART0_STOPBITS_1,
        .rx = USART0_RX_ENABLE,
        .tx = USART0_TX_ENABLE
    };

    usart0_init(&usart0);
    pinout_init();

    while(1)
    {

    }
    return 0;
}