
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

#include "button.h"

#include "kernel/drivers/gpio.h"

#include <stdio.h>

/* this struct holds the information on the Button pins
 *
 * each Button has only a single input pin
 */
struct button_info
{
  unsigned int pin;
};
typedef struct button_info button_info;

button_info buttons[] =
{
  [BUTTON_LEFT]   = { GPIO_PIN(6,  6) },
  [BUTTON_RIGHT]  = { GPIO_PIN(7, 12) },
  [BUTTON_TOP]    = { GPIO_PIN(7, 15) },
  [BUTTON_BOTTOM] = { GPIO_PIN(7, 14) },
  [BUTTON_CENTER] = { GPIO_PIN(1, 13) },
  [BUTTON_BACK]   = { GPIO_PIN(6, 10) }
};

button_state
button_get_state (button_id button)
{
  return gpio_get(buttons[button].pin);
}

/* initialize the gpio pins necessary for Button functions
 * this is done automatically on startup
 */
static void
__attribute((constructor))
button_init (void)
{
  int i;
  for (i = 0; i <= 5; i++) {
    // Loop over and initialize all 5 Buttons
    gpio_init_inpin(buttons[i].pin);
  }

  // disable pull-up for pin group 25 - required for left button
  *((volatile unsigned int*)(SYSCFG1_BASE + 0x10)) &= 0xFDFFFFFF;
}
