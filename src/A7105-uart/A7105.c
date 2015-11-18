/*
 *  This file is part of A7105-uart, a UART interface to the A7105 wireless
 *  tranceiver.
 *  Copyright (C) 2015 J.Deitmerg <github.com/mowfask>
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
    SPI_reg_write(0x00, 0x00);
    _delay_us(100); //probably a waste of time
}

void A7105_init(void)
{
    A7105_reset();
}

uint8_t A7105_test(void)
{
    A7105_reset();
    // The clock register (0x0d) should read 0xf5 after reset
    return(SPI_reg_read(0x0d) == 0xf5);
}

