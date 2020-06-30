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

#include "kernel/drivers/uart.h"
#include "kernel/drivers/sensor.h"
#include <stdio.h>

#define UART_BASE_1 (volatile void *)(0x01D0C000)
#define UART_BASE_2 (volatile void *)(0x01C42000)

volatile uart_reg_info * uart_ports[4] = {
    [SENSOR_PORT_1] = UART_BASE_1,
    [SENSOR_PORT_2] = UART_BASE_2,
    // Port 3 and 4 are different and currently not implemented.
};

uart_register uart_read(sensor_port_id port) {
    // Wait until new data can be recieved
    while ((uart_ports[port]->lsr & 0b01) == 0);
    return uart_ports[port]->rbr;
}

void uart_write(sensor_port_id port, uart_register data) {
    // Wait until new data can be send
    while (!(uart_ports[port]->lsr & (1 << 5)));
    uart_ports[port]->thr = data;
}
