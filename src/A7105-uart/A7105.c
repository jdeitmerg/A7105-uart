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

int8_t A7105_init(void)
{
    A7105_reset();
    // After reset 16MHz crystal and 500Kbps data rate are already
    // configured. The default base frequency is then 2400.001454 MHz.
    // A channel width of 500KHz is recommended. 16MHz/(4*500KHz)-1 = 7,
    // leave all the other bits as they are.
    SPI_reg_write(A7105_reg_PLLII, 0xf0 | (7 << 1));
    // Note: To stay in the 2.4 to 2.5 GHz ISM band, only channels 0
    // through 98 should be used (with "Auto IF Exchange" enabled).

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

    // For measuring time:
    // Prescale the 8MHz clock by 64, so we get 8us ticks
    TCCR1 = (1 << CS10) | (1 << CS11) | (1 << CS12);

    /* IF filter bank calibration
     */
    // Assume MFBS == 0 after reset
    SPI_single_write(A7105_strobe_PLL);
    // Set FBC bit
    SPI_reg_write(A7105_reg_calib, 0x01);

    TCNT1 = 0;
    // Poll FBC bit
    while(1)
    {
        if(TCNT1 > 125)
        {
            // IF calib not done after 1000us
            return(-0x10);
        }
        if(!testbit(SPI_reg_read(A7105_reg_calib), 0))
        {
            // IF calib done
            break;
        }
    }
    if(testbit(SPI_reg_read(A7105_reg_IF_calibI), 4))
    {
        // IF calib not successful
        return(-0x11);
    }

    return(0);
}

uint8_t A7105_test(void)
{
    A7105_reset();
    // The clock register should read 0xf5 after reset
    return(SPI_reg_read(A7105_reg_clock) == 0xf5);
}

