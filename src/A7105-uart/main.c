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

#include "common.h"
#include "softuart.h"
#include "A7105_SPI.h"
#include "A7105.h"

void init(void)
{
    softuart_init();
    SPI_init();
    A7105_init();
    sei();
}

int main(void)
{
    init();

    softuart_puts("Hello, World!\n\r");
    softuart_puts("Testing connection to A7105... ");
    if(A7105_test())
    {
        softuart_puts("Success! :)\n\r");
    }
    else
    {
        softuart_puts("Failure! :(\n\r");
    }
    while(1)
    {
    }
}

