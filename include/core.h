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

#ifndef CORE_H
#define CORE_H

#define F_CPU 16000000UL

#define YMODEM_HEADER_SIZE  64
#define YMODEM_CRC_SIZE     48
#define CCSDS_HEADER_SIZE   64
#define PAYLOAD_SIZE        1024
#define RX_BUFFER_SIZE      512
#define TX_BUFFER_SIZE      512

#include <avr/avr/interrupt.h>
#include <avr/stdio.h>
#include <avr/stdint.h>
#include <avr/stdlib.h>
#include <avr/unistd.h>
#include <avr/string.h>
#include <avr/util/delay.h>
#include "analog.h"
#include "ccommons.h"
#include "encoder.h"
#include "motor.h"
#include "pinout.h"
#include "timer.h"
#include "usart0.h"
#include "ymodem.h"

#endif//CORE_H