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

#define UART_FIFO_RECIEVE_CLEAR 0b010
#define UART_FIFO_TRANSMIT_CLEAR 0b100

void uart_set_bitrate(sensor_port_id port, unsigned int bitrate);
void uart_flush_fifo(sensor_port_id port, unsigned char buf);

// TODO: Funktion aufspalten
void uart_2_setup();

// Wait until all data has been send
void uart_wait_send_done(sensor_port_id port);
// Wait until new data can be inserted into the send buffer
void uart_wait_send_free(sensor_port_id port);
// Wait until new data can be read from the recieve buffer
void uart_wait_recv_free(sensor_port_id port);

// Read data from the UART Sensor port.
unsigned char uart_read(sensor_port_id port);
// Write data to the UART Sensor port.
void uart_write(sensor_port_id port, unsigned char data);
// Write data to the UART Sensor and wait until it is send.
void uart_write_block(sensor_port_id port, unsigned char data);

unsigned int uart_read_pin6(sensor_port_id port);

void uart_reg_print(sensor_port_id port);
