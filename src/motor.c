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

#define MOTOR_OC2B              0x00
#define MOTOR_OC1A              0x01
#define MOTOR_DIRECTION_CCW     0x01
#define MOTOR_DIRECTION_CW      0x00
#define MOTOR_INVERTED_ENABLED  0x00
#define MOTOR_INVERTED_DISABLED 0x01

typedef struct _MOTOR
{
    // Parameters
    uint8 timer;
    int8 inverted;
    uint8 pinPH;
    uint8 maskPH;
    // State
    uint8 direction;
    uint8 ocrnx;
}*PMOTOR, MOTOR;

int8 motor_init(void)
{
    OCR2B = 0;
    TCCR2A |= (1 << COM2B1);
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);
    return 0;
}

int8 motor_set(PMOTOR motor, uint8 percentage, uint8 direction)
{
    if(!motor)
        return;
    motor->ocrnx = (int8)((256/100) * percentage);
    motor->direction = direction;
    //set direction (default CCW)
    if(direction == motor->inverted)
    {
        pinout_port(motor->pinPH, motor->maskPH, PINOUT_ENABLE);
    }
    else
    {
        pinout_port(motor->pinPH, motor->maskPH, PINOUT_ENABLE);
    }
    // Set velocity
    if(motor->timer == MOTOR_OC2B)
    {
        OCR2B = motor->ocrnx;
    }
    else if(motor->timer == MOTOR_OC1A)
    {
        OCR1A = motor->ocrnx;
    }
    return 0;
}