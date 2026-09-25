#ifndef __HARDIIC_SLAVE_H
#define __HARDIIC_SLAVE_H

#include <STC8HX.h>

/*
 * Thin STC8H hardware-I2C slave layer.
 *
 * This file configures only the peripheral, address and pin mapping. It does
 * not define an interrupt vector, protocol state machine, callback, register
 * map, event logger or timeout policy. The application owns the I2C ISR and
 * handles I2CSLST/I2CRXD/I2CTXD directly, as in STC's reference examples.
 *
 * Include this implementation header in exactly one C source file. SDA and
 * SCL require external pull-up resistors.
 */

#define HARDIIC_SLAVE_OK                 0u
#define HARDIIC_SLAVE_ERROR_ARGUMENT     1u
#define HARDIIC_SLAVE_ERROR_PIN          2u
#define HARDIIC_SLAVE_ERROR_BUSY         3u
#define HARDIIC_SLAVE_ERROR_STATE        4u

#define HARDIIC_SLAVE_ACK                0u
#define HARDIIC_SLAVE_NAK                1u
#define HARDIIC_SLAVE_WRITE              0u
#define HARDIIC_SLAVE_READ               1u

/* P_SW2.I2C_S[1:0] mappings. Names are SDA first, SCL second. */
#define HARDIIC_SLAVE_P14_P15            0u
#define HARDIIC_SLAVE_P24_P25            1u
#define HARDIIC_SLAVE_P76_P77            2u
#define HARDIIC_SLAVE_P33_P32            3u

/* I2CSLST bits. */
#define HARDIIC_SLAVE_SL_BUSY            0x80u
#define HARDIIC_SLAVE_SL_START           0x40u
#define HARDIIC_SLAVE_SL_RX              0x20u
#define HARDIIC_SLAVE_SL_TX              0x10u
#define HARDIIC_SLAVE_SL_STOP            0x08u
#define HARDIIC_SLAVE_SL_ACK_IN          0x02u
#define HARDIIC_SLAVE_SL_ACK_OUT         0x01u

/* I2CSLCR interrupt-enable bits. */
#define HARDIIC_SLAVE_INT_START          0x40u
#define HARDIIC_SLAVE_INT_RX             0x20u
#define HARDIIC_SLAVE_INT_TX             0x10u
#define HARDIIC_SLAVE_INT_STOP           0x08u
#define HARDIIC_SLAVE_INT_ALL            0x78u

typedef struct
{
    uint8 address7;
    uint8 pin_group;
    uint8 accept_all_addresses;
    uint8 initialized;
    uint8 enabled;
} HARDIIC_SlaveState;

HARDIIC_SlaveState hardiic_slave_state;

/* Configure both pins as quasi-bidirectional. */
static uint8 hardiic_slave_configure_pins(uint8 pin_group)
{
    if (pin_group > HARDIIC_SLAVE_P33_P32)
    {
        return HARDIIC_SLAVE_ERROR_PIN;
    }

    switch (pin_group)
    {
        case HARDIIC_SLAVE_P14_P15:
            P1 |= 0x30u;
            P1M1 &= (uint8)~0x30u;
            P1M0 &= (uint8)~0x30u;
            break;

        case HARDIIC_SLAVE_P24_P25:
            P2 |= 0x30u;
            P2M1 &= (uint8)~0x30u;
            P2M0 &= (uint8)~0x30u;
            break;

        case HARDIIC_SLAVE_P76_P77:
            P7 |= 0xc0u;
            P7M1 &= (uint8)~0xc0u;
            P7M0 &= (uint8)~0xc0u;
            break;

        default:
            P3 |= 0x0cu;
            P3M1 &= (uint8)~0x0cu;
            P3M0 &= (uint8)~0x0cu;
            break;
    }

    P_SW2 = (P_SW2 & (uint8)~0x30u) |
            (uint8)(pin_group << 4) | 0x80u;
    hardiic_slave_state.pin_group = pin_group;
    return HARDIIC_SLAVE_OK;
}

