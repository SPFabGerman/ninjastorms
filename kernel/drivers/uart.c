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
#include "kernel/drivers/gpio.h"
#include <stdio.h>

#define UART_BASE_1 (volatile void *)(0x01D0C000)
#define UART_BASE_2 (volatile void *)(0x01C42000)

// Transmitter Line for Port 1
#define UART1_TXD 147
// Reciever Line for Port 2
#define UART1_RXD 148

// Transmitter Line for Port 2
// (Why are these even named this way?)
#define UART0_TXD 145
// Reciever Line for Port 2
#define UART0_RXD 146

volatile uart_reg_info * uart_ports[4] = {
    [SENSOR_PORT_1] = UART_BASE_1,
    [SENSOR_PORT_2] = UART_BASE_2,
    // Port 3 and 4 are different and currently not implemented.
};

void uart_2_setup()
{
    // Initialize TXD and RXD Registers
    gpio_init_pin(UART0_TXD);
    gpio_init_pin(UART0_RXD);

    // Calculate Bitrate
    int bitrate = 115200;
    // int bitrate = 2400;
    int divisor = /* Clock: */ 150000000 / (bitrate * 16);

    // Initialize UART Registers
    uart_ports[SENSOR_PORT_2]->lcr = 0x03;
    uart_ports[SENSOR_PORT_2]->mdr = 0x00;
    
    uart_ports[SENSOR_PORT_2]->dll = divisor & 0xff;
    uart_ports[SENSOR_PORT_2]->dlh = (divisor >> 8) & 0xff;

    uart_ports[SENSOR_PORT_2]->fcr = 0x01;
    uart_ports[SENSOR_PORT_2]->mcr = 0x00;
    uart_ports[SENSOR_PORT_2]->ier = 0x01;

    // Clear Pin Data and reset FIFO
    gpio_init_outpin(UART0_TXD);
    uart_ports[SENSOR_PORT_2]->fcr = 0x07;

    printf("Setup done.\n");
}

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
