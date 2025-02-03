#include "pinout.h"

int8 pinout_init(void)
{
    DDRB = (1<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3) | (1<<DDB4) | (1<<DDB5);
    DDRC = 0x00;
    DDRD = (1<<DDD6) | (1<<DDD7);
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    return 0;
}