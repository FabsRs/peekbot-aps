#include "pinout.h"

int8 pinout_init(void)
{
    DDRB = 0x00;
    DDRC = 0x00;
    DDRD = 0x00;

    DDRB = (1 << HT_AZ_G_UB) | (1 << AZ_EN) | (1 << AZ_PH) | (1 << DEBUG_LED);
    DDRC = 0x00;
    DDRD = (1 << OBC_TX) | (1 << HT_PCB_G) | (1 << EL_EN) | (1 << EL_PH) | (1 << HT_EL_G_UB);
    
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0X00;
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