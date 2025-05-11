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

#define SERIAL      1024
#define AZ_OFFSET   PW_STEPS >> 1

/*
 * Function: debug_print
 * --------------------
 * Sends a debug message via USART0.
 *
 * serial_tx: Pointer to the string to be transmitted
 *
 * returns: 0 on success
 */
int8 debug_print(char* serial_tx);

/*
 * Function: main
 * --------------------
 * Main function that initializes various components and handles serial communication for motor control.
 *
 * Initializes buffers, USART0 parameters, encoder and motor configurations, and sets up the main loop for handling commands.
 *
 * returns: 0 on success
 */
int main(void)
{
    uint8 rxBuffer[RX_BUFFER_SIZE];
    uint8 txBuffer[TX_BUFFER_SIZE];
    varchar(STR64, serial_rx);
    varchar(STR64, serial_tx);
    char* token;
    ENCODER_ABS orbis;
    ENCODER_INC amt103;
    MOTOR TransmotecAZ;
    MOTOR TransmotecEL;
    ANALOG senseAZ;
    ANALOG senseEL;

    uint8 debug = 0;
    uint8 direction = 0;
    uint8 percentage = 0;
    int8 turnCount = 0;
    int32 angle = 0;
    int32 deltaAngle = 0;
    int32 realAngle = 0;
    int32 prevAngle = 0;

    uint32 tolerance = 0;
    uint32 stoprange1 = 0;
    uint32 stoprange2 = 0;
    uint32 stoprange3 = 0;

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

    orbis.pinPWM = PINOUT_B;
    orbis.maskPWM = AZ_ENC_PWM;
    orbis.pinSTA = PINOUT_B;
    orbis.maskSTA = AZ_ENC_STA;
    orbis.angle = 0;

    TransmotecAZ.pinPH = PINOUT_B;
    TransmotecAZ.maskPH = AZ_PH;
    TransmotecAZ.timer = MOTOR_OC1A;
    TransmotecAZ.inverted = MOTOR_INVERTED_ENABLED;
    TransmotecAZ.direction = MOTOR_DIRECTION_CCW;
    TransmotecAZ.ocrnx = 0;

    TransmotecEL.pinPH = PINOUT_D;
    TransmotecEL.maskPH = EL_PH;
    TransmotecEL.timer = MOTOR_OC2B;
    TransmotecEL.inverted = MOTOR_INVERTED_DISABLED;
    TransmotecEL.direction = MOTOR_DIRECTION_CCW;
    TransmotecEL.ocrnx = 0;

    senseAZ.mask = AZ_SHUNT_UB;
    senseEL.mask = EL_SHUNT_UB;

    pinout_init();
    timer_init();
    motor_init();
    analog_init();
    usart0_init(rxBuffer, RX_BUFFER_SIZE, txBuffer, TX_BUFFER_SIZE);

    debug = 0;

    sei();
    
    motor_set(&TransmotecAZ, 0, MOTOR_DIRECTION_CCW);
    motor_set(&TransmotecEL, 0, MOTOR_DIRECTION_CCW);
    encoder_inc_get_state(&amt103);
    encoder_abs_read(&orbis);
    
    while(1){

        int count = 0;
        while(!usart0_serial_rx_count());
        _delay_ms(500);
        count = usart0_serial_rx_count();
        memset(serial_rx, 0, STR64);
        usart0_serial_rx(serial_rx, count);
        token = strtok(serial_rx, ";");  // Get first token

        while(token)
        {
            if(!strcmp(token, "EL"))
            {
                debug_print("Starting Elevation\n");
                
                token = strtok(NULL, ";");
                percentage = atoi(token);
                
                token = strtok(NULL, ";");
                angle = atoi(token);

                if(amt103.angle >= angle)
                {
                    direction = MOTOR_DIRECTION_CCW;
                }
                else if(amt103.angle < angle)
                {
                    direction = MOTOR_DIRECTION_CW;
                }
    
                motor_set(&TransmotecEL, percentage, direction);
                stoprange1 = (PW_STEPS / 256);
                stoprange2 = (PW_STEPS / 128);
                prevAngle = amt103.angle;
                for(int i = 0; amt103.angle != angle ; i++)
                {
                    deltaAngle = abs(amt103.angle - angle);

                    if(deltaAngle < stoprange1)
                        motor_set(&TransmotecEL, 10, direction);
                    else if(deltaAngle < stoprange2)
                        motor_set(&TransmotecEL, 50, direction);

                    encoder_inc_read(&amt103);

                    // if(prevAngle != amt103.angle)
                    // {
                    //     memset(serial_tx, 0, STR64);
                    //     snprintf(serial_tx, STR64, "%s\t%ld[%x]\n", (amt103.direction == ENCODER_DIR_CW) ? "CW" : "CCW", amt103.angle, amt103.state);
                    //     usart0_serial_tx(serial_tx, strlen(serial_tx));
                    //     prevAngle = amt103.angle;
                    // }
                }
                motor_set(&TransmotecEL, 0, direction);
                debug_print("Stopping Elevation\n");
                memset(serial_tx, 0, STR64);
                snprintf(serial_tx, STR64, "%s\t%ld[%x]\n", (amt103.direction == ENCODER_DIR_CW) ? "CW" : "CCW", amt103.angle, amt103.state);
                usart0_serial_tx(serial_tx, strlen(serial_tx));
            }
            else if(!strcmp(token, "AZ"))
            {
                debug_print("Starting Azimuth\n");
                
                token = strtok(NULL, ";");
                percentage = atoi(token);
                
                token = strtok(NULL, ";");
                angle = atoi(token);
                angle += AZ_OFFSET;
                
                realAngle=orbis.angle+turnCount*PW_STEPS;
                deltaAngle = abs(realAngle - angle);

                if(realAngle >= angle)
                {
                    direction = MOTOR_DIRECTION_CCW;
                }
                else if(realAngle < angle)
                {
                    direction = MOTOR_DIRECTION_CW;
                }

                motor_set(&TransmotecAZ, percentage, direction);
                tolerance = (PW_STEPS / 550);
                stoprange1 = (PW_STEPS / 64);
                stoprange2 = (PW_STEPS / 32);
                stoprange3 = (PW_STEPS / 16);
                for(int i = 0; i < 10000 && deltaAngle >= tolerance; i++)
                {
                    if(realAngle >= angle)
                    {
                        direction = MOTOR_DIRECTION_CCW;
                    }
                    else if(realAngle < angle)
                    {
                        direction = MOTOR_DIRECTION_CW;
                    }
                    // Smooth curve
                    if(deltaAngle < stoprange1)
                        motor_set(&TransmotecAZ, 5, direction);
                    else if(deltaAngle < stoprange2)
                        motor_set(&TransmotecAZ, 10, direction);
                    else if(deltaAngle < stoprange3)
                        motor_set(&TransmotecAZ, 50, direction);

                    prevAngle = orbis.angle;
                    encoder_abs_read(&orbis);

                    if(direction == MOTOR_DIRECTION_CCW && (prevAngle - orbis.angle) < -(PW_STEPS >> 1))
                    {
                        turnCount--;
                    }
                    else if(direction == MOTOR_DIRECTION_CW && (prevAngle - orbis.angle) > (PW_STEPS >> 1))
                    {
                        turnCount++;
                    }

                    realAngle=orbis.angle+turnCount*PW_STEPS;

                    deltaAngle = abs(realAngle - angle);
                    
                    memset(serial_tx, 0, STR64);
                    snprintf(serial_tx, STR64, "T[%d] R[%ld] A[%ld] P[%ld] O[%ld] D[%ld] D[%s]\n", turnCount, realAngle, angle, prevAngle, orbis.angle, deltaAngle, (direction == MOTOR_DIRECTION_CCW) ? "CCW" : "CW");
                    usart0_serial_tx(serial_tx, strlen(serial_tx));
                }
                motor_set(&TransmotecAZ, 0, direction);
                debug_print("Stopping Azimuth\n");
                memset(serial_tx, 0, STR64);
                snprintf(serial_tx, STR64, "T[%d] R[%ld] A[%ld] P[%ld] O[%ld] D[%ld] D[%s]\n", turnCount, realAngle, angle, prevAngle, orbis.angle, deltaAngle, (direction == MOTOR_DIRECTION_CCW) ? "CCW" : "CW");
                usart0_serial_tx(serial_tx, strlen(serial_tx));
            }

            // Get next command
            token = strtok(NULL, ";");
        }
    }

    // Old
    
    if(debug & DEBUG_AMT103)
    {
        memset(serial_tx, 0, STR64);
        snprintf(serial_tx, STR64, "%s\t%ld[%x]\n", (amt103.direction == ENCODER_DIR_CW) ? "CW" : "CCW", amt103.angle, amt103.state);
        usart0_serial_tx(serial_tx, strlen(serial_tx));
    }

    if(debug & DEBUG_ORBIS)
    {
        memset(serial_tx, 0, STR64);
        // encoder_abs_angle(&orbis);
        usart0_serial_tx(serial_tx, strlen(serial_tx));
    }

    if(debug & DEBUG_EL_MOT)
    {
        memset(serial_tx, 0, STR64);
        motor_set(&TransmotecEL, 50, MOTOR_DIRECTION_CW);
        snprintf(serial_tx, STR64, "%s\t%d\n", (TransmotecEL.direction == MOTOR_DIRECTION_CCW) ? "CCW" : "CW", TransmotecEL.ocrnx);
        usart0_serial_tx(serial_tx, strlen(serial_tx));
    }

    if(debug & DEBUG_AZ_MOT)
    {
        memset(serial_tx, 0, STR64);
        motor_set(&TransmotecAZ, 0, MOTOR_DIRECTION_CCW);
        snprintf(serial_tx, STR64, "%s\t%d\n", (TransmotecAZ.direction == MOTOR_DIRECTION_CCW) ? "CCW" : "CW", TransmotecAZ.ocrnx);
        usart0_serial_tx(serial_tx, strlen(serial_tx));
    }

    if(debug & DEBUG_EL_SHUNT)
    {
        memset(serial_tx, 0, STR64);
        analog_read(&senseEL);
        snprintf(serial_tx, STR64, "value EL: %d\n", senseEL.value);
        usart0_serial_tx(serial_tx, strlen(serial_tx));
    }

    if(debug & DEBUG_AZ_SHUNT)
    {
        memset(serial_tx, 0, STR64);
        analog_read(&senseAZ);
        snprintf(serial_tx, STR64, "value AZ: %d\n", senseAZ.value);
        usart0_serial_tx(serial_tx, strlen(serial_tx));
    }

    return 0;
}

int8 debug_print(char* serial_tx)
{
    usart0_serial_tx(serial_tx, strlen(serial_tx));
    return 0;
}


