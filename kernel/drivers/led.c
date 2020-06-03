
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

#include "led.h"

#include "kernel/drivers/gpio.h"

/* this struct holds the information on the led pins
 *
 * each LED has two pins
 *   - pin1, for RED on/off
 *   - pin2, for GREEN on/off
 */
struct led_info
{
  unsigned int pin1;
  unsigned int pin2;
};
typedef struct led_info led_info;

led_info leds[] =
{
  [LED_LEFT]  = { GPIO_PIN(6, 13), GPIO_PIN(6,  7) },
  [LED_RIGHT] = { GPIO_PIN(6, 12), GPIO_PIN(6, 14) }
};

void
led_set (led_id led, led_color color)
{
  if (led & LED_LEFT)
    {
      gpio_set(leds[LED_LEFT].pin1, color & 0x01); // Set Pin 1 to the first bit
      gpio_set(leds[LED_LEFT].pin2, (color & 0x02) >> 1); // Set Pin 2 to the second bit
    }
  if (led & LED_RIGHT)
    {
      gpio_set(leds[LED_RIGHT].pin1, color & 0x01);
      gpio_set(leds[LED_RIGHT].pin2, (color & 0x02) >> 1);
    }
}

/* initialize the gpio pins necessary for led functions
 * this is done automatically on startup
 */
static void
__attribute((constructor))
led_init (void)
{
  gpio_init_outpin(leds[LED_LEFT].pin1);
  gpio_init_outpin(leds[LED_LEFT].pin2);
  gpio_init_outpin(leds[LED_RIGHT].pin1);
  gpio_init_outpin(leds[LED_RIGHT].pin2);
}
