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

// This file provides functions to interact with various EV3 Sensors.

#include <stdio.h>

#include "kernel/drivers/uart_sensor.h"
#include "kernel/drivers/uart.h"

void uartsensor_setup(sensor_port_id port) {
    if (port == SENSOR_PORT_2) {
        uart_2_setup();
    }
}


void uartsensor_wait_init(sensor_port_id port, unsigned char deviceid) {
    unsigned char checksum = 0xff ^ 0x40 ^ deviceid;

    // Flush FIFO to free it from potentionally corrupt data.
    uart_flush_fifo(port, UART_FIFO_RECIEVE_CLEAR);

    unsigned char c = uart_read(port); // Read first byte.
    while (1) {
        // Check last read byte (from prev loop)
        while (c != 0x40) {
            c = uart_read(port);
        }

        c = uart_read(port);
        if (c != deviceid) {
            // Go Back to the start of the loop,
            // if the byte is not the expected one.
            continue;
        }

        // Read Checksum Byte
        c = uart_read(port);
        if (c != checksum) {
            continue;
        }

        // All the Init Bytes were read => End the function
        return;
    }
}

void uartsensor_wait_data(sensor_port_id port, unsigned char mode) {
    unsigned char initByte = 0b11000000 | mode;

    // Flush FIFO to free it from potentionally corrupt data.
    uart_flush_fifo(port, UART_FIFO_RECIEVE_CLEAR);

    volatile unsigned char c = uart_read(port); // Read first byte.
    while (c != initByte)
    {
        c = uart_read(port);
    }

    c = uart_read(port); // Dump read the next byte

    // Read Checksum Byte
    // Dump read to, since some sensors (e.g. Color Sensor)
    // sometimes write wrong checksum values
    c = uart_read(port);
    
}

void uartsensor_dump_bytes(sensor_port_id port, unsigned int num) {
    int i;
    for (i = 0; i < num; i++) {
        // Use volatile to ensure that the compiler
        // doesn't optimize the function call out.
        volatile char c = uart_read(port);
    }
}


void uartsensor_send_ack(sensor_port_id port) {
    uart_write_block(port, 0x04);
}

int uartsensor_respond_ack(sensor_port_id port) {
    unsigned char c = uart_read(port);
    if (c == 0x04) {
        uartsensor_send_ack(port);
        return 1;
    }

    return 0;
}

void uartsensor_send_nack(sensor_port_id port) {
    uart_write(port, 0x02);
}


void uartsensor_set_low_bitrate(sensor_port_id port) {
    uart_set_bitrate(port, 2400);
}

void uartsensor_set_middle_bitrate(sensor_port_id port) {
    uart_set_bitrate(port, 57600);
}

void uartsensor_set_high_bitrate(sensor_port_id port) {
    uart_set_bitrate(port, 460800);
}


void uartsensor_change_mode(sensor_port_id port, unsigned char mode) {
    unsigned char checksum = 0xff ^ 0b01000011 ^ mode;
    uart_write(port, /* CMD: Select Mode */ 0b01000011);
    uart_write(port, mode);
    uart_write(port, checksum); // Check Byte
}

unsigned char uartsensor_read_data(sensor_port_id port) {
    volatile char c = uart_read(port); // Data Pack Init
    volatile char b = uart_read(port); // Actuall 1 Byte long data
    c = uart_read(port); // Checksum
    
    return b;
}



void uartsensor_hexdump(sensor_port_id port) {
    while (1) {
        uartsensor_send_nack(port);

        int c = uart_read(port);
        printf("%x ", c);
    }
}
