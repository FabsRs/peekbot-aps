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

#ifndef PINOUT_H
#define PINOUT_H

#include "avr.h"
#include "ccommons.h"

#define PINOUT_ON           0x01
#define PINOUT_OFF          0x02

#define PINOUT_B            0x03
#define PINOUT_C            0x04
#define PINOUT_D            0x05

#define PINOUT_ENABLE       0x06
#define PINOUT_DISABLE      0x07

// ARDUINO LAYOUT DIGITAL
#define D0              PORTD0
#define D1              PORTD1
#define D2              PORTD2
#define D3              PORTD3
#define D4              PORTD4
#define D5              PORTD5
#define D6              PORTD6
#define D7              PORTD7
#define D8              PORTB0
#define D9              PORTB1
#define D10             PORTB2
#define D11             PORTB3
#define D12             PORTB4
#define D13             PORTB5
#define D14             PORTC0
#define D15             PORTC1
#define D16             PORTC2
#define D17             PORTC3
#define D18             PORTC4
#define D19             PORTC5

// ARDUINO LAYOUT ANALOG
#define A0              D14
#define A1              D15
#define A2              D16
#define A3              D17
#define A4              D18
#define A5              D19

// APS LAYOUT
#define OBC_RX              PORTD0      // IN (Digital)         DDRD0:0     PORTD0:0    D0
#define OBC_TX              PORTD1      // OUT (Digital)        DDRD1:1     PORTD1:0    D1
#define HT_PCB_G            PORTD2      // OUT (Digital)        DDRD2:1     PORTD2:0    D2
#define EL_EN               PORTD3      // OUT (Digital)        DDRD3:1     PORTD3:0    D3
#define EL_PH               PORTD4      // OUT (Digital)        DDRD4:1     PORTD4:0    D4
#define EL_ENC_CH_B         PORTD5      // IN (Digital)         DDRD5:0     PORTD5:0    D5
#define EL_ENC_CH_A         PORTD6      // IN (Digital)         DDRD6:0     PORTD6:0    D6
#define HT_EL_G_UB          PORTD7      // OUT (Digital)        DDRD7:1     PORTD7:0    D7

#define HT_AZ_G_UB          PORTB0      // OUT (Digital)        DDRB0:1     PORTB0:0    D8
#define AZ_EN               PORTB1      // OUT (Digital)        DDRB1:1     PORTB1:0    D9
#define AZ_PH               PORTB2      // OUT (Digital)        DDRB2:1     PORTB2:0    D10
#define AZ_ENC_STA          PORTB3      // IN (Digital)         DDRB3:0     PORTB3:0    D11
#define AZ_ENC_PWM          PORTB4      // IN (Digital)         DDRB4:0     PORTB4:0    D12
#define DEBUG_LED           PORTB5      // OUT (Digital)        DDRB5:1     PORTB5:0    D13

#define TH_PCB_A_TEMP       PORTC0      // IN (Analog)          DDRC0:0     PORTC0:0    A0
#define TH_PCB_B_TEMP       PORTC1      // IN (Analog)          DDRC1:0     PORTC1:0    A1
#define TH_AZ_TEMP          PORTC2      // IN (Analog)          DDRC2:0     PORTC2:0    A2
#define TH_EL_TEMP          PORTC3      // IN (Analog)          DDRC3:0     PORTC3:0    A3
#define AZ_SHUNT_UB         PORTC4      // IN (Analog)          DDRC4:0     PORTC4:0    A4
#define EL_SHUNT_UB         PORTC5      // IN (Analog)          DDRC5:0     PORTC5:0    A5

// Initializes the pinout configuration
int8 pinout_init(void);

// Reads the state of a specified pin
// pin: The pin to read (PINOUT_B, PINOUT_C, PINOUT_D)
// mask: The mask to apply to the pin
// returns: The state of the pin (0 or 1), -1 if an error occurs
int8 pinout_pin(uint8 pin, uint8 mask);

// Sets the state of a specified port
// port: The port to set (PINOUT_B, PINOUT_C, PINOUT_D)
// mask: The mask to apply to the port
// act: The action to perform (PINOUT_ENABLE or PINOUT_DISABLE)
// returns: 0 on success, -1 if an error occurs
int8 pinout_port(uint8 pin, uint8 mask, uint8 act);

#endif//PINOUT_H