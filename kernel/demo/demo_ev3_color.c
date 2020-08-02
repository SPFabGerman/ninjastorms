
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

#include "demo_ev3_color.h"

#include "kernel/drivers/uart_sensor.h"

#include <stdio.h>

void demo_ev3_color(void) {
  // Setup UART Port
  uartsensor_setup(SENSOR_PORT_2);

  // Wait for Start of Initialization Protocoll
  // TODO: Wait instead for break condition (Pin6 is Low for, x Sec.)
  uartsensor_wait_init(SENSOR_PORT_2, EV3_COLOR_ID);

  // Read the rest of the init sequence
  uartsensor_dump_bytes(SENSOR_PORT_2, EV3_COLOR_DUMP);

  // Check if we reached actually the end
  if (!uartsensor_respond_ack(SENSOR_PORT_2)) {
      printf("No ACK Found.\n");
      return;
  }

  // TODO: No SYNC Byte Handling here? Why?

  // Change Bitrate
  uartsensor_set_middle_bitrate(SENSOR_PORT_2);

  uartsensor_wait_data(SENSOR_PORT_2, UARTSENSOR_MODE_DEFAULT);

  // Select Mode 2 (COL-COLOR)
  uartsensor_change_mode(SENSOR_PORT_2, COL_COLOR);

  while(1) {
    unsigned char d = uartsensor_read_data(SENSOR_PORT_2);
    switch (d) {
    case COL_COLOR_NOTHING:
      printf("No Color\n");
      break;
    case COL_COLOR_BLACK:
      printf("BLACK\n");
      break;
    case COL_COLOR_BLUE:
      printf("BLUE\n");
      break;
    case COL_COLOR_GREEN:
      printf("GREEN\n");
      break;
    case COL_COLOR_YELLOW:
      printf("YELLOW\n");
      break;
    case COL_COLOR_RED:
      printf("RED\n");
      break;
    case COL_COLOR_WHITE:
      printf("WHITE\n");
      break;
    case COL_COLOR_BROWN:
      printf("BROWN\n");
      break;
    default:
      printf("???\n");
      break;
    }

    uartsensor_send_nack(SENSOR_PORT_2);
  }
}
