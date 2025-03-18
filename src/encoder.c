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

uint16 enc_abs_counter = 0;
uint16 enc_tim_counter = 0;

varchar(STR64, serial_tx);

int8 az_preverr(PENCODER_ABS encoder_abs)
{
    encoder_abs->az_preverr=encoder_abs->az_err; //Set previous loop's error value to new "previous erro" value 
}
int8 az_err(PENCODER_ABS encoder_abs)
{
    encoder_abs->az_err=encoder_abs->az_tar-encoder_abs->az_pos; //Deviation from set target in encoder positions (integer)
}
int8 az_errcnt(PENCODER_ABS encoder_abs) //Assuming 2048PPR absolute, change thresholds for higher PPR
{
    if((abs(encoder_abs->az_err)>int8 1)&&(abs(encoder_abs->az_err-encoder_abs->az_preverr)<32)) //use loop count instead of time to ignore loop time variance and keep integers
    {
        encoder_abs->az_errcnt++;
    }
} 
int8 az_interr(PENCODER_ABS encoder_abs) //the longer the error remains out of bounds the higher the integral response
{
    az_errcnt(encoder_abs);
    encoder_abs->az_interr+=encoder_abs->az_err*encoder_abs->az_errcnt;
}
int8 az_dererr(PENCODER_ABS encoder_abs) //numerical derivative of error, causes damping. 
{
    encoder_abs->az_dererr=(encoder_abs->az_err-encoder_abs->az_preverr) //Derivative period is 1 loop count.
}

int8 el_preverr(PENCODER_INC encoder_inc)
{
    encoder_inc->el_preverr=encoder_inc->el_err; //Set previous loop's error value to new "previous erro" value 
}
int8 el_errcnt(PENCODER_INC encoder_inc) //Assuming 2048PPR, change thresholds for higher PPR
{
    if((abs(encoder_inc->el_err)>int8 1)&&(abs(encoder_inc->el_err-encoder_inc->el_preverr)<32)) //use loop count instead of time to ignore loop time variance and keep integers
    {
        encoder_inc->el_errcnt++;
    }
} 
int8 el_interr(PENCODER_INC encoder_inc)
{
    el_errcnt(encoder_inc);
    encoder_inc->el_interr+=encoder_inc->el_err*encoder_inc->el_errcnt;
}
int8 el_dererr(PENCODER_INC encoder_inc)
{
    encoder_inc->el_dererr=(encoder_inc->el_err-encoder_inc->el_preverr) //Derivative period is 1 loop count.
}
ISR(TIMER0_COMPA_vect,ISR_BLOCK){ 
    enc_tim_counter++;
    enc_abs_counter += bit_is_set(PINB, AZ_ENC_PWM);
    TIMSK0 |= !(enc_tim_counter%16384) << OCIE0A;
    OCR0A = (0x03);             // Set Timer output to 2MHz
    TCCR0A |= (1 << WGM01);     // Set to CTC OCRA immediate stop at MAX
    pinout_port(PINOUT_B, DEBUG_LED, PINOUT_ENABLE);
}



int8 encoder_abs_angle(PENCODER_ABS encoder_abs)
{
    if(!encoder_abs)
        return -1;
    encoder_abs->angle=(enc_abs_counter>>2)-1;
    memset(serial_tx, 0, STR64);
    snprintf(serial_tx, STR64, "Angle[%d]\n", enc_abs_counter);
    usart0_serial_tx(serial_tx, strlen(serial_tx));
    enc_tim_counter = 0;
    enc_abs_counter = 0;
    return 0;
}

int8 encoder_abs_calibrate(PENCODER_ABS encoder_abs)
{
    if(!encoder_abs)
        return -1;
    for(int i = 0 ; i < X2PW_TICKS/16 && (!pinout_pin(encoder_abs->pinPWM,encoder_abs->maskPWM)) ; i++);
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = (0x03);             // Set Timer output to 2MHz
    TCCR0A |= (1 << WGM01);     // Set to CTC OCRA immediate stop at MAX
    return 0;
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
    if(encoder_abs_calibrate(encoder_abs))
        return -1;
    for(int i = 0 ; i < X2PW_TICKS/16 && bit_is_set(TIMSK0,OCIE0A) ; i++);
    if(encoder_abs_angle(encoder_abs))
        return -1;
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
