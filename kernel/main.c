
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

#include "kernel/drivers/button.h"
#include "kernel/scheduler.h"
#include "memory.h"
#include "kernel/utilities.h"

#include "syscall.h"

#include <stdio.h>
#include <errno.h>

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
  int direct_call_result = add_task(&task_c);
  if(direct_call_result<0){
    printf("add task failed with errno %i \n",errno);
  }
  while (1)
    {
      printf("  task d: %i\n", n++);
      volatile int i;
      for (i = 0; i < 10000000; ++i);
      if(n>5){
          //That's enough!
          shutdown();
      }
    }
}


static void
syscall_test(void)
{
    create_process(&task_a);
    create_process(&task_b);
}

static void
getchar_test(void)
{
#ifdef BOARD_EV3
  while (1) {
    volatile int i;
    printf("Type a character!\n");
    int c = getchar();
    printf("Your character was: %c\n", c);
  }
#endif
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

  // add_task(&task_a);
  // add_task(&task_b);
  // add_task(&task_c);
  add_task(&task_d);
#ifndef BOARD_EV3
  // Seems to block out all task on EV3
  add_task(&syscall_test);
#endif

  add_task(&getchar_test);

  start_scheduler();

  puts("All done. ninjastorms out!");

  return 0;
}
