/******************************************************************************
 *       ninjastorms - shuriken operating system                              *
 *                                                                            *
 *    Copyright (C) 2013 - 2016  Andreas Grapentin et al.                     *
 *                                                                            *
 *    This program is free software: you can redistribute it and/or modify    *
 *    it under the terms of the GNU General Public License as published by    *
 *    the Free Software Foundation, either version 3 of the License, or       *
 *    (at your option) any later version.                                     *
 *                                                                            *
 *    This program is distributed in the hope that it will be useful,         *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *    GNU General Public License for more details.                            *
 *                                                                            *
 *    You should have received a copy of the GNU General Public License       *
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
******************************************************************************/

#include "ports.h"

#define UARTSENSOR_MODE_DEFAULT 0

#define EV3_COLOR_ID 0x1d
#define EV3_COLOR_DUMP 307

enum ev3_color_mode {
    COL_REFLECT,
    COL_AMBIENT,
    COL_COLOR,
    // Debug Modes:
    REF_RAW,
    RGB_RAW,
    COL_CAL
};

enum col_color_return {
    COL_COLOR_NOTHING,
    COL_COLOR_BLACK,
    COL_COLOR_BLUE,
    COL_COLOR_GREEN,
    COL_COLOR_YELLOW,
    COL_COLOR_RED,
    COL_COLOR_WHITE,
    COL_COLOR_BROWN
};


void uartsensor_setup(sensor_port_id port);

// Waits for the next beginn of an input sequence.
// Aligns the Recieve FIFO so that the next read starts from the first byte
// after the three Init Bytes, that specify the Device ID.
void uartsensor_wait_init(sensor_port_id port, unsigned char deviceid);
// Wait for the next full data pack from the sensor.
// Aligns the FIFO so that the next read starts at the next data pack.
void uartsensor_wait_data(sensor_port_id port, unsigned char mode);

// Reads num bytes and throw them away.
void uartsensor_dump_bytes(sensor_port_id port, unsigned int num);

// Sends an ACK Byte to the sensor.
// Must be done, at the end of the initialization,
// after the sensor itself sends an ACK.
void uartsensor_send_ack(sensor_port_id port);
// Reads the next byte. If it was an ACK, it responds with an ACK and returns 1.
// Otherwise it sends nothing and returns 0.
int uartsensor_respond_ack(sensor_port_id port);
// Sends a NACK Byte to the sensor.
// Needs to be done at least every 300ms.
// Causes sensor to resend data.
void uartsensor_send_nack(sensor_port_id port);

void uartsensor_set_low_bitrate(sensor_port_id port);
void uartsensor_set_middle_bitrate(sensor_port_id port);
void uartsensor_set_high_bitrate(sensor_port_id port);

// Changes the mode of the sensor.
// `mode` needs to be supported by the specific sensor.
void uartsensor_change_mode(sensor_port_id port, unsigned char mode);

// Reads one 1Byte long data pack and returns the read data byte.
// The function assumes, the FIFO is alligned correctly.
// As long as there is no corrupt data in the FIFO,
// the FIFO is gurannteed the be alligned correctly after the function call.
unsigned char uartsensor_read_data(sensor_port_id port);


// Helper Setup Function for the EV3 Color Sensor.
// Needs to be called before data is first read and probably every time after
// a long time out, since the sensor probably reset itself
int uartsensor_setup_color(sensor_port_id port);


void uartsensor_hexdump(sensor_port_id port);
