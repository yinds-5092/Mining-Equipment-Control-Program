#ifndef _CMP_H_
#define _CMP_H_

typedef enum
{
    CMP_POS_P37 = 0x00,
    CMP_POS_P50 = 0x01,
    CMP_POS_P51 = 0x02,
    CMP_POS_ADC = 0x03
} CMP_PositiveInput;

typedef enum
{
    CMP_NEG_P36 = 0x00,
    CMP_NEG_REFV = 0x04
} CMP_NegativeInput;

typedef enum
{
    CMP_HYS_0MV = 0x00,
    CMP_HYS_10MV = 0x40,
    CMP_HYS_20MV = 0x80,
    CMP_HYS_30MV = 0xc0
} CMP_Hysteresis;

typedef enum
{
    CMP_OUTPUT_DISABLED = 0,
    CMP_OUTPUT_P34 = 1,
    CMP_OUTPUT_P41 = 2
} CMP_Output;

void cmp_init(CMP_PositiveInput positive, CMP_NegativeInput negative,
              CMP_Output output, uint8 invert)
{
    P_SW2 |= 0x80; /* Enable access to extended-function registers. */

    if (positive == CMP_POS_P37)
    {
        P3M0 &= ~0x80;
        P3M1 |= 0x80;
    }
    else if (positive == CMP_POS_P50)
    {
        P5M0 &= ~0x01;
        P5M1 |= 0x01;
    }
    else if (positive == CMP_POS_P51)
    {
        P5M0 &= ~0x02;
        P5M1 |= 0x02;
    }

    if (negative == CMP_NEG_P36)
    {
        P3M0 &= ~0x40;
        P3M1 |= 0x40;
    }

    CMPEXCFG = (uint8)(negative | positive);
    CMPCR2 = invert ? 0x80 : 0x00;

    if (output == CMP_OUTPUT_P41)
        P_SW2 |= 0x08;
    else
        P_SW2 &= ~0x08;

    CMPCR1 = (uint8)(0x80 | (output == CMP_OUTPUT_DISABLED ? 0x00 : 0x02));
}

void cmp_filter_config(CMP_Hysteresis hysteresis, uint8 filter_clocks,
                       uint8 analog_filter)
{
    CMPEXCFG = (uint8)((CMPEXCFG & ~0xc0) | hysteresis);
    CMPCR2 = (uint8)((CMPCR2 & 0x80) |
                     (analog_filter ? 0x00 : 0x40) |
                     (filter_clocks & 0x3f));
}

void cmp_enable(uint8 enable)
{
    if (enable)
        CMPCR1 |= 0x80;
    else
        CMPCR1 &= ~0x80;
}

uint8 cmp_read(void)
{
    return (CMPCR1 & 0x01) ? 1 : 0;
}

#endif
