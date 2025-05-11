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

#ifndef ENCODER_H
#define ENCODER_H

#include "avr.h"
#include "ccommons.h"
#include "pinout.h"

#define ENCODER_DIR_CCW     -1
#define ENCODER_DIR_CW      1

#define ENCODER_INC_STATE_1       0x00
#define ENCODER_INC_STATE_2       0x02
#define ENCODER_INC_STATE_3       0x03
#define ENCODER_INC_STATE_4       0x01

#define PW_STEPS    8192
#define SAMPLING    20


// Structure to represent an absolute encoder
// pinPWM: PWM pin for the encoder
// maskPWM: Mask for the PWM pin
// pinSTA: Status pin for the encoder (Absolute encoder position status: 11, 01, 00, 10)
// maskSTA: Mask for the status pin
// angle: Current angle of the encoder
typedef struct _ENCODER_ABS
{
    // Parameters
    int8 pinPWM;
    int8 maskPWM;
    int8 pinSTA;
    int8 maskSTA;
    // State Variables
    int32 angle;

}*PENCODER_ABS, ENCODER_ABS;


// Structure to represent an incremental encoder
// pinA: Pin A for the encoder
// maskA: Mask for pin A
// pinB: Pin B for the encoder
// maskB: Mask for pin B
// ppr: Pulses per revolution
// angle: Current angle of the encoder
// direction: Current direction of rotation
// state: Current state of the encoder (unused)
typedef struct _ENCODER_INC
{
    // Parameters
    int8 pinA;
    int8 maskA;
    int8 pinB;
    int8 maskB;
    uint16 ppr;
    // State Variables
    int32 angle;
    int8 direction;
    uint8 state;
}*PENCODER_INC, ENCODER_INC;


// Gets the current state of the incremental encoder
int8 encoder_inc_get_state(PENCODER_INC encoder_inc);

// Reads the value from the absolute encoder
int8 encoder_abs_read(PENCODER_ABS encoder_abs);

// Reads the value from the incremental encoder
int8 encoder_inc_read(PENCODER_INC encoder_inc);

#endif//ENCODER_H