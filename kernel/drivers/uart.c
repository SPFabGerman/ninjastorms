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
#include "kernel/drivers/uart_reg.h"
#include "kernel/drivers/gpio.h"
#include <stdio.h>



// === Adress and Line Definitions ===

// PORT 1
#define UART1_BASE (volatile void *)(0x01D0C000)
// Transmitter Line for Port 1
#define UART1_TXD 147
// Reciever Line for Port 1
#define UART1_RXD 148
// Enable Line for Port 1
#define UART1_ENABLE GPIO_PIN(8, 11)

// PORT 2
#define UART2_BASE (volatile void *)(0x01C42000)
#define UART2_CLOCK 150000000
// Transmitter Line for Port 2
// (Why are these even named this way?)
#define UART0_TXD 145
// Reciever Line for Port 2
#define UART0_RXD 146
// Enable Line for Port 2
#define UART0_ENABLE GPIO_PIN(8, 14)
// Port 2 - Pin 5 - Digital Input / Output 1
#define DIGIB0 GPIO_PIN(0, 14)
// Port 2 - Pin 6 - Digital Input / Output 2
#define DIGIB1 GPIO_PIN(0, 13)

#define NUMPORTS 2
// Port 3 and 4 are different and currently not implemented.

#define UART_FIFO_ENABLE 0b001
#define UART_FIFO_DISABLE 0b000

// TODO: Check where the volatile is really necessary
volatile uart_reg_info * uart_ports[NUMPORTS] = {
    [SENSOR_PORT_1] = UART1_BASE,
    [SENSOR_PORT_2] = UART2_BASE,
    // Port 3 and 4 are different and currently not implemented.
};



// === Functions ===

void uart_set_bitrate(sensor_port_id port, unsigned int bitrate) {
    if (port >= NUMPORTS) return;

    unsigned int clock;
    if (port == SENSOR_PORT_2) {
        clock = UART2_CLOCK;
    } else {
        return;
    }

    unsigned int divisor = clock / (bitrate * 16);

    uart_ports[port]->dll = divisor & 0xff;
    uart_ports[port]->dlh = (divisor >> 8) & 0xff;
}

void uart_flush_fifo(sensor_port_id port, unsigned char buf)
{
    if (port >= NUMPORTS)
        return;

    uart_ports[port]->fcr = UART_FIFO_ENABLE | buf;
}

void uart_2_setup() {
    // Enable Pin Lines
    // FABIANS Code
    gpio_init_pin(UART0_ENABLE);
    // gpio_init_outpin(UART0_ENABLE);
    // Initialize TXD and RXD Lines
    gpio_init_pin(UART0_TXD);
    gpio_init_pin(UART0_RXD);
    // Initialize Digital Input / Output Lines (TODO: needed here?)
    gpio_init_pin(DIGIB0);
    gpio_init_pin(DIGIB1);

    // Initialize UART Registers
    uart_ports[SENSOR_PORT_2]->lcr = 0x03;
    uart_ports[SENSOR_PORT_2]->mdr = 0x00;

    uart_set_bitrate(SENSOR_PORT_2, /* Lowest Bitrate: */ 2400);

    uart_ports[SENSOR_PORT_2]->fcr = UART_FIFO_ENABLE;
    uart_ports[SENSOR_PORT_2]->mcr = 0x00; // 0x03;
    uart_ports[SENSOR_PORT_2]->ier = 0x00; // 0x01;

    // TODO: Can be combined with above?
    gpio_set_low(UART0_ENABLE);

    // Flush FIFO
    // TODO: Can be combined with above?
    uart_flush_fifo(SENSOR_PORT_2, UART_FIFO_RECIEVE_CLEAR | UART_FIFO_TRANSMIT_CLEAR);

    // Set Bit 13 and 14 to 1 to enable power supply
    uart_ports[SENSOR_PORT_2]->pwremu_mgmt = (0b11 << 13);

    // Set Pin 5 and 6 to Float
    // TODO: Needed?
    gpio_set_float(DIGIB0);
    gpio_set_float(DIGIB1);
}


void uart_wait_send_done(sensor_port_id port) {
    if (port >= NUMPORTS) return;
    while ((uart_ports[port]->lsr & (0b11 << 5)) != (0b11 << 5)) ;
}

void uart_wait_send_free(sensor_port_id port) {
    if (port >= NUMPORTS) return;
    while ((uart_ports[port]->lsr & (0b1 << 5)) == 0) ;
}

void uart_wait_recv_free(sensor_port_id port) {
    if (port >= NUMPORTS) return;
    while ((uart_ports[port]->lsr & 0b1) == 0) ;
}


unsigned char uart_read(sensor_port_id port) {
    if (port >= NUMPORTS) return 0;

    // Wait until new data can be read
    uart_wait_recv_free(port);
    return uart_ports[port]->rbr;
}

void uart_write(sensor_port_id port, unsigned char data) {
    if (port >= NUMPORTS) return;

    // Wait until new data can be written
    uart_wait_send_free(port);
    uart_ports[port]->thr = data;
}

void uart_write_block(sensor_port_id port, unsigned char data) {
    if (port >= NUMPORTS) return;

    uart_write(port, data);
    // Wait until data has been send
    uart_wait_send_done(port);
}



unsigned int uart_read_pin6(sensor_port_id port) {
    if (port == SENSOR_PORT_2)
        return gpio_read(DIGIB1);

    return -1;
}

void uart_reg_print(sensor_port_id port)
{
    if (port >= NUMPORTS)
        return;

    printf("Port %i:\t", port);
    uart_register *arr = (uart_register *)uart_ports[port];
    int i;
    for (i = 0; i <= 13; i++)
    {
        printf("%i 0x%x\t", i, arr[i]);
    }
    printf("\n");
}
