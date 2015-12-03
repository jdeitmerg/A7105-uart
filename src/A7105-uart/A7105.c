/*
 *  This file is part of A7105-uart, a UART interface to the A7105 wireless
 *  tranceiver.
 *  Copyright (C) 2015 J.Deitmerg <mowfask@gmail.com>
 *
 *  A7105-uart is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  A7105-uart is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with A7105-uart.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "A7105.h"
#include "A7105_SPI.h"

void A7105_reset(void)
{
    SPI_reg_write(A7105_reg_mode, 0x00);
}

static uint8_t calib_VCO_bank(void)
{
    // Prescale the 8MHz clock by 64, so we get 8us ticks
    TCCR1 = (1 << CS10) | (1 << CS11) | (1 << CS12);

    // Assume MVBS == 0 after reset
    SPI_single_write(A7105_strobe_PLL);
    // We'll leave VTH and VTL at their recommended values.

    // Set VBC bit to start calibration
    SPI_reg_write(A7105_reg_calib, 0x02);

    TCNT1 = 0;
    // Poll VBC bit
    while(1)
    {
        if(TCNT1 > 125)
        {
            // VCO current calib not done after 1000us
            // Disable timer
            TCCR1 = 0x00;
            return(0x01);
        }
        if(!testbit(SPI_reg_read(A7105_reg_calib), 1))
        {
            // VCO bank calib done
            break;
        }
    }
    // Disable timer
    TCCR1 = 0x00;
    // VBCF bit: Indicates calib failure
    if(testbit(SPI_reg_read(A7105_reg_VCO_sb_calibI), 3))
    {
        // VCO bank calib not successful
        return(0x02);
    }

    return(0);
}

static uint8_t calib_VCO_current(void)
{
    // Prescale the 8MHz clock by 64, so we get 8us ticks
    TCCR1 = (1 << CS10) | (1 << CS11) | (1 << CS12);

    // Assume MVCS == 0 after reset
    SPI_single_write(A7105_strobe_PLL);
    // Set VCC bit to start calibration
    SPI_reg_write(A7105_reg_calib, 0x04);

    TCNT1 = 0;
    // Poll VCC bit
    while(1)
    {
        if(TCNT1 > 125)
        {
            // VCO current calib not done after 1000us
            // Disable timer
            TCCR1 = 0x00;
            return(0x04);
        }
        if(!testbit(SPI_reg_read(A7105_reg_calib), 2))
        {
            // VCO current calib done
            break;
        }
    }
    // Disable timer
    TCCR1 = 0x00;
    // FVCC bit: Indicates calib failure
    if(testbit(SPI_reg_read(A7105_reg_VCO_c_calib), 4))
    {
        // VCO current calib not successful
        return(0x08);
    }

    return(0);
}

static uint8_t calib_filter_bank(void)
{
    // Prescale the 8MHz clock by 64, so we get 8us ticks
    TCCR1 = (1 << CS10) | (1 << CS11) | (1 << CS12);

    // Assume MFBS == 0 after reset
    SPI_single_write(A7105_strobe_PLL);
    // Set FBC bit to start calibration
    SPI_reg_write(A7105_reg_calib, 0x01);

    TCNT1 = 0;
    // Poll FBC bit
    while(1)
    {
        if(TCNT1 > 125)
        {
            // IF calib not done after 1000us
            // Disable timer
            TCCR1 = 0x00;
            return(0x10);
        }
        if(!testbit(SPI_reg_read(A7105_reg_calib), 0))
        {
            // IF calib done
            break;
        }
    }
    // Disable timer
    TCCR1 = 0x00;
    // FBCF bit: Indicates calib failure
    if(testbit(SPI_reg_read(A7105_reg_IF_calibI), 4))
    {
        // IF calib not successful
        return(0x20);
    }

    return(0);
}

uint8_t calib_all(void)
{
    uint8_t retval;

    retval = calib_VCO_bank();
    retval |= calib_VCO_current();
    retval |= calib_filter_bank();

    SPI_single_write(A7105_strobe_standby);

    return(retval);
}

void A7105_init(void)
{
    A7105_reset();
    // After reset 16MHz crystal and 500Kbps data rate are already
    // configured. The default base frequency is then 2400.001454 MHz.
    // A channel width of 500KHz is recommended. 16MHz/(4*500KHz)-1 = 7,
    // leave all the other bits as they are.
    SPI_reg_write(A7105_reg_PLLII, 0xf0 | (7 << 1));
    // Note: To stay in the 2.4 to 2.483 GHz ISM band, only channels 1
    // through 166 should be used (with "Auto IF Exchange" enabled, master
    // sends on 1-166, slave sends on 0-165).

    /* Current understanding of the datasheet (concerning Auto IF):
     * Master sends on channel n, sets ULS=0 to receive on channel n-1.
     * Slave then has to send on channel n-1, but sets ULS=1 to receive on
     * channel n-1+1=n. Besides the bad english, the datasheet is also
     * not clear about what to do if the channel width is not 500KHz, as
     * ULS always shifts by 500KHz.
     */

    // Demodulator DC estimation average mode: 0x1 is recommended.
    // Preamble pattern detection length: 0x2 recommended for 500Kbps
    SPI_reg_write(A7105_reg_codeII, 0x4 | (0x1 << 4) | 0x2);
    // Demodulator DC estimation mode: Average and hold (0x2)
    SPI_reg_write(A7105_reg_RX_testI, 0x07 | (0x2 << 5));
}

uint32_t A7105_ID_read(void)
{
    uint32_t ID;
    uint8_t idbytes[4];

    // We could read this straight into ID, but the result would depend on
    // the endiness of the architecture.
    SPI_reg_multi_read(A7105_reg_ID, idbytes, 4);

    ID  = (uint32_t) idbytes[0] << 24;
    ID |= (uint32_t) idbytes[1] << 16;
    ID |= (uint32_t) idbytes[2] << 8;
    ID |= (uint32_t) idbytes[3] << 0;

    return(ID);
}

void A7105_ID_write(uint32_t ID)
{
    uint8_t idbytes[4];

    idbytes[0] = ID >> 24;
    idbytes[1] = ID >> 16;
    idbytes[2] = ID >> 8;
    idbytes[3] = ID >> 0;

    SPI_reg_multi_write(A7105_reg_ID, idbytes, 4);
}

