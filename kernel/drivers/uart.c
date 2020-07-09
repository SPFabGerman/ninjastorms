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

#define UART2_CLOCK 150000000

// Transmitter Line for Port 1
#define UART1_TXD 147
// Reciever Line for Port 2
#define UART1_RXD 148

#define UART1_ENABLE GPIO_PIN(8, 11)

// Transmitter Line for Port 2
// (Why are these even named this way?)
#define UART0_TXD 145
#define UART0_TXD_v2 GPIO_PIN(8, 3)
// Reciever Line for Port 2
#define UART0_RXD 146
#define UART0_RXD_v2 GPIO_PIN(8, 4)

#define UART0_ENABLE GPIO_PIN(8, 14)
// Port 2 - Pin 5 - Digital Input / Output 1
#define DIGIB0 GPIO_PIN(0, 14)
// Port 2 - Pin 6 - Digital Input / Output 2
#define DIGIB1 GPIO_PIN(0, 13)

volatile uart_reg_info * uart_ports[4] = {
    [SENSOR_PORT_1] = UART_BASE_1,
    [SENSOR_PORT_2] = UART_BASE_2,
    // Port 3 and 4 are different and currently not implemented.
};

void uart_2_setup()
{
    // Enable Pin Lines
    gpio_init_pin(UART0_ENABLE);
    // Initialize TXD and RXD Lines
    gpio_init_pin(UART0_TXD);
    gpio_init_pin(UART0_RXD);
    // Initialize Digital Input / Output Lines (TODO: needed here?)
    gpio_init_pin(DIGIB0);
    gpio_init_pin(DIGIB1); 

    // gpio_init_pin(UART0_TXD_v2);
    // gpio_init_pin(UART0_RXD_v2);

    // Break Port 1
    // gpio_init_pin_false(UART1_TXD);
    // gpio_init_pin_false(UART1_RXD);
    // gpio_init_inpin(UART1_TXD);
    // gpio_init_outpin(UART1_RXD);
    // printf("Port 1 broken.");

    // Calculate Bitrate
    // int bitrate = 115200;
    unsigned int bitrate = 2400;
    unsigned int divisor = UART2_CLOCK / (bitrate * 16);

    // Initialize UART Registers
    uart_ports[SENSOR_PORT_2]->lcr = 0x03;
    uart_ports[SENSOR_PORT_2]->mdr = 0x00;
    
    uart_ports[SENSOR_PORT_2]->dll = divisor & 0xff;
    uart_ports[SENSOR_PORT_2]->dlh = (divisor >> 8) & 0xff;

    uart_ports[SENSOR_PORT_2]->fcr = 0x01;
    uart_ports[SENSOR_PORT_2]->mcr = 0x00;
    // uart_ports[SENSOR_PORT_2]->mcr = 0x03;
    // uart_ports[SENSOR_PORT_2]->ier = 0x01;
    uart_ports[SENSOR_PORT_2]->ier = 0x00;

    // Clear Pin Data and reset FIFO
    gpio_init_pin(UART0_TXD);
    gpio_set_low(UART0_ENABLE);
    // gpio_init_outpin(UART0_ENABLE);
    // gpio_init_outpin(UART0_TXD);
    // gpio_init_outpin(UART0_TXD_v2);
    uart_ports[SENSOR_PORT_2]->fcr = 0x07;

    // Disble Line
    gpio_set_high(UART0_ENABLE);
    // Set Pin 5 and 6 to Float
    gpio_set_float(DIGIB0);
    gpio_set_float(DIGIB1);

    // Clear Pin Data and reset FIFO
    gpio_init_pin(UART0_TXD);
    gpio_set_low(UART0_ENABLE);
    uart_ports[SENSOR_PORT_2]->fcr = 0x07;

    printf("Setup done.\n");
}

void uart_reg_printf(sensor_port_id port) {
    printf("Port: %i\n", port);
    uint32_t * arr = (uint32_t *) uart_ports[port];
    int i;
    for (i = 0; i <= 13; i++) {
        printf("%i: 0x%x ", i, arr[i]);
    }
    printf("\n");
}

uart_register uart_read(sensor_port_id port)
{
    // Wait until new data can be recieved
    while ((uart_ports[port]->lsr & 0b01) == 0);
    return uart_ports[port]->rbr;
}

void uart_write(sensor_port_id port, uart_register data) {
    // Wait until new data can be send
    while (!(uart_ports[port]->lsr & (1 << 5)));
    uart_ports[port]->thr = data;
}
