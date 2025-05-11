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

#include "encoder.h"
#include "usart0.h"

uint32 actr = 0;
uint32 tctr = 0;
varchar(STR64, serial_tx);

ISR(TIMER0_COMPA_vect,ISR_BLOCK){
    tctr++;
    actr += pinout_pin(PINOUT_B, AZ_ENC_PWM);
    if(tctr >= PW_STEPS)
    {
        TIMSK0 &= !(1 << OCIE0A);
    }
}

int8 encoder_inc_get_state(PENCODER_INC encoder_inc)
{
    encoder_inc->state = 
        (pinout_pin(encoder_inc->pinA, encoder_inc->maskA) | 
        pinout_pin(encoder_inc->pinB, encoder_inc->maskB) << 1
    );    
    return 0;
}

int8 encoder_abs_read(PENCODER_ABS encoder_abs)
{
    if(!encoder_abs)
        return -1;
    tctr = 0;
    actr = 0;
    TIMSK0 |= (1 << OCIE0A);
    while(bit_is_set(TIMSK0,OCIE0A));
    encoder_abs->angle=(actr >> 3)-1;
    return 0;
}

int8 encoder_inc_read(PENCODER_INC encoder_inc)
{
    if(!encoder_inc)
        return -1;
    
    uint8 state = encoder_inc->state;                               // Store previous state
    encoder_inc_get_state(encoder_inc);                             // Store new state
    
    if(state != encoder_inc->state)                                 // If position changed
    {
        if(state == ENCODER_INC_STATE_1)
        {
            if(encoder_inc->state == ENCODER_INC_STATE_4)
            {
                encoder_inc->direction = ENCODER_DIR_CW;
            }
            else if(encoder_inc->state == ENCODER_INC_STATE_2)
            {
                encoder_inc->direction = ENCODER_DIR_CCW;
            }
        }
        else if (state == ENCODER_INC_STATE_2)
        {
            if(encoder_inc->state == ENCODER_INC_STATE_1)
            {
                encoder_inc->direction = ENCODER_DIR_CW;
            }
            else if(encoder_inc->state == ENCODER_INC_STATE_3)
            {
                encoder_inc->direction = ENCODER_DIR_CCW;
            }
        }
        else if (state == ENCODER_INC_STATE_3)
        {
            if(encoder_inc->state == ENCODER_INC_STATE_2)
            {
                encoder_inc->direction = ENCODER_DIR_CW;
            }
            else if(encoder_inc->state == ENCODER_INC_STATE_4)
            {
                encoder_inc->direction = ENCODER_DIR_CCW;
            }
        }
        else if (state == ENCODER_INC_STATE_4)
        {
            if(encoder_inc->state == ENCODER_INC_STATE_3)
            {
                encoder_inc->direction = ENCODER_DIR_CW;
            }
            else if(encoder_inc->state == ENCODER_INC_STATE_1)
            {
                encoder_inc->direction = ENCODER_DIR_CCW;
            }
        }
        encoder_inc->angle += encoder_inc->direction;
    }

    
    return 0;
}
