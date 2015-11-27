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

#ifndef A7105_H
#define A7015_H

#include "common.h"

enum A7105_reg {
    A7105_reg_mode = 0x00,
    A7105_reg_mode_control,   //0x01
    A7105_reg_calib,          //0x02
    A7105_reg_FIFOI,          //0x03
    A7105_reg_FIFOII,         //0x04
    A7105_reg_FIFO_data,      //0x05
    A7105_reg_ID,             //0x06
    A7105_reg_OSCI,           //0x07
    A7105_reg_OSCII,          //0x08
    A7105_reg_OSCIII,         //0x09
    A7105_reg_CKO,            //0x0a
    A7105_reg_GPIOI,          //0x0b
    A7105_reg_GPIOII,         //0x0c
    A7105_reg_clock,          //0x0d
    A7105_reg_data_rate,      //0x0e
    A7105_reg_channel,        //0x0f
    A7105_reg_PLLII,          //0x10
    A7105_reg_PLLIII,         //0x11
    A7105_reg_PLLIV,          //0x12
    A7105_reg_PLLV,           //0x13
    A7105_reg_TXI,            //0x14
    A7105_reg_TXII,           //0x15
    A7105_reg_delayI,         //0x16
    A7105_reg_delayII,        //0x17
    A7105_reg_RX,             //0x18
    A7105_reg_RX_gainI,       //0x19
    A7105_reg_RX_gainII,      //0x1a
    A7105_reg_RX_gainIII,     //0x1b
    A7105_reg_RX_gainIV,      //0x1c
    A7105_reg_RSSI_thres,     //0x1d
    A7105_reg_ADC,            //0x1e
    A7105_reg_codeI,          //0x1f
    A7105_reg_codeII,         //0x20
    A7105_reg_codeIII,        //0x21
    A7105_reg_IF_calibI,      //0x22
    A7105_reg_IF_calibII,     //0x23
    A7105_reg_VCO_c_calib,    //0x24
    A7105_reg_VCO_sb_calibI,  //0x25
    A7105_reg_VCO_sb_calibII, //0x26
    A7105_reg_battery,        //0x27
    A7105_reg_TX_test,        //0x28
    A7105_reg_RX_testI,       //0x29
    A7105_reg_RX_testII,      //0x2a
    A7105_reg_CPC,            //0x2b
    A7105_reg_crystal_test,   //0x2c
    A7105_reg_PLL_test,       //0x2d
    A7105_reg_VCO_testI,      //0x2e
    A7105_reg_VCO_testII,     //0x2f
    A7105_reg_IFAT,           //0x30
    A7105_reg_R_scale,        //0x31
    A7105_reg_filter_test     //0x32
};

enum A7105_strobe {
    A7105_strobe_sleep       = 0x80,
    A7105_strobe_idle        = 0x90,
    A7105_strobe_standby     = 0xa0,
    A7105_strobe_PLL         = 0xb0,
    A7105_strobe_RX          = 0xc0,
    A7105_strobe_TX          = 0xd0,
    A7105_strobe_write_reset = 0xe0,
    A7105_strobe_read_reset  = 0xf0
};

void A7105_reset(void);

/* calib_all: Perform 3 calibrations as in chapter 15 of datasheet.
 *            Should be performed when everything is set up (to the point
 *            that a channel is selected).
 * Note: Timer 1 is used during calibration. Make sure it's not used when
 *       this function is run and reconfigure it afterwards.
 * Returns:
 *  0       on success
 *  The ored combination of the following values:
 *    0x01  if VCO bank calibration took more than 1000us
 *    0x02  if VCO bank calibration was not successful
 *    0x04  if VCO current calibration took more than 1000us
 *    0x08  if VCO current calibration was not successful
 *    0x10  if IF filter bank calibration took more than 1000us
 *    0x20  if IF filter bank calibration was not successful
 */
uint8_t calib_all(void);

void A7105_init(void);

/* ID read and write functions:
 * Read or write the 32 bit ID. The most significant bit is read/written
 * first. With the recommended values for byte 0 the ID pattern should
 * therefore be 0x5******* or 0xA*******.
 */
uint32_t A7105_ID_read(void);
void A7105_ID_write(uint32_t ID);

#endif

