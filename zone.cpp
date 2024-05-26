/* zone.cpp ---
 *
 * Filename: zone.cpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer:
 * Created: Wed May 15 2024
 * Last-Updated:
 *           By:
 *     Update #: 0
 * Keywords:
 * Compatibility:
 *
 */

/* Copyright (c) 2016 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */
#include "zone.hpp"
#include <ncurses.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>

// Initializes zone position and dimensions
zone_t *init_zone(int upper_left_x, int upper_left_y, int width, int height, int goal_width)
{
  zone_t *z;
  z = (zone_t *)malloc(sizeof(zone_t));
  z->upper_left_x = upper_left_x + 10; // added 10 to create some space on the left side of the zone to display messages
  z->upper_left_y = upper_left_y;
  z->width = width - 10;
  z->height = height;
  z->draw_char = '#';
  z->zone_color = 1;
  z->goal_char = '-';
  z->goal_color = 4;
  z->goal_width = goal_width;
  return (z);
}

// Renders zone on the screen with the goal
void draw_zone(zone_t *z)
{
  int row_counter, column_counter;

  attron(COLOR_PAIR(z->zone_color)); // add colors

  // Draw the top side of the zone
  for (row_counter = z->upper_left_x; row_counter <= (z->upper_left_x + z->width); row_counter++)
  {
    mvprintw(z->upper_left_y, row_counter, "%c", z->draw_char);
  }

  // Draw left side of zone
  for (column_counter = z->upper_left_y; column_counter <= (z->upper_left_y + z->height); column_counter++)
  {
    mvprintw(column_counter, z->upper_left_x, "%c", z->draw_char);
  }

  // Draw right side of well
  for (column_counter = z->upper_left_y; column_counter <= (z->upper_left_y + z->height); column_counter++)
  {
    mvprintw(column_counter, (z->upper_left_x + z->width), "%c", z->draw_char);
  }

  // Draw Bottom of zone
  for (row_counter = z->upper_left_x; row_counter < (z->upper_left_x + z->width); row_counter++)
  {
    mvprintw((z->upper_left_y + z->height), row_counter, "%c", z->draw_char);
  }

  // Draw top and bottom goal for zone
  attron(COLOR_PAIR(z->goal_color));

  int goal_start_x = z->upper_left_x + ((z->width - z->goal_width) / 2) + 1;
  for (row_counter = goal_start_x; row_counter < goal_start_x + z->goal_width; row_counter++)
  {
    mvprintw(z->upper_left_y, row_counter, "%c", z->goal_char);
    mvprintw((z->upper_left_y + z->height), row_counter, "%c", z->goal_char);
  }

  attroff(COLOR_PAIR(z->goal_color)); // reset colors
}

// Replaces the zone boundary with blank spaces
void undraw_zone(zone_t *z)
{
  int row_counter, column_counter;

  // Draw the top side of the zone
  for (row_counter = z->upper_left_x; row_counter <= (z->upper_left_x + z->width); row_counter++)
  {
    mvprintw(z->upper_left_y, row_counter, " ", z->draw_char);
  }

  // Draw left side of zone
  for (column_counter = z->upper_left_y; column_counter <= (z->upper_left_y + z->height); column_counter++)
  {
    mvprintw(column_counter, z->upper_left_x, " ", z->draw_char);
  }

  // Draw right side of well
  for (column_counter = z->upper_left_y; column_counter <= (z->upper_left_y + z->height); column_counter++)
  {
    mvprintw(column_counter, (z->upper_left_x + z->width), " ", z->draw_char);
  }

  // Draw Bottom of zone
  for (row_counter = z->upper_left_x; row_counter <= (z->upper_left_x + z->width); row_counter++)
  {
    mvprintw(z->upper_left_y + z->height, row_counter, " ", z->draw_char);
  }
}
