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

#include "avr.h"
#include "ccommons.h"

#define PINOUT_ON       0x01
#define PINOUT_OFF      0x02

#define HT_PCB          PORTB0  // OUT (Digital)        DDRB0:1     PORTB0:0
#define AZ_EN           PORTB1  // OUT (Digital)        DDRB1:1     PORTB1:0
#define EL_EN           PORTB2  // OUT (Digital)        DDRB2:1     PORTB2:0
#define APS_STA         PORTB3  // OUT (Digital)        DDRB3:1     PORTB3:0
#define AZ_PH           PORTB4  // OUT (Digital)        DDRB4:1     PORTB4:0
#define EL_PH           PORTB5  // OUT (Digital)        DDRB5:1     PORTB5:0
#define XTAL1           PORTB6  // Clock (Do not use)   DDRB6:0     PORTB6:0
#define XTAL2           PORTB7  // Clock (Do not use)   DDRB7:0     PORTB7:0

#define TH1_TEMP        PORTC0  // IN (Analog)          DDRC0:0     PORTC0:0
#define TH2_TEMP        PORTC1  // IN (Analog)          DDRC1:0     PORTC1:0
#define TH3_TEMP        PORTC2  // IN (Analog)          DDRC2:0     PORTC2:0
#define TH4_TEMP        PORTC3  // IN (Analog)          DDRC3:0     PORTC3:0
#define AZ_SHUNT        PORTC4  // IN (Analog)          DDRC4:0     PORTC4:0
#define EL_SHUNT        PORTC5  // IN (Analog)          DDRC5:0     PORTC5:0
#define RESET           PORTC6  // Reset (Do not Use)   DDRC6:0     PORTC6:0

#define RXD             PORTD0  // IN (Digital)         DDRD0:0     PORTC0:0
#define TXD             PORTD1  // OUT (Digital)        DDRD1:1     PORTC1:0
#define AZ_ENC_PWM      PORTD2  // IN (Digital)         DDRD2:0     PORTC2:0
#define AZ_ENC_STA      PORTD3  // IN (Digital)         DDRD3:0     PORTC3:0
#define EL_ENC_CH_A     PORTD4  // IN (Digital)         DDRD4:0     PORTC4:0
#define EL_ENC_CH_B     PORTD5  // IN (Digital)         DDRD5:0     PORTC5:0
#define HT_AZ           PORTD6  // OUT (Digital)        DDRD6:1     PORTC6:0
#define HT_EL           PORTD7  // OUT (Digital)        DDRD7:1     PORTC7:0

int8 pinout_init(void);