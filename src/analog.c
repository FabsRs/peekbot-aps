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

#include "analog.h"


/*
 * Function: analog_init
 * --------------------
 * Initializes the analog module by setting reference voltages, enabling analog inputs,
 * and setting the ADC prescaler to its maximum value.
 *
 * returns: 0 on success
 */
int8 analog_init()
{
    ADMUX |= (1 << REFS0) | (1 << REFS1);   // NOTE: Assumes capacitor at AREF pin (Arduino Nano)
    DIDR0 |= (1 << ADC5D) | (1 << ADC4D) | (1 << ADC3D) | (1 << ADC2D) | (1 << ADC1D) | (1 << ADC0D);   // Enable Analog input on all analog pins
    ADCSRA |= (1 << ADEN);  // Enable the ADC module
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);   // Set maximum prescaler

    return 0;
}

/*
 * Function: analog_read
 * --------------------
 * Reads the value from the specified analog input channel.
 *
 * analog: pointer to an ANALOG structure containing the mask for the input channel
 *
 * returns: 0 on success, -1 if the read operation times out
 */
int8 analog_read(PANALOG analog)
{
    uint16 maxctr = 1000;
    ADMUX = (ADMUX & 0xF0) | analog->mask;
    ADCSRA |= (1 << ADSC);
    while(maxctr-- && (ADCSRA & (1 << ADSC)));
    if(!maxctr)
        return -1;
    analog->value = ADCL | (ADCH << 8);
    return 0;
}