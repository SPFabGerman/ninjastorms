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
    
    // Enable Recieve Interrupt
    *UART_IER = *UART_IER | 0b01;

    // Loop until we get a Recieve Interrupt
    while (1) {

        int interrupt, has_new_interrupt;
        // Wait for next interrupt
        do {
            // Get State of Interrupt Identification Register
            interrupt = *UART_IIR;
            has_new_interrupt = interrupt & 0b0001;
        } while (has_new_interrupt == 1);

        // Get the type of the new interrupt
        int interrupt_type = (interrupt >> 1) & (0b111);
        switch (interrupt_type) {
            int dummy;
            int c;

        case 0b000:
            // Modem Status Change?
            // Make a dummy read to reset interrupt register.
            dummy = *UART_MSR;
            break;

        case 0b011:
            // Line Status Register has changed.
            // Again, we don't really care about that
            // and just use a dummy read to reset interrupt register.
            dummy = *UART_LSR;
            break;

        case 0b001:
            // Transmitter holding register is empty
            // All Data has been send and new data can be send.
            // Implementation not needed here.
            // Will be reset on the next IIR read.
            break;

        case 0b110:
            // Character Timeout:
            // ???
            // Is also reset by a RBR read.
        case 0b010:
            // New Data can be recieved! Yay.
            // Read RBR to reset interrupt state and return.
            c = *UART_RBR;
            return c;
        
        default:
            // Undefined behaviour?
            return -1;
        } /* switch (interrupt_type) */

    } /* while (1) */

    // Unreachable
    return -1;
}
