# A7105 UART Bridge

This project aims at providing a UART interface to a A7105 2.4 GHz wireless
tranceiver. An Atmel Attiny85 with a soft UART and soft SPI will be used
for low level communication and some basic abstraction. The current use
case is programming another chip over the air, so error-free bulk transfers
are the top priority.

## Status
No software running, not even the hardware is built.

## Attributions
For the soft UART, [**avr-softuart** by Brian Lalor
](https://github.com/blalor/avr-softuart) will be used. It is based on the
**Timer-based AVR Software-UART** by Martin Thomas, which in turn is based
on the **generic software uart** by Colin Gittins.

## Licenses
The submodule **avr-softuart** is licensed under a BSD-style license. All
other modules are licensed under the GPL (see individual LICENCE files).
