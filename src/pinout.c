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

#include "pinout.h"

/*
 * Function: pinout_init
 * --------------------
 * Initializes the pinout configuration by setting the data direction registers and port registers.
 *
 * returns: 0 on success
 */
int8 pinout_init(void)
{
    DDRB = 0x00;
    DDRC = 0x00;
    DDRD = 0x00;

    DDRB = (1 << HT_AZ_G_UB) | (1 << AZ_EN) | (1 << AZ_PH) | (1 << DEBUG_LED);
    DDRC = 0x00;
    DDRD = (1 << OBC_TX) | (1 << HT_PCB_G) | (1 << EL_EN) | (1 << EL_PH) | (1 << HT_EL_G_UB);
    
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0X00;
    return 0;
}

/*
 * Function: pinout_pin
 * --------------------
 * Reads the state of a specified pin by applying the given mask.
 *
 * pin: The pin to read (PINOUT_B, PINOUT_C, PINOUT_D)
 * mask: The mask to apply to the pin
 *
 * returns: The state of the pin (0 or 1), -1 if an error occurs
 */
int8 pinout_pin(uint8 pin, uint8 mask)
{
    switch (pin)
    {
    case PINOUT_B:
        return bit_is_set(PINB, mask) >> mask;
        break;
    case PINOUT_C:
        return bit_is_set(PINC, mask) >> mask;
        break;
    case PINOUT_D:
        return bit_is_set(PIND, mask) >> mask;
        break;
    }
    return -1;
}

/*
 * Function: pinout_port
 * --------------------
 * Sets the state of a specified port by applying the given mask and action.
 *
 * port: The port to set (PINOUT_B, PINOUT_C, PINOUT_D)
 * mask: The mask to apply to the port
 * act: The action to perform (PINOUT_ENABLE or PINOUT_DISABLE)
 *
 * returns: 0 on success, -1 if an error occurs
 */
int8 pinout_port(uint8 port, uint8 mask, uint8 act)
{
    switch (port)
    {
    case PINOUT_B:
        if (act == PINOUT_ENABLE)
            PORTB |= (1 << mask);
        else if (act == PINOUT_DISABLE)
            PORTB &= ~(1 << mask);
        else
            return -1;
        break;
    case PINOUT_C:
        if (act == PINOUT_ENABLE)
            PORTC |= (1 << mask);
        else if (act == PINOUT_DISABLE)
            PORTC &= ~(1 << mask);
        else
            return -1;
        break;
    case PINOUT_D:
        if (act == PINOUT_ENABLE)
            PORTD |= (1 << mask);
        else if (act == PINOUT_DISABLE)
            PORTD &= ~(1 << mask);
        else
            return -1;
        break;
    }
    return -1;
}