/* Prepare the peripheral without enabling it or changing EA. */
uint8 hardiic_slave_init(uint8 address7, uint8 pin_group,
                          uint8 accept_all_addresses)
{
    uint8 result;

    if (address7 > 0x7fu)
    {
        return HARDIIC_SLAVE_ERROR_ARGUMENT;
    }

    P_SW2 |= 0x80u;
    I2CCFG = 0x00u;
    I2CSLCR = 0x00u;
    I2CSLST = 0x00u;

    result = hardiic_slave_configure_pins(pin_group);
    if (result != HARDIIC_SLAVE_OK)
    {
        return result;
    }

    hardiic_slave_state.address7 = address7;
    hardiic_slave_state.accept_all_addresses =
        accept_all_addresses ? 1u : 0u;
    hardiic_slave_state.initialized = 1u;
    hardiic_slave_state.enabled = 0u;
    return HARDIIC_SLAVE_OK;
}

/* Enable slave mode and the selected local interrupt sources. EA is unchanged. */
uint8 hardiic_slave_start(uint8 interrupt_mask, uint8 initial_tx)
{
    if (!hardiic_slave_state.initialized)
    {
        return HARDIIC_SLAVE_ERROR_STATE;
    }

    interrupt_mask &= HARDIIC_SLAVE_INT_ALL;
    I2CCFG = 0x81u;
    I2CSLADR = (uint8)(hardiic_slave_state.address7 << 1) |
               (hardiic_slave_state.accept_all_addresses ? 0x01u : 0x00u);
    I2CSLST = 0x00u;
    I2CTXD = initial_tx;
    I2CSLCR = interrupt_mask;
    hardiic_slave_state.enabled = 1u;
    return HARDIIC_SLAVE_OK;
}

/* Disable the slave. Returns BUSY unless force is nonzero. */
uint8 hardiic_slave_stop(uint8 force)
{
    if (!force && (I2CSLST & HARDIIC_SLAVE_SL_BUSY))
    {
        return HARDIIC_SLAVE_ERROR_BUSY;
    }

    I2CSLCR = 0x00u;
    I2CCFG &= (uint8)~0x80u;
    hardiic_slave_state.enabled = 0u;
    return HARDIIC_SLAVE_OK;
}

/* Change the address only while the bus is idle. */
uint8 hardiic_slave_set_address(uint8 address7,
                                 uint8 accept_all_addresses)
{
    if (address7 > 0x7fu)
    {
        return HARDIIC_SLAVE_ERROR_ARGUMENT;
    }
    if (!hardiic_slave_state.initialized)
    {
        return HARDIIC_SLAVE_ERROR_STATE;
    }
    if (I2CSLST & HARDIIC_SLAVE_SL_BUSY)
    {
        return HARDIIC_SLAVE_ERROR_BUSY;
    }

    hardiic_slave_state.address7 = address7;
    hardiic_slave_state.accept_all_addresses =
        accept_all_addresses ? 1u : 0u;
    if (hardiic_slave_state.enabled)
    {
        I2CSLADR = (uint8)(address7 << 1) |
            (accept_all_addresses ? 0x01u : 0x00u);
    }
    return HARDIIC_SLAVE_OK;
}

/* Change pin mapping only while the bus is idle. */
uint8 hardiic_slave_set_pins(uint8 pin_group)
{
    uint8 was_enabled;
    uint8 result;

    if (!hardiic_slave_state.initialized)
    {
        return HARDIIC_SLAVE_ERROR_STATE;
    }
    if (I2CSLST & HARDIIC_SLAVE_SL_BUSY)
    {
        return HARDIIC_SLAVE_ERROR_BUSY;
    }

    was_enabled = hardiic_slave_state.enabled;
    I2CCFG &= (uint8)~0x80u;
    result = hardiic_slave_configure_pins(pin_group);
    if (was_enabled)
    {
        I2CCFG |= 0x80u;
    }
    return result;
}

#endif /* __HARDIIC_SLAVE_H */


