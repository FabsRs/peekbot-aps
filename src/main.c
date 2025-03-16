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

#include "core.h"

#define DEBUG_AMT103        0x01
#define DEBUG_ORBIS         0x02
#define DEBUG_AZ_MOT        0x04
#define DEBUG_EL_MOT        0x08
#define DEBUG_AZ_SHUNT      0x10
#define DEBUG_EL_SHUNT      0x20

#define SERIAL 1024

int main(void)
{
    uint8 rxBuffer[RX_BUFFER_SIZE];
    uint8 txBuffer[TX_BUFFER_SIZE];
    varchar(STR64, serial);
    ENCODER_ABS orbis;
    ENCODER_INC amt103;
    MOTOR TransmotecAZ;
    MOTOR TransmotecEL;
    uint32 time = 0;
    uint8 debug = 0;

    usart0.params.mode = USART0_MODE_ASYNC_NORMAL;
    usart0.params.baudrate = USART0_BR_57600;
    usart0.params.databits = USART0_DATABITS_8;
    usart0.params.parity = USART0_PARITY_NONE;
    usart0.params.stopbits = USART0_STOPBITS_1;
    usart0.params.rx = USART0_RX_ENABLE;
    usart0.params.tx = USART0_TX_ENABLE;
    usart0.params.timeout_ms = 250;

    amt103.pinA = PINOUT_D;
    amt103.pinB = PINOUT_D;
    amt103.maskA = EL_ENC_CH_A;
    amt103.maskB = EL_ENC_CH_B;
    amt103.ppr = 2048;
    amt103.state = 0;
    amt103.angle = 0;
    amt103.direction = 0;
    encoder_inc_get_state(&amt103);

    orbis.pinPWM = PINOUT_B;
    orbis.maskPWM = AZ_ENC_PWM;
    orbis.pinSTA = PINOUT_B;
    orbis.maskSTA = AZ_ENC_STA;
    orbis.angle = 0;

    TransmotecAZ.pinPH = PINOUT_B;
    TransmotecAZ.maskPH = AZ_PH;
    TransmotecAZ.timer = MOTOR_OC1A;
    TransmotecAZ.inverted = MOTOR_INVERTED_DISABLED;
    TransmotecAZ.direction = MOTOR_DIRECTION_CCW;
    TransmotecAZ.ocrnx = 0;

    TransmotecEL.pinPH = PINOUT_D;
    TransmotecEL.maskPH = EL_PH;
    TransmotecEL.timer = MOTOR_OC2B;
    TransmotecEL.inverted = MOTOR_INVERTED_DISABLED;
    TransmotecEL.direction = MOTOR_DIRECTION_CCW;
    TransmotecEL.ocrnx = 0;

    pinout_init();
    timer_init();
    motor_init();
    usart0_init(rxBuffer, RX_BUFFER_SIZE, txBuffer, TX_BUFFER_SIZE);

    debug = (DEBUG_EL_MOT | DEBUG_AZ_MOT);

    uint8 speed = 0;
    uint8 direction = 0;

    sei();

    while(1){
        time++;
        encoder_inc_read(&amt103);
        if(time >= 1000)
        {
            if(debug & DEBUG_AMT103)
            {
                memset(serial, 0, STR64);
                snprintf(serial, STR64, "%s\t%ld[%x]\n", (amt103.direction == ENCODER_DIR_CW) ? "CW" : "CCW", amt103.angle, amt103.state);
                usart0_serial_tx(serial, strlen(serial));
            }

            if(debug & DEBUG_ORBIS)
            {
                memset(serial, 0, STR64);
                encoder_abs_angle(&orbis);
                usart0_serial_tx(serial, strlen(serial));
            }

            if(debug & DEBUG_EL_MOT)
            {
                speed++;
                if(speed > 100)
                {
                    if(direction == MOTOR_DIRECTION_CCW)
                        direction = MOTOR_DIRECTION_CW;
                    else if(direction == MOTOR_DIRECTION_CW)
                        direction = MOTOR_DIRECTION_CCW;
                    speed = 0;
                }
                memset(serial, 0, STR64);
                motor_set(&TransmotecEL, speed, direction);
                snprintf(serial, STR64, "%s\t%d\n", (TransmotecEL.direction == MOTOR_DIRECTION_CCW) ? "CCW" : "CW", TransmotecEL.ocrnx);
                usart0_serial_tx(serial, strlen(serial));
            }

            if(debug & DEBUG_AZ_MOT)
            {
                speed++;
                if(speed > 100)
                {
                    if(direction == MOTOR_DIRECTION_CCW)
                        direction = MOTOR_DIRECTION_CW;
                    else if(direction == MOTOR_DIRECTION_CW)
                        direction = MOTOR_DIRECTION_CCW;
                    speed = 0;
                }
                memset(serial, 0, STR64);
                motor_set(&TransmotecAZ, speed, direction);
                snprintf(serial, STR64, "%s\t%d\n", (TransmotecAZ.direction == MOTOR_DIRECTION_CCW) ? "CCW" : "CW", TransmotecAZ.ocrnx);
                usart0_serial_tx(serial, strlen(serial));
            }

            if(debug & DEBUG_EL_SHUNT)
            {
                memset(serial, 0, STR64);
                usart0_serial_tx(serial, strlen(serial));
            }

            if(debug & DEBUG_AZ_SHUNT)
            {
                memset(serial, 0, STR64);
                usart0_serial_tx(serial, strlen(serial));
            }
            
            time = 0;
        }

    }

    return 0;
}


