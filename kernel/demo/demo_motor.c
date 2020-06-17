
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

#include "demo_motor.h"

#include "kernel/drivers/button.h"
#include "kernel/drivers/motor.h"

void
demo_motor (void)
{
  printf("Starting Motor Demo Task for Motor on Port A.\n");
  while(1) {
    if(button_get_state(BUTTON_CENTER))
    {
      motor_set_state(MOTOR_PORT_A, MOTOR_OFF);
    }
    else if (button_get_state(BUTTON_LEFT)) {
      motor_set_state(MOTOR_PORT_A, MOTOR_FORWARD);
    }
    else if (button_get_state(BUTTON_RIGHT))
    {
      motor_set_state(MOTOR_PORT_A, MOTOR_BACKWARD);
    }
  }
}
