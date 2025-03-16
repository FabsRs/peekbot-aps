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

#include "motor.h"

int8 motor_init_oc2b(void)
{
    TCCR2A |= (1 << COM2B1);
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);
    return 0;
}

int8 motor_init_oc1a(void)
{
    TCCR1A |= (1 << COM1A1);
    TCCR1A |= (1 << WGM10);
    TCCR1B |= (1 << WGM12) |(1 << CS10) | (1 << CS12);
    return 0;
}

int8 motor_init(void)
{
    motor_init_oc2b();
    motor_init_oc1a();
    return 0;
}

int8 motor_set(PMOTOR motor, uint16 percentage, uint8 direction)
{
    if(!motor)
        return -1;
    if(percentage > 100)
        percentage = 100;
    motor->ocrnx = (int8)((255 * percentage) / 100);
    motor->direction = direction;
    //set direction (default CCW)
    if(direction == motor->inverted)
    {
        pinout_port(motor->pinPH, motor->maskPH, PINOUT_ENABLE);
    }
    else
    {
        pinout_port(motor->pinPH, motor->maskPH, PINOUT_DISABLE);
    }
    // Set velocity
    if(motor->timer == MOTOR_OC2B)
    {
        if(!motor->ocrnx)
        {
            TCCR2A &= !(1 << COM2B1);
        }
        else
        {
            motor_init_oc2b();
            OCR2B = motor->ocrnx;
        }
    }
    else if(motor->timer == MOTOR_OC1A)
    {
        if(!motor->ocrnx)
        {
            TCCR1A &= !(1 << COM1A1);
        }
        else
        {
            motor_init_oc1a();
            OCR1A = motor->ocrnx;
        }
    }
    return 0;
}