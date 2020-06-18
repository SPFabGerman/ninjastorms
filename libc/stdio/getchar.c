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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>

#if BOARD_EV3
// UART - Recieve Buffer Register
#define UART_RBR (volatile char *)(0x01D0C000)
// UART - Interrupt Enable Register
#define UART_IER (volatile char *)(0x01D0C004)
// UART - Interrupt Identification Register
#define UART_IIR (volatile char *)(0x01D0C008)
#define UART_LSR (volatile char *)(0x01D0C014)
#define UART_MSR (volatile char *)(0x01D0C018)
#endif

/**
 * This Function waits for and returns the next character that
 * is typed by the user on the Serial Console (Port 1).
 * It uses the internal FIFO of the UART Bus (if it is enabled, which by default it is)
 * to buffer up to 32 (?) characters. If more characters are typed, the behaviour is undefined.
 * 
 * If an error occured it returns -1.
 */
int getchar()
{
#if BOARD_VERSATILEPB
    printf("getchar: Unimplemented!\n");
    return -1;
#endif

    // Wait until new data can be recieved
    while ((*UART_LSR & 0b01) == 0);

    int c = *UART_RBR;
    return c;
}
