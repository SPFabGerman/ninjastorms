
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

#include "sensor.h"

#include "kernel/drivers/gpio.h"
#include "kernel/drivers/adc.h"
#include "kernel/drivers/uart.h"

#include <stdio.h>

struct sensor_port_info
{
  unsigned int pin1;      // I_ONB           - 9V enable (high)
  unsigned int pin2;      // LEGDETB         - Digital input pulled up
  unsigned int pin5;      // DIGIB0          - Digital input/output
  unsigned int pin6;      // DIGIB1          - Digital input/output
  unsigned int buffer;    // Buffer disable
  unsigned char adc1;  // adc channel 1
  unsigned char adc2;  // adc channel 2
};
typedef struct sensor_port_info sensor_port_info;

static sensor_port_info ports[] =
{
  { GPIO_PIN(8, 10), GPIO_PIN(2,  2), GPIO_PIN(0,  2), GPIO_PIN(0, 15), GPIO_PIN(8, 11), 0x6, 0x5 },
  { GPIO_PIN(8, 12), GPIO_PIN(8, 15), GPIO_PIN(0, 14), GPIO_PIN(0, 13), GPIO_PIN(8, 14), 0x8, 0x7 },
  { GPIO_PIN(8,  9), GPIO_PIN(7, 11), GPIO_PIN(0, 12), GPIO_PIN(1, 14), GPIO_PIN(7,  9), 0xA, 0x9 },
  { GPIO_PIN(6,  4), GPIO_PIN(7,  8), GPIO_PIN(0,  1), GPIO_PIN(1, 15), GPIO_PIN(7, 10), 0xC, 0xB },
};

sensor_touch_state
sensor_touch_get_state (sensor_port_id port)
{
  unsigned short Data1 = adc_get(ports[port].adc1);
  unsigned short Data2 = adc_get(ports[port].adc2);

  return ((Data2 > 3000) || (Data1 > 500 && Data1 < 1000) ? SENSOR_TOUCH_DOWN : SENSOR_TOUCH_UP);
}

unsigned short
sensor_light_get (sensor_port_id port)
{
  // turn the light on
  gpio_set(ports[port].pin5, 1);

  return adc_get(ports[port].adc1);
}

