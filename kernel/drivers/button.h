
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

#pragma once

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

enum button_id
{
  BUTTON_LEFT = 0x00,
  BUTTON_RIGHT = 0x01,
  /* The Button over the Central Button.
   * Not to be confused with BUTTON_UP. */
  BUTTON_TOP = 0x02,
  /* The Button under the Central Button.
   * Not to be confused with BUTTON_DOWN. */
  BUTTON_BOTTOM = 0x03,
  BUTTON_CENTER = 0x04,
  /* The Button in the top left corner. */
  BUTTON_BACK = 0x05
};
typedef enum button_id button_id;

enum button_state
{
  /* Signals, that the Button is not pressed.
   * Identical to BUTTON_STATE_UP.
   * Kept for backwards compatability.
   * Not to be confused with BUTTON_TOP.
   */
  BUTTON_UP = 0x00,
  /* Signals, that the Button is pressed.
   * Identical to BUTTON_STATE_DOWN.
   * Kept for backwards compatability.
   * Not to be confused with BUTTON_BOTTOM.
   */
  BUTTON_DOWN = 0x01
};
typedef enum button_state button_state;
// Define some additional aliases for clarity.

/* Signals, that the Button is not pressed.
 * Identical to BUTTON_UP.
 */
static const button_state BUTTON_STATE_UP = BUTTON_UP;
/* Signals, that the Button is pressed.
 * Identical to BUTTON_DOWN.
 */
static const button_state BUTTON_STATE_DOWN = BUTTON_DOWN;

/* get the state of a button
 *
 * params:
 *   - button_id: the button to probe, must be in enum button_id
 *
 * returns:
 *   the state of the probed button, either BUTTON_STATE_UP (0) or BUTTON_STATE_DOWN (1)
 */
button_state button_get_state (button_id button);
