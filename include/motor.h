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

#ifndef MOTOR_H
#define MOTOR_H

#include <util/delay.h>
#include "avr.h"
#include "ccommons.h"
#include "pinout.h"

#define MOTOR_OC2B              0x00
#define MOTOR_OC1A              0x01
#define MOTOR_DIRECTION_CCW     0x01
#define MOTOR_DIRECTION_CW      0x00
#define MOTOR_INVERTED_ENABLED  0x00
#define MOTOR_INVERTED_DISABLED 0x01

// Structure to represent a motor
// timer: Timer pin used for PWM control (OCRnA, OCRnB)
// inverted: Flag indicating if the motor direction is inverted
// pinPH: Pin for setting the motor direction (Connected to equivalent pin of motor driver)
// maskPH: Mask for the direction pin
// direction: Current direction of the motor
// ocrnx: Output compare register value for PWM control (Top value of the match interrupt)
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

// Initializes the motor control using timer 2B
int8 motor_init_oc2b(void);

// Initializes the motor control using timer 1A
int8 motor_init_oc1a(void);

// Initializes the motor control
int8 motor_init(void);

// Sets the motor speed and direction
int8 motor_set(PMOTOR motor, uint16 percentage, uint8 direction);

#endif//MOTOR_H