int
ev3_sensor_color_get (sensor_port_id port)
{
  port = SENSOR_PORT_2;

  // Setup UART Port
  uart_2_setup();

  // Just wait for a few seconds.
  // printf("Wait Start.\n");
  // int s;
  // for (s = 0; s < 100000; ++s);
  // printf("Wait End.\n");

  // Print out Pin 6 to Wait for Break Condition
  // while (1)
  // {
  //   printf("READ: %i\n", uart_read_pin6(SENSOR_PORT_2));
  // }

  // Wait for Start of Initialization Protocoll
  // TODO: Wait instead for break condition (Pin6 is Low for, ... some time ...)
  int initFound = 0;
  int c = uart_read(SENSOR_PORT_2);
  while (!initFound) {
    while (c != 0x40)
      c = uart_read(SENSOR_PORT_2);

    c = uart_read(SENSOR_PORT_2);
    if (c != 0x1d) {
      continue;
    }

    c = uart_read(SENSOR_PORT_2);
    if (c != 0xa2) {
      continue;
    }

    initFound = 1;
  }

  printf("Init Sequence found. 0x%x\n", uart_ports[SENSOR_PORT_2]->lsr);

  // We now recieved the first three bytes of the initialization protocoll.
  // (Hopefully.)
  int i;
  // for (i = 0; i < 10; i++)
  //   uart_read(SENSOR_PORT_2);

  // Send Bitrate (57600)
  // uart_write(SENSOR_PORT_2, 0x52);
  // uart_write(SENSOR_PORT_2, 0x00);
  // uart_write(SENSOR_PORT_2, 0xe1);
  // uart_write(SENSOR_PORT_2, 0x00);
  // uart_write(SENSOR_PORT_2, 0x00);
  // uart_write(SENSOR_PORT_2, 0x4c);
  // printf("Bitrate Send.\n");

  // Send Acknowgledgement
  // uart_write(SENSOR_PORT_2, 0x04);
  // printf("ACK Send.\n");

  // Flood Buffer with ACK
  // for (i = 0; i < 64; i++) {
  //   uart_ports[SENSOR_PORT_2]->thr = 0x04;
  // }
  // printf("ACK Flood done.\n");

  // Wait for end
  for (i = 0; i < 307; i++)
    uart_read(SENSOR_PORT_2);

  if (uart_read(SENSOR_PORT_2) != 0x04)
  {
    printf("Not at ACK Byte.\n");
    // Print out Hexdump
    while (1)
    {
      int c = uart_read(SENSOR_PORT_2);
      printf("%x ", c);
    }
  }
  printf("ACK Found. 0x%x\n", uart_ports[SENSOR_PORT_2]->lsr);
  // if (uart_read(SENSOR_PORT_2) == 0x00)
  //   printf("SYNC read.\n");

  // Send Acknowgledgement Byte
  // Needs to be done in 80ms!
  // uart_write(SENSOR_PORT_2, 0x04);
  uart_ports[SENSOR_PORT_2]->fcr = 0x07;
  printf("FIFO Clear. 0x%x\n", uart_ports[SENSOR_PORT_2]->lsr);
  uart_ports[SENSOR_PORT_2]->thr = 0x04;
  printf("ACK Send. 0x%x\n", uart_ports[SENSOR_PORT_2]->lsr);

  printf("Wait until ACK recieved. 0x%x\n", uart_ports[SENSOR_PORT_2]->lsr);
  while ((uart_ports[SENSOR_PORT_2]->lsr & 0x60) != 0x60);
  printf("ACK recieved. 0x%x\n", uart_ports[SENSOR_PORT_2]->lsr);

  // uart_write(SENSOR_PORT_2, 0x00);
  // printf("SYNC Send.\n");

  // Change Bitrate
  uart_set_bitrate(SENSOR_PORT_2, 57600);
  printf("Bitrate Set. 0x%x\n", uart_ports[SENSOR_PORT_2]->lsr);

  // uart_ports[SENSOR_PORT_2]->fcr = 0x03;
  // uart_ports[SENSOR_PORT_2]->base_p_12 = (0x3 << 13);
  // printf("UART FIFO Reset.\n");

  // Select Mode 2 (COL-COLOR)
  // uart_write(SENSOR_PORT_2, 0b01000011);
  // uart_write(SENSOR_PORT_2, 2);
  // uart_write(SENSOR_PORT_2, 0xff ^ 2 ^ 0b01000011); // Check Byte
  // printf("Mode Send.\n");

  // uart_write(SENSOR_PORT_2, 0x02);
  // printf("NACK Send.\n");

  // Change Bitrate
  // uart_set_bitrate(SENSOR_PORT_2, 2400);
  // printf("Bitrate Reset.\n");

  // Print out Hexdump
  printf("Start Hexdump: 0x%x\n", uart_ports[SENSOR_PORT_2]->lsr);
  while (1)
  {
    // Send NACK. Needs to be done at least every 300ms.
    // Also causes sensor to resend data.
    uart_write(SENSOR_PORT_2, 0x02); // Dump FIFO with NACK

    int c = uart_read(SENSOR_PORT_2);
    printf("%x ", c);
  }
}

/* initialize the gpio pins necessary for sensor functions
 * this is done automatically on startup
 */
static void
__attribute__((constructor))
sensor_init (void)
{
  unsigned int i;
  for (i = 0; i < sizeof(ports) / sizeof(ports[0]); ++i)
    {
      gpio_init_inpin(ports[i].pin1);
      gpio_init_inpin(ports[i].pin2);
      gpio_init_outpin(ports[i].pin5);
      gpio_init_pin(ports[i].pin6);
      gpio_init_pin(ports[i].buffer);
    }

  // disable pull-dpwn
  *((volatile unsigned int*)(SYSCFG1_BASE + 0x0C)) &= ~0xFFFFFFFF;
}
