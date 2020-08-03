
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
  if (!uartsensor_setup_color(SENSOR_PORT_2)) {
    printf("Error in Setup.\n");
    return;
  }

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
