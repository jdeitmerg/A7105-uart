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

#include "common.h"
#include "softuart.h"
#include "A7105_SPI.h"
#include "A7105.h"

// Similar to glibc example
int uart_putchar(char c, FILE* stream)
{
    if (c == '\n')
    {
        softuart_putchar('\r');
    }
    softuart_putchar(c);
    return 0;
}

int uart_getchar(FILE *stream) {
    return(softuart_getchar());
}

static FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar,
                                             _FDEV_SETUP_RW);

void init(void)
{
    softuart_init();
    // for printf and friends
    stdout = &uart_stream;
    stdin = &uart_stream;
    sei();

    SPI_init();
    A7105_init();
}

int main(void)
{
    init();

    A7105_set_channel(20);
    A7105_set_mode(slave);

    printf("\n\nA7105 initialized successfully!\n");

    printf("\nCalibration result: 0x%02hhx\n", A7105_calib());

    while(1)
    {
    }
}

