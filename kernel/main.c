
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

#include "main.h"

#include "kernel/scheduler.h"
#include "kernel/drivers/led.h"

#include "kernel/demo/demo_button.c"

#include <stdio.h>

static void
task_a (void)
{
  unsigned int n = 0;

  while (1)
    {
      printf("  task a: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
    }
}

static void
task_b (void)
{
  unsigned int n = 0;

  while (1)
    {
      printf("  task b: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
    }
}

static void
task_c (void)
{
  unsigned int n = 0;

  while (1)
    {
      printf("  task c: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
    }
}

static void
task_d (void)
{
  unsigned int n = 0;

  while (1)
    {
      printf("  task d: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
    }
}

static void
task_button_leds (void)
{
  printf("Start the LED Task.\n");
  printf("Press and hold the Left/Right Button to specify which LED you want to change.\n");
  printf("You can select the color with the top, center and bottom Button.\n");
  
  while (1)
  {
    button_state newState;

    if ((newState = button_get_state(BUTTON_TOP)))
    {
      if (button_get_state(BUTTON_LEFT) == BUTTON_DOWN)
      {
        led_set(LED_LEFT, LED_GREEN);
      }
      if (button_get_state(BUTTON_RIGHT) == BUTTON_DOWN)
      {
        led_set(LED_RIGHT, LED_GREEN);
      }
    }

    if ((newState = button_get_state(BUTTON_BOTTOM)))
    {
      if (button_get_state(BUTTON_LEFT) == BUTTON_DOWN)
      {
        led_set(LED_LEFT, LED_RED);
      }
      if (button_get_state(BUTTON_RIGHT) == BUTTON_DOWN)
      {
        led_set(LED_RIGHT, LED_RED);
      }
    }

    if ((newState = button_get_state(BUTTON_CENTER)))
    {
      if (button_get_state(BUTTON_LEFT) == BUTTON_DOWN)
      {
        led_set(LED_LEFT, LED_ORANGE);
      }
      if (button_get_state(BUTTON_RIGHT) == BUTTON_DOWN)
      {
        led_set(LED_RIGHT, LED_ORANGE);
      }
    }

    if ((newState = button_get_state(BUTTON_BACK)))
    {
      if (button_get_state(BUTTON_LEFT) == BUTTON_DOWN)
      {
        led_set(LED_LEFT, LED_BLACK);
      }
      if (button_get_state(BUTTON_RIGHT) == BUTTON_DOWN)
      {
        led_set(LED_RIGHT, LED_BLACK);
      }
    }
  }
}

char shuriken[] =
"                 /\\\n"
"                /  \\\n"
"                |  |\n"
"              __/()\\__\n"
"             /   /\\   \\\n"
"            /___/  \\___\\\n";

int
kernel_main (void)
{
  puts("This is ninjastorms OS");
  puts("  shuriken ready");
  puts(shuriken);

  add_task(&task_a);
  add_task(&task_b);
  add_task(&task_c);
  add_task(&task_d);

  add_task(&task_button_leds);
  
  add_task(&button_test);

  start_scheduler();

  puts("All done. ninjastorms out!");

  return 0;
}
