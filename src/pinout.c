#include "pinout.h"

int8 pinout_init(void)
{
    DDRB = (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB7) | (1 << DDB5);
    DDRC = 0x00;
    DDRD = (1 << DDD0) | (1 << DDD1) | (1 << DDD2);
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    return 0;
}

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