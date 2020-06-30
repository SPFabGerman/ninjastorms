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

#include "kernel/drivers/sensor.h"

// Type for one UART Register.
// Needs to be an specific byte length.
// (4 Byte, so one int, for the EV3)
typedef int uart_register;

// A Struct that can be used, to acces all the registers of the uart devices.
typedef struct {
    union {
        uart_register base_adress;
        // Recieve Buffer Register (R)
        uart_register rbr;
        // Transmiter Holding Register (W)
        uart_register thr;
    };

    union {
        uart_register base_p_1;
        // Interrupt Enable Register (R/W)
        uart_register ier;
    };

    union {
        uart_register base_p_2;
        // Interrupt Identification Register (R)
        uart_register iir;
        // FIFO controll Regisert (W)
        uart_register fcr;
    };

    union {
        uart_register base_p_3;
        // Line Controll Register (R/W)
        uart_register lcr;
    };

    union {
        uart_register base_p_4;
        // Modem Controll Register (R/W)
        uart_register mcr;
    };

    union {
        uart_register base_p_5;
        // Line Status Register (R)
        uart_register lsr;
    };

    union {
        uart_register base_p_6;
        // Modem Status Register (R)
        uart_register msr;
    };

    union {
        uart_register base_p_7;
        // Unused
        uart_register scr;
    };

    union {
        uart_register base_p_8;
        // Irgendwas mit der Clock
        uart_register dll;
    };

    union {
        uart_register base_p_9;
        // Irgendwas mit der Clock
        uart_register dlh;
    };

    uart_register base_p_10;
    uart_register base_p_11;
    uart_register base_p_12;

    union {
        uart_register base_p_13;
        // ?
        uart_register mdr;
    };
} uart_reg_info;

extern volatile uart_reg_info *uart_ports[];

// Read data from the UART Sensor port.
uart_register uart_read(sensor_port_id port);

// Write data to the UART Sensor port.
void uart_write(sensor_port_id port, uart_register data);
