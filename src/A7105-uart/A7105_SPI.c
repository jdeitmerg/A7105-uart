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

#include "A7105_SPI.h"

void SPI_init(void)
{
    //CS and CLK as outputs
    setbit(SPI_CS_DDR, SPI_CS_DDP);
    setbit(SPI_CLK_DDR, SPI_CS_DDP);

    //CS high by default
    SPI_CDESELECT();
    //CLK low by default
    clearbit(SPI_CLK_PORT, SPI_CLK_PP);
}

